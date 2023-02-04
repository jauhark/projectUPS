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

    RAMPGEN_reset(&rgen1);
    rgen1.freq = (float32_t) (AC_FREQ);
    rgen1.stepAngleMax = (float32_t) (2.0f / INV_PWM_SWITCHING_FREQUENCY);

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

    myCtrl.Umax=0.8;
    myCtrl.Umin=-0.8;
    myCtrl.Kp=_dclCoeff_Kp;
    myCtrl.Ki=_dclCoeff_Ki;

#endif
//----------------------------------------------------------
#endif
    myCtrl.Umax=0.9;
    myCtrl.Umin=0.1;
    myCtrl.Kp=_dclCoeff_Kp;
    myCtrl.Ki=_dclCoeff_Ki;

    //================================================================
    /*
     *SETUP LEDPIN at pin34
     */
    GPIO_setDirectionMode(LED_PIN_SWITCH, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(LED_PIN_SWITCH, GPIO_PIN_TYPE_STD);

    GPIO_setDirectionMode(LED_PIN_ZCROSS_DETECTION, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(LED_PIN_ZCROSS_DETECTION, GPIO_PIN_TYPE_STD);

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

    uint16_t count=0;

    //================================================================
    while (1)
    {
        SW_PRESSED_VAL = _GET_SWITCH_VAL();

        if(count==100){
        float32_t temp=voltRMSTotalCycle;
        memset(LCD_DATA[0], 0, sizeof(LCD_DATA[0]));
        memset(LCD_DATA[1], 0, sizeof(LCD_DATA[1]));
        lcdVal1p000=(uint16_t)temp;
        lcdVal0p100=((uint16_t)(temp*10))%(lcdVal1p000*10);
        lcdVal0p010=((uint16_t)(temp*100))%(lcdVal1p000*100+lcdVal0p100*10);
        lcdVal0p001=((uint16_t)(temp*1000))%(lcdVal1p000*1000+lcdVal0p100*100+lcdVal0p010*10);

        sprintf(LCD_DATA[0], "RMS: %u.%u%u%u", lcdVal1p000, lcdVal0p100, lcdVal0p010, lcdVal0p001);

        temp=voltPeakTotalCycle;
        lcdVal1p000=(uint16_t)temp;
        lcdVal0p100=((uint16_t)(temp*10))%(lcdVal1p000*10);
        lcdVal0p010=((uint16_t)(temp*100))%(lcdVal1p000*100+lcdVal0p100*10);
        lcdVal0p001=((uint16_t)(temp*1000))%(lcdVal1p000*1000+lcdVal0p100*100+lcdVal0p010*10);

        sprintf(LCD_DATA[1], "PEAK: %u.%u%u%u", lcdVal1p000, lcdVal0p100, lcdVal0p010, lcdVal0p001);

        lcdDrive_updateLCD_DATA(LCD_DATA);
        count=0;
        }
        count++;

        lcdDrive_updateLCD();

#if SFRA_ENABLED==1
        SFRA_F32_runBackgroundTask(&sfra1);
        SFRA_GUI_runSerialHostComms(&sfra1);
        DEVICE_DELAY_US(9000); //10mS delay
#endif
    }
}
/*
 * ###################################################################################
 * ###################################################################################
 */

//TODO Inverter ISR Code
interrupt void inverterISR(void)
{
    RAMPGEN_run(&rgen1);
    invSine = sinf((rgen1.out) * 6.283185307f);

    //TODO-Implement control system
    //DONE ADC
    //DONE PWM
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

//    error=invSine-voltCompVal;

    //================================================================
/*
 * AVERAGE CONTROL CODE
 */
#if CONTROL_MODE ==AVERAGE_CONTROL
    //softStart
    if(invModIndex<INVMOD_AVG)invModIndex+=0.0001;

    if (ADC_getPPBEventStatus(ADCA_BASE, ADC_PPB_NUMBER1)==ADC_EVT_ZERO)
    {
        if (adcRes > adcResPrev)
        {

            GPIO_writePin(LED_PIN_ZCROSS_DETECTION, 1);
            /*
             * positive cycle detected
             *  halfCycleFlag=1
             * voltAverage calculates average voltage of previous -ve cycle
             */
            halfCycleFlag=1;

            voltAveragePrev=voltAverage;
            voltAverage =fabs((voltSumNHalf)/ sampleCount);
//            voltAverage=sqrt(voltSumNHalf/sampleCount);
            voltRMSTotalCycle = (voltAverage + voltAveragePrev)*1.11/ 2.0f;
            voltPeakTotalCycle=(voltAverage+voltAveragePrev)*0.7853981;


            /*TODO:
             * when previous negative cycle has higher power content
             * than previous positive cycle, then increase the offset
             */
//            OFFSET_ERROR=voltAverage-voltAveragePrev;
//            if(OFFSET_ERROR>0.001){
//                /*
//                 * voltaverage contains previous negative cycle values
//                 * voltaverageprev contains previous previous positive cycle values
//                 * if error is positive, then negative cycle contains more power
//                 * positive offset should be added
//                 */
//                OFFSET_CONTROL_VAL+=-0.0001;
//            }else if(OFFSET_ERROR<-0.001){
//                OFFSET_CONTROL_VAL+=0.0001;
//            }


            sampleCount = 0;
            voltSumPHalf=0;
        }
        else if (adcRes < adcResPrev)
        {
            GPIO_writePin(LED_PIN_ZCROSS_DETECTION, 0);
            /*
             * negative cycle detected
             *  halfCycleFlag=0
             * voltAverage calculates average voltage of previous +ve cycle
             */
            halfCycleFlag=0;

            voltAveragePrev=voltAverage;
            voltAverage =fabs((voltSumPHalf) / sampleCount);
//            voltAverage=sqrt(voltSumPHalf/sampleCount);
            voltRMSTotalCycle = (voltAverage + voltAveragePrev)*1.11/ 2.0f;
            voltPeakTotalCycle=(voltAverage+voltAveragePrev)*0.7853981;
            /*TODO:
             * when previous positive cycle has higher power content
             * than previous negative cycle, then decrease the offset
             */
//            OFFSET_ERROR=voltAverage-voltAveragePrev;
//            if(OFFSET_ERROR>0.001){
//                /*
//                 * voltaverage contains previous positive cycle values
//                 * voltaverageprev contains previous previous negative cycle values
//                 * if error is positive, then positive cycle contains more power
//                 * negative offset should be added
//                 */
//                OFFSET_CONTROL_VAL+=0.0001;
//            }else if(OFFSET_ERROR<-0.001){
//                OFFSET_CONTROL_VAL+=-0.0001;
//            }

            sampleCount=0;
            voltSumNHalf=0;
        }


        invModIndex=DCL_runPI_C3(&myCtrl, ref_peakVal, voltPeakTotalCycle);

    }

    if (halfCycleFlag)
    {
        /* positive cycle */
        voltSumPHalf += voltVal;
//        voltSumPHalf += (voltVal*voltVal);
        sampleCount++;
    }
    else
    {
        /* negative cycle */
        sampleCount++;
        voltSumNHalf += voltVal;
//        voltSumNHalf+=(voltVal*voltVal);
    }

    //--------------------------------------------------
//    invDutyPU=invSine*invModIndex+OFFSET_CONTROL_VAL;
    invDutyPU=invSine*invModIndex;
    //--------------------------------------------------

    ADC_clearPPBEventStatus(ADCA_BASE, ADC_PPB_NUMBER1, ADC_EVT_ZERO);
    ADC_clearPPBEventStatus(ADCA_BASE, ADC_PPB_NUMBER1, ADC_EVT_TRIPHI);
    ADC_clearPPBEventStatus(ADCA_BASE, ADC_PPB_NUMBER1, ADC_EVT_TRIPLO);
#endif
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
    invDutyPU=DCL_runDF23_C4(&myCtrl, error );

    GPIO_togglePin(LED_PIN_ZCROSS_DETECTION);
//    invDutyPU=invSine*invModIndex;
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
    invDutyPU=DCL_runPI_C3(&myCtrl, invSine, voltCompVal);

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

    mainsVoltage[k] = voltCompVal;
//    rmsVoltage[k]=voltRMSTotalCycle*1.414314f*2.0f;
//    rmsVoltage[k]=voltRMSTotalCycle;
//    errorVals[k] = OFFSET_ERROR;
//    errorVals[k]=invSine;
//    rmsVoltage[k] = invSine;
    referenceVoltage[k] = invSine ;
    adcRes_Iin_Buffer[k] = adcRes_Iin;
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
//    SFRA_F32_collect((float*) &invDutyPU, (float*) &voltCompVal);

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
