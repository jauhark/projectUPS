/*
 * Jauhar K
 * 15-12-2022
 * PROJECT UPS
 * tms320F280049C
 * THIS IS THE MAIN PROJECT
 */

#include "project_main.h"

/*
 * ###################################################################################
 * ###################################################################################
 */
void main(void)

{

    setupDevice();
    disablePWMCLKCounting();
    //================================================================
    /*
     * setupInverterPWM()
     * @params: Base1, Base2, PWM_PERIOD, deadBand_risingEdge, deadband_fallingEdge
     */
    setupInverterPWM(EPWM1_BASE, EPWM2_BASE,
    INV_PWM_PERIOD,
                     INV_DEADBAND_PWM_COUNT,
                     INV_DEADBAND_PWM_COUNT);

    setupADC();

    _sineLTCounter = 0;

    //================================================================
    /*
     * SFRA INITS
     * TODO
     */
#if SFRA_ENABLED==1
    SFRA_F32_reset(&sfra1);
    SFRA_F32_config(&sfra1,
    SFRA_ISR_FREQ,
                    SFRA_AMPLITUDE,
                    SFRA_FREQ_LENGTH,
                    SFRA_FREQ_START,
                    SFRA_FREQ_STEP_MULTIPLY,
                    plantMagVect, plantPhaseVect, olMagVect, olPhaseVect,
                    clMagVect, clPhaseVect, freqVect, 1);
    SFRA_F32_resetFreqRespArray(&sfra1);
    SFRA_F32_initFreqArrayWithLogSteps(&sfra1, SFRA_FREQ_START,
    SFRA_FREQ_STEP_MULTIPLY);

    SFRA_GUI_config(SFRA_GUI_SCI_BASE,
    SFRA_GUI_VBUS_CLK,
                    SFRA_GUI_SCI_BAUDRATE,
                    SFRA_GUI_SCIRX_GPIO,
                    SFRA_GUI_SCIRX_GPIO_PIN_CONFIG,
                    SFRA_GUI_SCITX_GPIO,
                    SFRA_GUI_SCITX_GPIO_PIN_CONFIG,
                    SFRA_GUI_LED_INDICATOR,
                    SFRA_GUI_LED_GPIO,
                    SFRA_GUI_LED_GPIO_PIN_CONFIG,
                    &sfra1, 1);
    /*
     * SFRA INIT ENDS
     */
#endif
    //================================================================
    /*
     * TODO
     * SETTING UP CONTROL LOOP
     */
#if CONTROL_MODE==INSTANT_CONTROL
//----------------------------------------------------------
#if CONTROL_TYPE==TYPE3_CONTROL

    DCL_resetDF23(&myCtrl);

    myCtrl.b0 = _dclCoeff_B0;
    myCtrl.b1 = _dclCoeff_B1;
    myCtrl.b2 = _dclCoeff_B2;
    myCtrl.b3 = _dclCoeff_B3;
    myCtrl.a1 = _dclCoeff_A1;
    myCtrl.a2 = _dclCoeff_A2;
    myCtrl.a3 = _dclCoeff_A3;
#endif

#if CONTROL_TYPE==PI_CONTROL

    myCtrl.Umax=0.1;
    myCtrl.Umin=-0.1;
    myCtrl.Kp=_dclCoeff_Kp;
    myCtrl.Ki=_dclCoeff_Ki;

#endif
#endif

#if CONTROL_MODE==AVERAGE_CONTROL
    myCtrl.Umax = 0.85;
    myCtrl.Umin = 0.1;
    myCtrl.Kp = _dclCoeff_Kp;
    myCtrl.Ki = _dclCoeff_Ki;

    myOffsetControlPI.Umax=0.01;
    myOffsetControlPI.Umin=-0.01;
    myOffsetControlPI.Kp=0.05;
    myOffsetControlPI.Ki=0.01;

    DCL_resetDF23(&my3p3zCtrl);

    my3p3zCtrl.b0 = _dclCoeff_B0;
    my3p3zCtrl.b1 = _dclCoeff_B1;
    my3p3zCtrl.b2 = _dclCoeff_B2;
    my3p3zCtrl.b3 = _dclCoeff_B3;
    my3p3zCtrl.a1 = _dclCoeff_A1;
    my3p3zCtrl.a2 = _dclCoeff_A2;
    my3p3zCtrl.a3 = _dclCoeff_A3;
#endif
//----------------------------------------------------------
    //================================================================
    /*
     *SETUP LEDPINS
     */
    GPIO_setDirectionMode(LED_PIN_SWITCH, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(LED_PIN_SWITCH, GPIO_PIN_TYPE_STD);

    GPIO_setDirectionMode(LED_PIN_ZCROSS_DETECTION, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(LED_PIN_ZCROSS_DETECTION, GPIO_PIN_TYPE_STD);

    /*
     * SETUP RELAY PINS
     */
    GPIO_setDirectionMode(RELAY_SWITCH, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(RELAY_SWITCH, GPIO_PIN_TYPE_STD);

    /*
     * SETUP PINOUT for LCD_DRIVE
     */
    GPIO_setDirectionMode(lcd_EN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(lcd_EN, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(lcd_RS, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(lcd_RS, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(lcd_DB4, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(lcd_DB4, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(lcd_DB5, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(lcd_DB5, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(lcd_DB6, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(lcd_DB6, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(lcd_DB7, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(lcd_DB7, GPIO_PIN_TYPE_STD);
    /*
     * END LCDDrive PIN SETUP
     */

    lcdDrive_initialise();
    lcdDrive_clearDisplay();
    lcdDrive_updateLCD_DATA(LCD_DATA);
    // ISR Mapping
    setupInterrupt();
    // Enable PWM Clocks
    enablePWMCLKCounting();

    uint16_t countLCD = 0;
    uint16_t countSwitch = 0;
    float32_t temp = 0;

    //================================================================
    while (1)
    {
        SW_PRESSED_VAL = _GET_SWITCH_VAL();
        /*
         * LCD DATA PARSING AND UPDATING
         * ------------------------------------------------------------------
         */
        if (countLCD == 100)
        {
            temp = voltRMSTotalCycle;
            memset(LCD_DATA[0], 0, sizeof(LCD_DATA[0]));
            memset(LCD_DATA[1], 0, sizeof(LCD_DATA[1]));
            lcdVal1p000 = (uint16_t) temp;
            lcdVal0p100 = ((uint16_t) (temp * 10)) % (lcdVal1p000 * 10);
            lcdVal0p010 = ((uint16_t) (temp * 100))
                    % (lcdVal1p000 * 100 + lcdVal0p100 * 10);
            lcdVal0p001 =
                    ((uint16_t) (temp * 1000))
                            % (lcdVal1p000 * 1000 + lcdVal0p100 * 100
                                    + lcdVal0p010 * 10);

            sprintf(LCD_DATA[0], "RMS: %u.%u%u%u", lcdVal1p000, lcdVal0p100,
                    lcdVal0p010, lcdVal0p001);

            temp = voltPeakTotalCycle;
            lcdVal1p000 = (uint16_t) temp;
            lcdVal0p100 = ((uint16_t) (temp * 10)) % (lcdVal1p000 * 10);
            lcdVal0p010 = ((uint16_t) (temp * 100))
                    % (lcdVal1p000 * 100 + lcdVal0p100 * 10);
            lcdVal0p001 =
                    ((uint16_t) (temp * 1000))
                            % (lcdVal1p000 * 1000 + lcdVal0p100 * 100
                                    + lcdVal0p010 * 10);

            sprintf(LCD_DATA[1], "PEAK: %u.%u%u%u", lcdVal1p000, lcdVal0p100,
                    lcdVal0p010, lcdVal0p001);

            lcdDrive_updateLCD_DATA(LCD_DATA);
            countLCD = 0;
        }
        countLCD++;

        lcdDrive_updateLCD();
        /*
         * -------------------------------------------------------------------
         */

        if (SW_PRESSED_VAL == 15)
        {
            GPIO_togglePin(RELAY_SWITCH);
            DEVICE_DELAY_US(100000);
        }

#if SFRA_ENABLED==1
        SFRA_F32_runBackgroundTask(&sfra1);
        SFRA_GUI_runSerialHostComms(&sfra1);
        DEVICE_DELAY_US(9000); //10mS delay
#endif
        DEVICE_DELAY_US(1000);
    }
}
/*
 * ###################################################################################
 * ###################################################################################
 */

//TODO Inverter ISR Code
interrupt void inverterISR(void)
{
//    RAMPGEN_run(&rgen1);
//    invSine = sinf((rgen1.out) * 6.283185307f);
    invSine = SFRA_F32_inject(_sineLTable[_sineLTCounter]);
    _sineLTCounter++;
    if (_sineLTCounter >= 200)
        _sineLTCounter = 0;
    clearInterruptEPWM();

}    // MainISR Ends Here
/*
 * ###################################################################################
 * ###################################################################################
 */

interrupt void adcISR(void)
{
    adcRes = (int16_t) _GETRES_SOC0;
    adcRes_Iin = (int16_t) _GETRES_IN_Idc
    ;

    /*SFRA INJECT*/
//    invDutyPU = invModIndex*(SFRA_F32_inject(invSine));
    voltVal = ((float32_t) adcRes) * (-3.3f / 4096.0f);

    voltCompVal = ((float32_t) adcRes) * ((float32_t) MAINSADCGAINFACTOR);

    //================================================================
    //softStart
    if (invModIndex < INVMOD_AVG)
        invModIndex += 0.0001;

    /*
     * average voltage calculation
     */
    if (_sineLTCounter==0)
    {

        GPIO_writePin(LED_PIN_ZCROSS_DETECTION, 1);
        /*
         * positive cycle detected
         *  halfCycleFlag=1
         * voltAverage calculates average voltage of previous -ve cycle
         */
        halfCycleFlag = 1;

        voltAveragePrev = voltAverage;

        voltAverage = fabs((voltSumNHalf) / sampleCount);
        voltRMSTotalCycle = (voltAverage + voltAveragePrev) * 1.11 / 2.0f;
        voltPeakTotalCycle = (voltAverage + voltAveragePrev) * 0.7853981;

        OFFSET_ERROR=voltAverage-voltAveragePrev;
        OFFSET_CONTROL_VAL=DCL_runPI_C2(&myOffsetControlPI, 0, OFFSET_ERROR);

        sampleCount = 0;
        voltSumPHalf = 0;

        /*
         * PI CONTROL
         */
        invModIndex=DCL_runPI_C2(&myCtrl, ref_peakVal, voltPeakTotalCycle);


    }
    else if (_sineLTCounter==100)
    {
        GPIO_writePin(LED_PIN_ZCROSS_DETECTION, 0);
        /*
         * negative cycle detected
         *  halfCycleFlag=0
         * voltAverage calculates average voltage of previous +ve cycle
         */
        halfCycleFlag = 0;

        voltAveragePrev = voltAverage;

        voltAverage = fabs((voltSumPHalf) / sampleCount);
        voltRMSTotalCycle = (voltAverage + voltAveragePrev) * 1.11 / 2.0f;
        voltPeakTotalCycle = (voltAverage + voltAveragePrev) * 0.7853981;



        sampleCount = 0;
        voltSumNHalf = 0;

        /*
         * PI CONTROL
         */
        invModIndex=DCL_runPI_C2(&myCtrl, ref_peakVal, voltPeakTotalCycle);

    }


    if (halfCycleFlag)
    {
        /* positive cycle */
        voltSumPHalf += voltVal;
        sampleCount++;
    }
    else
    {
        /* negative cycle */
        sampleCount++;
        voltSumNHalf += voltVal;
    }
    /*
     * END avg voltage calculation
     */

#if CONTROL_MODE ==AVERAGE_CONTROL
    //--------------------------------------------------
    invDutyPU = (invSine+OFFSET_CONTROL_VAL) * invModIndex;
    //--------------------------------------------------

#endif

//    ADC_clearPPBEventStatus(ADCA_BASE, ADC_PPB_NUMBER1, ADC_EVT_ZERO);
//    ADC_clearPPBEventStatus(ADCA_BASE, ADC_PPB_NUMBER1, ADC_EVT_TRIPHI);
//    ADC_clearPPBEventStatus(ADCA_BASE, ADC_PPB_NUMBER1, ADC_EVT_TRIPLO);

    /*
     * AVREAGE CONTROL CODE ENDS
     */

    //================================================================
    /*
     *  INSTANT CONTROL CODE STARTS
     *  */
#if CONTROL_MODE==INSTANT_CONTROL

#if CONTROL_TYPE==TYPE3_CONTROL
    /*TODO
     * CONTROL TYPE 3 CODE HERE
     */

    error=invSine-voltCompVal;
//    controlVal=0;
    controlVal=DCL_runDF23_C4(&myCtrl, error );

    if(controlVal>CtrlValLimit)controlVal=CtrlValLimit;
    else if(controlVal<(float32_t)(-1.0f*CtrlValLimit))controlVal=(float32_t)(-1.0f*CtrlValLimit);

    invDutyPU=invSine*invModIndex+controlVal;

//    if(invDutyPU>0.9)invDutyPU=0.9;
//    if(invDutyPU<-0.9)invDutyPU=-0.9;
    /*
     * END CODE
     */
#endif

#if CONTROL_TYPE==PI_CONTROL
    /*TODO
     * CONTROL TYPE PI CONTROL CODE HERE
     */
    error=invSine-voltCompVal;
    controlVal=DCL_runPI_C3(&myCtrl, invSine, voltCompVal);
    invDutyPU=invSine*invModIndex+controlVal;

    /*
     * END CODE
     */
#endif
#endif
    /*
     * INSTANT CONTROL CODE ENDS
     */
    //================================================================
    /*
     * OPEN LOOP CONTROL
     */
#if CONTROL_MODE==OPEN_LOOP
    invDutyPU=SFRA_F32_inject(invSine*invModIndex);
#endif

    //================================================================
#if DEBUG==1
    /*
     * DEBUG
     */
    volatile static uint32_t kinit = 0;
    volatile static uint16_t logData = 0;
    volatile static uint16_t k = 0;

    graph_2[k]=voltCompVal;
//    graph_2[k]=error;
    graph_1[k] = invDutyPU;
//    graph_4[k]=controlVal;

    k++;
    if (k >= SAMPLENO)
        k = 0;

    if (SW_PRESSED_VAL == 5)
    {
//        GPIO_togglePin(LED_PIN_SWITCH);
        SW_PRESSED_VAL = 0;
    }
    /*
     * DEBUG END
     */

#endif

    //================================================================

    updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, invDutyPU);
    //TODO: CONTROL SYSTEM

    adcResPrev = adcRes;

    /*SFRA COLLECT*/
    SFRA_F32_collect((float*) &invDutyPU, (float*) &voltCompVal);
    clearInterruptADC();
}
/*
 * ###################################################################################
 * ###################################################################################
 */

/*
 *
 //=======================================
 if (logData == 1)
 {


 if (kinit < 10000)
 {
 kinit++;
 k=0;
 }
 else
 {
 if (k < SAMPLENO)
 {
 mainsVoltage[k]=voltVal;
 //                mainsVoltage[k]=adcRes;
 rmsVoltage[k]=voltRMSTotalCycle;
 k++;
 }

 if (k >= SAMPLENO)
 {
 NOP;
 logData=0;
 }
 }

 }
 */
/*
 * END DEBUG
 */
