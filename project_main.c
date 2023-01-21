/*
 * Jauhar K
 * 15-12-2022
 * PROJECT UPS
 * tms320F280049C
 * THIS IS THE MAIN PROJECT
 */

#include "project_main.h"




//================================================================
/*
 * ############################################################################
 */
void main(void)

 {

    setupDevice();
    disablePWMCLKCounting();

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

    /*
     * SFRA INITS
     * TODO
     */
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

    /*
     * TODO
     * SETTING UP CONTROL LOOP
     */
//    DCL_resetDF23(&myCtrl);
//
//    myCtrl.b0=_dclCoeff_B0;
//    myCtrl.b1=_dclCoeff_B1;
//    myCtrl.b2=_dclCoeff_B2;
//    myCtrl.b3=_dclCoeff_B3;
//    myCtrl.a1=_dclCoeff_A1;
//    myCtrl.a2=_dclCoeff_A2;
//    myCtrl.a3=_dclCoeff_A3;

    myPiCtrl.Umax=0.3;
    myPiCtrl.Umin=-0.3;
    myPiCtrl.Kp=0.1;
    myPiCtrl.Ki=0;

    /*
     *
     */
    //
    // ISR Mapping
    setupInterrupt();
    // Enable PWM Clocks
    enablePWMCLKCounting();

    /*
     *SETUP LEDPIN at pin34
     */
    GPIO_setDirectionMode(LED_PIN_SWITCH, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(LED_PIN_SWITCH, GPIO_PIN_TYPE_STD);

    GPIO_setDirectionMode(LED_PIN_ZCROSS_DETECTION, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(LED_PIN_ZCROSS_DETECTION, GPIO_PIN_TYPE_STD);

//    setupInterrupt();

    while (1)
    {
        SW_PRESSED_VAL = _GET_SWITCH_VAL();
        SFRA_F32_runBackgroundTask(&sfra1);
        SFRA_GUI_runSerialHostComms(&sfra1);
        GPIO_togglePin(LED_PIN_ZCROSS_DETECTION);
        DEVICE_DELAY_US(9000); //10mS delay
    }

}

//===============================================================
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

//================================================================
uint16_t _temp = 0;

interrupt void adcISR(void)
{
    adcRes = (int16_t) _GETRES_SOC0;
    adcRes_Iin = (int16_t) _GETRES_IN_Idc;

    /*SFRA INJECT*/
//    invDutyPU = invModIndex*(SFRA_F32_inject(invSine));

    voltVal = ((float32_t) adcRes) * (-3.3f / 4096.0f);

    voltCompVal=((float32_t)adcRes)*((float32_t)MAINSADCGAINFACTOR);

#if AVG_CONTROL ==1
    _temp = ADC_getPPBEventStatus(ADCA_BASE, ADC_PPB_NUMBER1);

    if (_temp == ADC_EVT_ZERO)
    {
        if (adcRes > adcResPrev)
        {
            /* positive cycle detected */
            voltSumPHalf = 0;
            voltAverage = sqrt(voltSumNHalf / sampleCount);
            sampleCount = 0;
            halfCycleFlag = 1;
            GPIO_writePin(LED_PIN_ZCROSS_DETECTION, 1);
            voltRMSTotalCycle = (voltAverage + voltAveragePrev) / 2;
            voltAveragePrev = voltAverage;
        }
        else if (adcRes < adcResPrev)
        {
            /* negative cycle detected*/
            voltSumNHalf = 0;
            voltAverage = sqrt(voltSumPHalf / sampleCount);
            sampleCount = 0;
            halfCycleFlag = 0;
            GPIO_writePin(LED_PIN_ZCROSS_DETECTION, 0);
            voltRMSTotalCycle = (voltAverage + voltAveragePrev) / 2;
            voltAveragePrev = voltAverage;
        }
    }

    if (halfCycleFlag)
    {
        /* positive cycle */
        voltSumPHalf += (voltVal * voltVal);
        sampleCount++;
    }
    else
    {
        /* negative cycle */
        sampleCount++;
        voltSumNHalf += (voltVal * voltVal);
    }

    ADC_clearPPBEventStatus(ADCA_BASE, ADC_PPB_NUMBER1, ADC_EVT_ZERO);
    ADC_clearPPBEventStatus(ADCA_BASE, ADC_PPB_NUMBER1, ADC_EVT_TRIPHI);
    ADC_clearPPBEventStatus(ADCA_BASE, ADC_PPB_NUMBER1, ADC_EVT_TRIPLO);
#endif




    error=invSine-voltCompVal;
//    invModIndex=DCL_runDF23_C4(&myCtrl, error);
    invModIndex=DCL_runPI_C2(&myPiCtrl, invSine, voltCompVal);

//    if(invModIndex>0.3)invModIndex=0.3;
//    else if(invModIndex<-0.3)invModIndex=-0.3;

    invDutyPU=invSine*(0.6+invModIndex);
//    invDutyPU=invSine*invModIndex;

    if(invDutyPU>0.9)invDutyPU=0.9;
    else if(invDutyPU<-0.9)invDutyPU=-0.9;

//    invDutyPU=SFRA_F32_inject(invSine);


#if DEBUG==1
    /*
     * DEBUG
     */
    volatile static uint32_t kinit = 0;
    volatile static uint16_t logData = 0;
    volatile static uint16_t k = 0;

    mainsVoltage[k] = voltCompVal;
//    rmsVoltage[k]=voltRMSTotalCycle*1.414314f;
    errorVals[k]=error;
    rmsVoltage[k] = invSine;
    controlVals[k]=invDutyPU;
    adcRes_Iin_Buffer[k] = adcRes_Iin;
    k++;
    if (k >= SAMPLENO)
        k = 0;
    //=======================================

    if (SW_PRESSED_VAL == 5)
    {
//        GPIO_togglePin(LED_PIN_SWITCH);
        SW_PRESSED_VAL = 0;
    }
    /*
     * DEBUG END
     */

#endif


    updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, invDutyPU);
    //TODO: CONTROL SYSTEM

    adcResPrev = adcRes;

    /*SFRA COLLECT*/
//    SFRA_F32_collect((float *)&invDutyPU, (float*)&voltCompVal);
    clearInterruptADC();
}

//================================================================

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
