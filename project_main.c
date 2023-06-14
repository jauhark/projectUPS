/*
 * Jauhar K
 * 15-12-2022
 * PROJECT UPS
 * tms320F280049C
 * THIS IS THE MAIN PROJECT
 */

#include "project_main.h"

    uint16_t countLCD = 0;
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

    /*
     * STATE MACHINE INITS
     */

    STATE_PTR = &SYSTEM_OFF_STATE;

    /*
     * -----------------------------------
     */

    SINE_MEASUREMENT_reset(&obj_invInst_V);
    SINE_MEASUREMENT_reset(&obj_invLoad_I);

    POWER_MEAS_SINE_ANALYZER_reset(&obj_mains_chgr_V_I);
    obj_mains_chgr_V_I.sampleFreq = (float32_t) (INV_ISR_FREQ);
    obj_mains_chgr_V_I.threshold = (float32_t) (0.25);
    obj_mains_chgr_V_I.nSamplesMax = INV_ISR_FREQ / (AC_FREQ - 10);
    obj_mains_chgr_V_I.nSamplesMin = INV_ISR_FREQ / (AC_FREQ + 10);
    obj_mains_chgr_V_I.emaFilterMultiplier = 2.0f / INV_ISR_FREQ;

    PR_0_Vals.KP = 0.5;
    PR_0_Vals.Ki = 1000000.0;
    PR_0_Vals.Wr = 2e-05;
    PR_0_Vals.Wo = 50;

    PR_3_Vals.KP = 0.25;
    PR_3_Vals.Ki = 100000.0;
    PR_3_Vals.Wr = 1e-03;
    PR_3_Vals.Wo = 50 * 3;

    PR_5_Vals.KP = 0.1;
    PR_5_Vals.Ki = 10000;
    PR_5_Vals.Wr = 1e-3;
    PR_5_Vals.Wo = 50 * 5;

    PR_7_Vals.KP = 0;
    PR_7_Vals.Ki = 10000;
    PR_7_Vals.Wr = 1e-3;
    PR_7_Vals.Wo = 50 * 7;

    /* LPF for DC sampling*/
    DCL_resetDF22(&DF_0);

    B0 = 0.754762724747215;
    B1 = -0.754762724747215;
    B2 = 0;
    A1 = -0.509525449494429;
    A2 = 0;

    DF_0.b0 = B0;
    DF_0.b1 = B1;
    DF_0.b2 = B2;

    DF_0.a1 = A1;
    DF_0.a2 = A2;

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
    DCL_resetDF22(&PR_0);
    DCL_resetDF22(&PR_3);
    DCL_resetDF22(&PR_5);
    DCL_resetDF22(&PR_7);

    computeDF22_PRcontrollerCoeff(&PR_0, PR_0_Vals.KP, PR_0_Vals.Ki,
                                  PR_0_Vals.Wo * 2 * PI_VALUE, INV_ISR_FREQ,
                                  PR_0_Vals.Wr * 2 * PI_VALUE);

    computeDF22_PRcontrollerCoeff(&PR_3, PR_3_Vals.KP, PR_3_Vals.Ki,
                                  PR_3_Vals.Wo * 2 * PI_VALUE, INV_ISR_FREQ,
                                  PR_3_Vals.Wr * 2 * PI_VALUE);

    computeDF22_PRcontrollerCoeff(&PR_5, PR_5_Vals.KP, PR_5_Vals.Ki,
                                  PR_5_Vals.Wo * 2 * PI_VALUE, INV_ISR_FREQ,
                                  PR_5_Vals.Wr * 2 * PI_VALUE);

    computeDF22_PRcontrollerCoeff(&PR_7, PR_7_Vals.KP, PR_7_Vals.Ki,
                                  PR_7_Vals.Wo * 2 * PI_VALUE, INV_ISR_FREQ,
                                  PR_7_Vals.Wr * 2 * PI_VALUE);

    DCL_resetPI(&CHGR_PI);
    CHGR_PI.Imax = 0.9;
    CHGR_PI.Imin = 0;
    CHGR_PI.Kp = 1;
    CHGR_PI.Ki = 0;

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
    GPIO_setDirectionMode(INV_MAINS_RELAY_SWITCH, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(INV_MAINS_RELAY_SWITCH, GPIO_PIN_TYPE_STD);

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


    //================================================================
    while (1)
    {
        _GET_SWITCH_VAL(&SW_PRESSED_VAL);
        if (SW_PRESSED_VAL == SW_ONOFF)
        {
            if (FLG.ON_OFF == 1)
                FLG.ON_OFF = 0;
            else
                FLG.ON_OFF = 1;
        }
        lcd_UI(SW_PRESSED_VAL);
        lcdDrive_updateLCD();
        if(countLCD>=500){
            countLCD=0;
        }
        countLCD++;
    }
}
/*
 * ###################################################################################
 * ###################################################################################
 */

//TODO Inverter ISR Code
interrupt void inverterISR(void)
{

    TEMP_computeDF22_PRcontrollerCoeff(&PR_0, PR_0_Vals.KP, PR_0_Vals.Ki,
                                       PR_0_Vals.Wo * 2 * PI_VALUE,
                                       INV_ISR_FREQ,
                                       PR_0_Vals.Wr * 2 * PI_VALUE);

    TEMP_computeDF22_PRcontrollerCoeff(&PR_3, PR_3_Vals.KP, PR_3_Vals.Ki,
                                       PR_3_Vals.Wo * 2 * PI_VALUE,
                                       INV_ISR_FREQ,
                                       PR_3_Vals.Wr * 2 * PI_VALUE);

    TEMP_computeDF22_PRcontrollerCoeff(&PR_5, PR_5_Vals.KP, PR_5_Vals.Ki,
                                       PR_5_Vals.Wo * 2 * PI_VALUE,
                                       INV_ISR_FREQ,
                                       PR_5_Vals.Wr * 2 * PI_VALUE);

    TEMP_computeDF22_PRcontrollerCoeff(&PR_7, PR_7_Vals.KP, PR_7_Vals.Ki,
                                       PR_7_Vals.Wo * 2 * PI_VALUE,
                                       INV_ISR_FREQ,
                                       PR_7_Vals.Wr * 2 * PI_VALUE);

    if (resetPR == 1)
    {
        resetPR = 0;
        DCL_resetDF22(&PR_0);
        DCL_resetDF22(&PR_3);
        DCL_resetDF22(&PR_5);
        DCL_resetDF22(&PR_7);
        DCL_resetDF22(&DF_0);
    }

    clearInterruptEPWM();

}    // MainISR Ends Here

/*---------------------------------------------------------------------------------------------------*/

interrupt void adcISR(void)
{

    /*---------------X ISR CODE X-----------------*/
    /*
     * reading and processing adc values
     */
    /*
     * TODO:
     * read batt voltage: if it went down low, restart inverter when it comes back. else turn off inverter
     * high load trip: if output current> threshold, trip pwm.
     */
    readADCValues(&invInst_V, &mainsInst_V, &invLoad_I, &battBus_V,
                  &battChrg_I);

    //assigning values
    obj_invInst_V.valInst = invInst_V;
    obj_invLoad_I.valInst = invLoad_I;

    obj_mains_chgr_V_I.v = mainsInst_V;
    obj_mains_chgr_V_I.i = battChrg_I;

    battBus_V = FILTER_SAMPLE(battBus_V * INVVbus_SCALEFACTOR);

    //calculating rms-avg values
    SINE_MEASUREMENT_computeValues(&obj_invInst_V); //measuring rms-avg of generated voltage - only during inv_mode
    SINE_MEASUREMENT_computeValues(&obj_invLoad_I); //measuring rms-avg of generated current - only during inv_mode
    POWER_MEAS_SINE_ANALYZER_run(&obj_mains_chgr_V_I); //measuring rms-avg of mains-charging voltage and current

    /*the flags*/
//    updateFlags();

    /*State Machine*/
    STATE_PTR=&INV_MODE_STATE;
    (*STATE_PTR)();

    writeToGraph(obj_invInst_V.refSignalScaledUP, 0, 0);

    clearInterruptADC();
    /*---------------X ISR ENDS HERE X-----------------*/

}
/*
 * ###################################################################################
 * ###################################################################################
 */
//----------------------------------------------------------------------
/*
 * STATE MACHINE
 * ->HIGH PRIORITY TASKS
 */

/*
 * TODO: implement all states;
 * now, Only 2 states + 1 intermediate state
 * startup, inv_mode + inv_mode_transition
 */

/*
 * SYSTEM OFF STATE
 */
void SYSTEM_OFF_STATE(void)
{
    GPIO_writePin(INV_MAINS_RELAY_SWITCH, 1); //inverter is connected. but output is zero
    updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, 0);
    if (FLG.ON_OFF == SYS_ON)
        STATE_PTR = &STARTUP_STATE;
}
/*
 * STARTUP STATE
 * executes when inverter is switched on
 * ->currently goes to INV_MODE_TRANSITION_STATE
 */
void STARTUP_STATE(void)
{
    /* TURN OFF RELAY */
    GPIO_writePin(INV_MAINS_RELAY_SWITCH, 1);
    setupInverterPWM(EPWM1_BASE, EPWM2_BASE,
    INV_PWM_PERIOD,
                     INV_DEADBAND_PWM_COUNT,
                     INV_DEADBAND_PWM_COUNT);

    updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, 0);
    obj_invInst_V.refSignalCounter = 0;

    /* NEXT STATE*/
    if (FLG.ON_OFF == SYS_OFF)
    {
        STATE_PTR = &SYSTEM_OFF_STATE;
    }
    else if (FLG.MAINS_STATE == MAINS_STATE_OK)
    {
        STATE_PTR = &MAINS_MODE_TRANSITION_STATE;
    }
    else if (FLG.BATTERY_STATE == BATT_STATE_OK)
    {
        STATE_PTR = &INV_MODE_TRANSITION_STATE;
        softStartModIndex = 0;
    }
    else
        STATE_PTR = &FAULT_MODE_STATE;

}

/*
 * INV_MODE_TRANSITION_STATE
 * contains soft start routine
 * ->currently goes to INV_MODE_STATE
 */
void INV_MODE_TRANSITION_STATE(void)
{
    /*
     * implement soft start
     */
    if (softStartModIndex == 0)
    {
        setupInverterPWM(EPWM1_BASE, EPWM2_BASE,
        INV_PWM_PERIOD,
                         INV_DEADBAND_PWM_COUNT,
                         INV_DEADBAND_PWM_COUNT);

        updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, 0);
    }

    GPIO_writePin(INV_MAINS_RELAY_SWITCH, 1);

    if (obj_invInst_V.refSignal == 0)
        softStartModIndex += 0.01;

    //NEXT STATE
    if (FLG.ON_OFF == SYS_OFF)
    {
        STATE_PTR = &SYSTEM_OFF_STATE;
    }
    else if (softStartModIndex > 1)
    {
        STATE_PTR = &INV_MODE_STATE;
        softStartModIndex = 0;
    }

    obj_invInst_V.refSignal = _sineLTable[obj_invInst_V.refSignalCounter]
            * INV_SCALINGFACTOR_UP * INV_D_SCALEFACTOR * softStartModIndex;
    obj_invLoad_I.refSignalCounter = obj_invInst_V.refSignalCounter;

    obj_invInst_V.refSignalCounter++;
    if (obj_invInst_V.refSignalCounter >= 200)
    {
        obj_invInst_V.refSignalCounter = 0;
    }

    obj_invLoad_I.refSignalCounter = obj_invInst_V.refSignalCounter;

    updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, obj_invInst_V.refSignal);

}
/*
 * INV_MODE_STATE
 * contains control routine
 */
void INV_MODE_STATE(void)
{
    /*
     * implement inv mode algorithm
     */
    GPIO_writePin(INV_MAINS_RELAY_SWITCH, 1);
    //--------------------------------------------------
    /* refsignal generation */
    obj_invInst_V.refSignal = _sineLTable[obj_invInst_V.refSignalCounter];
    obj_invInst_V.refSignalScaledUP = obj_invInst_V.refSignal
            * INV_SCALINGFACTOR_UP;

    obj_invInst_V.refSignalCounter++;
    if (obj_invInst_V.refSignalCounter >= 200)
    {
        obj_invInst_V.refSignalCounter = 0;
    }
    obj_invLoad_I.refSignalCounter = obj_invInst_V.refSignalCounter;

    //---------------------------------------------------

    /* control algorithm */
    errorInstant = (obj_invInst_V.refSignalScaledUP - obj_invInst_V.valInst);

    PR_ControlVal = (DCL_runDF22_C1(&PR_0, errorInstant)
            + DCL_runDF22_C1(&PR_3, errorInstant)
            + DCL_runDF22_C1(&PR_5, errorInstant)
            + DCL_runDF22_C1(&PR_7, errorInstant)) * 0.25;

    if (PR_ControlVal > INV_CORRECTION_LIM)
        PR_ControlVal = INV_CORRECTION_LIM;
    else if (PR_ControlVal < -INV_CORRECTION_LIM)
        PR_ControlVal = -INV_CORRECTION_LIM;

    totalControlVal = (obj_invInst_V.refSignalScaledUP + PR_ControlVal);

    invDutyPU = totalControlVal * INV_D_SCALEFACTOR;

    updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, invDutyPU);

    /*
     * NEXT STATE
     */
    if (FLG.ON_OFF == SYS_OFF)
    {
        updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, 0);
        STATE_PTR = &SYSTEM_OFF_STATE;
    }
    else if (FLG.MAINS_STATE == MAINS_STATE_OK)
    {
        updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, 0);
        STATE_PTR = &MAINS_MODE_TRANSITION_STATE;
    }
    else if (FLG.INV_OVERLOAD == 1 || FLG.BATTERY_STATE != BATT_STATE_OK)
    {
        updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, 0);
        STATE_PTR = &FAULT_MODE_STATE;
    }
    /*
     *
     */
    SFRA_F32_collect((float*) &errorInstant, (float*) &PR_ControlVal);
}
/*
 * MAINS_MODE_TRANSITION_STATE
 */
void MAINS_MODE_TRANSITION_STATE(void)
{
    setupChargerPWM(EPWM1_BASE, EPWM2_BASE, INV_PWM_PERIOD);
    updateChargerPWM(INV_PWM1_BASE, INV_PWM2_BASE, 0);
    if (FLG.ON_OFF == SYS_OFF)
    {
        STATE_PTR = &SYSTEM_OFF_STATE;
    }
    else
    {
        STATE_PTR = &MAINS_MODE_STATE;
    }
}

/*
 * MAINS_MODE_STATE
 */
void MAINS_MODE_STATE(void)
{
    GPIO_writePin(INV_MAINS_RELAY_SWITCH, 0); //now inverter is disconnected. mains fed to h bridge

//    if (battBus_V > 0 && battBus_V < 13)
//    {
//        //todo constant current charging
//        BATT_CHGR_PWM_VAL_INST = DCL_runPI_C2(&CHGR_PI, BATT_CHGR_MAX_I,
//                                              current_Signal.valAverage);
//        updateChargerPWM(INV_PWM1_BASE, INV_PWM2_BASE, BATT_CHGR_PWM_VAL_INST);
//
//    }
//    else if (battBus_V >= 13 && battBus_V < 13.8)
//    {
//        //todo constant voltage charging at 13.8 v
//        BATT_CHGR_PWM_VAL_INST = DCL_runPI_C2(&CHGR_PI, BATT_CHGR_MAX_V,
//                                              invVbus);
//        updateChargerPWM(INV_PWM1_BASE, INV_PWM2_BASE, BATT_CHGR_PWM_VAL_INST);
//    }
//    else
//    {
//        //todo turn off charging. set charging pwm to zero       updateChargerPWM(EPWM1_BASE, EPWM2_BASE, BATT_CHGR_PWM_VAL_INST);
//        updateChargerPWM(INV_PWM1_BASE, INV_PWM2_BASE, 0);
//    }
    if (FLG.ON_OFF == SYS_OFF)
    {
        STATE_PTR = &SYSTEM_OFF_STATE;
    }
    else if (FLG.CHGR_CURRENT_OK == 0)
    {
        STATE_PTR = &FAULT_MODE_STATE;
    }
    else if (FLG.MAINS_STATE != MAINS_STATE_OK)
    {
        if (FLG.BATTERY_STATE == BATT_STATE_OK)
        {
            softStartModIndex = 0;
            STATE_PTR = &INV_MODE_TRANSITION_STATE;
        }
        else
        {
            STATE_PTR = &FAULT_MODE_STATE;
        }
    }

}

/*
 * FAULT_MODE_STATE
 */
void FAULT_MODE_STATE(void)
{
    setupInverterPWM(EPWM1_BASE, EPWM2_BASE,
    INV_PWM_PERIOD,
                     INV_DEADBAND_PWM_COUNT,
                     INV_DEADBAND_PWM_COUNT);

    GPIO_writePin(INV_MAINS_RELAY_SWITCH, 1); //inverter is connected. but output is zero
    updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, 0);
    PR_ControlVal = 0;
//    DEVICE_DELAY_US(1000000);
    static int count = 0;

    //NEXT STATE
    if (FLG.ON_OFF == SYS_OFF)
    {
        STATE_PTR = &SYSTEM_OFF_STATE;
    }
    else if ((FLG.MAINS_STATE == MAINS_STATE_OK
            || FLG.BATTERY_STATE == BATT_STATE_OK) && count == 10000)
    {
        STATE_PTR = &STARTUP_STATE;
        count = 0;
    }
    count++;
    if (count > 20000)
        count = 0;
}

//----------------------------------------------------------------------
/*
 * cutoff
 */
inline void _safetyCutoff(float32_t *val)
{
    GPIO_writePin(INV_MAINS_RELAY_SWITCH, 0);
    *(val) = 0.6;
}

void runSafetyCutoffCheck(float32_t fVal, float32_t *modIndex)
{
    /*
     * when first violation detected,
     * start globeCounter
     * increment violationCount whenever violation detected
     * when globeCounter reaches a certain value, then violationCount
     * is checked. if it is above threshold, cutoff initiated.
     */
    static uint32_t globeCounter = 0;
    static uint32_t counterONFlag = 0;
    static uint32_t safetyViolationCount = 0;

    if ((fVal >= 1.3 || fVal <= -1.3) && (counterONFlag == 0))
    {
        counterONFlag = 1;
        globeCounter = 0;
    }
    if (counterONFlag == 1)
    {
        if (fVal >= 1.3 || fVal <= -1.3)
            safetyViolationCount++;
        globeCounter++;
    }
    if (globeCounter >= 20)
    {
        if (safetyViolationCount >= 5)
        {
            _safetyCutoff(modIndex);
        }
        globeCounter = 0;
        safetyViolationCount = 0;
        counterONFlag = 0;
    }

}

//----------------------------------------------------------------------
/*
 * compute coeff
 */
//TODO computeDF22_PRcontrollerCoeff(
void computeDF22_PRcontrollerCoeff(DCL_DF22 *v, float32_t kp, float32_t ki,
                                   float32_t wo, float32_t fs, float32_t wrc)
{
    /*
     * w0: resonant freq
     * wrc: determines Q factor
     */
    float32_t temp1, temp2, wo_adjusted;
    wo_adjusted = 2.0f * fs * tanf(wo / (2.0f * fs));

    temp1 = 4.0f * fs * fs + wo_adjusted * wo_adjusted + 4.0f * fs * wrc;
    temp2 = 4.0f * ki * wrc * fs / temp1;
    v->b0 = temp2;
    v->b1 = 0;
    v->b2 = -temp2;
    v->a1 = ((-8.0f * fs * fs + 2.0f * wo_adjusted * wo_adjusted) / temp1);
    v->a2 = ((temp1 - 8.0f * fs * wrc) / temp1);
    v->x1 = 0;
    v->x2 = 0;

    if (kp != 0)
    {
        v->b0 += kp;
        v->b1 += kp * v->a1;
        v->b2 += kp * v->a2;
    }

    v->a1 = (v->a1);
    v->a2 = (v->a2);
}
//----------------------------------------------------------------------
/*
 * compute Measurement Values
 */
void SINE_MEASUREMENT_computeValues(SINE_MEASURMENT *measObj)
{
    if (measObj->refSignalCounter == 0)
    {
//        GPIO_writePin(LED_PIN_ZCROSS_DETECTION, 1);
        /*
         * positive cycle detected
         *  halfCycleFlag=1
         * voltAverage calculates average voltage of previous -ve cycle
         */
        measObj->halfCycleFlag = 1;

        measObj->valAvgPrevPrev = measObj->valAvgPrev;
        measObj->valAvgPrev = fabs(measObj->valSumNHalf / measObj->sampleCount);
        measObj->valAverage = (measObj->valAvgPrev + measObj->valAvgPrevPrev)
                / 2.0f;
        measObj->valRMS = measObj->valAverage * 1.11f;

        measObj->sampleCount = 0;
        measObj->valSumPHalf = 0;
    }
    else if (measObj->refSignalCounter == 100)
    {
//        GPIO_writePin(LED_PIN_ZCROSS_DETECTION, 0);
        /*
         * negative cycle detected
         *  halfCycleFlag=0
         * voltAverage calculates average voltage of previous +ve cycle
         */
        measObj->halfCycleFlag = 0;

        measObj->valAvgPrevPrev = measObj->valAvgPrev;
        measObj->valAvgPrev = fabs(measObj->valSumPHalf / measObj->sampleCount);
        measObj->valAverage = (measObj->valAvgPrev + measObj->valAvgPrevPrev)
                / 2.0f;
        measObj->valRMS = measObj->valAverage * 1.11f;

        measObj->sampleCount = 0;
        measObj->valSumNHalf = 0;
    }
    if (measObj->halfCycleFlag)
    {
        /* positive cycle */
        measObj->valSumPHalf += measObj->valInst;
        measObj->sampleCount++;
    }
    else
    {
        /* Negative cycle */
        measObj->valSumNHalf += measObj->valInst;
        measObj->sampleCount++;
    }
}
//----------------------------------------------------------------------
void TEMP_computeDF22_PRcontrollerCoeff(DCL_DF22 *v, float32_t kp, float32_t ki,
                                        float32_t wo, float32_t fs,
                                        float32_t wrc)
{
    /*
     * w0: resonant freq
     * wrc: determines Q factor
     */
    float32_t temp1, temp2, wo_adjusted;
    wo_adjusted = 2.0f * fs * tanf(wo / (2.0f * fs));

    temp1 = 4.0f * fs * fs + wo_adjusted * wo_adjusted + 4.0f * fs * wrc;
    temp2 = 4.0f * ki * wrc * fs / temp1;
    v->b0 = temp2;
    v->b1 = 0;
    v->b2 = -temp2;
    v->a1 = ((-8.0f * fs * fs + 2.0f * wo_adjusted * wo_adjusted) / temp1);
    v->a2 = ((temp1 - 8.0f * fs * wrc) / temp1);

    if (kp != 0)
    {
        v->b0 += kp;
        v->b1 += kp * v->a1;
        v->b2 += kp * v->a2;
    }

    v->a1 = (v->a1);
    v->a2 = (v->a2);
}
//----------------------------------------------------------------------
float32_t FILTER_SAMPLE(float32_t value)
{
    const int _SNO = 50;
    static float32_t samples[_SNO] = { 0 };
    static int k = 0;
    static float32_t sum = 0;

    sum = sum - samples[k] + value;
    samples[k] = value;
    k++;
    if (k >= _SNO)
        k = 0;
    return (sum / ((float32_t) _SNO));

}

float32_t FILTER_SAMPLE2(float32_t value)
{
    const int _SNO = 10;
    static float32_t samples[_SNO] = { 0 };
    static int k = 0;
    static float32_t sum = 0;

    sum = sum - samples[k] + value;
    samples[k] = value;
    k++;
    if (k >= _SNO)
        k = 0;
    return (sum / ((float32_t) _SNO));

}

//----------------------------------------------------------------------
void updateFlags()
{
    /*
     * if current state == startup mode, then V > batt_low_lim1 and
     * V<batt_high_lim1 to proceed (BATT_OK)
     *
     * if current state == normal mode, then V <batt_low_lim2 and
     * V> batt_high_lim2 to fault (BATT_HIGH or BATT_LOW)
     */
    /*checking invInst_V*/
    if (mainsInst_V > MAINS_HI_LIM)
    {
        FLG.MAINS_STATE = MAINS_HI_FAULT;
    }
    else if (mainsInst_V < MAINS_LO_LIM)
    {
        FLG.MAINS_STATE = MAINS_LO_FAULT;
    }
    else if (FLG.MAINS_STATE == MAINS_HI_FAULT
            && mainsInst_V <= (MAINS_HI_LIM - MAINS_HYST))
    {
        FLG.MAINS_STATE = MAINS_STATE_OK;
    }
    else if (FLG.MAINS_STATE == MAINS_LO_FAULT
            && mainsInst_V >= (MAINS_LO_LIM + MAINS_HYST))
    {
        FLG.MAINS_STATE = MAINS_STATE_OK;
    }
    else
        FLG.MAINS_STATE = MAINS_STATE_OK;

    /*checking battBus_V*/
    if (battBus_V > BATT_HI_LIM)
    {
        FLG.BATTERY_STATE = BATT_HI_FAULT;
    }
    else if (battBus_V < BATT_LO_LIM)
    {
        FLG.BATTERY_STATE = BATT_LO_FAULT;
    }
    else if (FLG.BATTERY_STATE == BATT_HI_FAULT
            && battBus_V <= (BATT_HI_LIM - BATT_HYST))
    {
        FLG.BATTERY_STATE = BATT_STATE_OK;
    }
    else if (FLG.BATTERY_STATE == BATT_LO_FAULT
            && battBus_V >= (BATT_LO_LIM + BATT_HYST))
    {
        FLG.BATTERY_STATE = BATT_STATE_OK;
    }
    else
        FLG.BATTERY_STATE = BATT_STATE_OK;

    /*checking invLoad_I*/
    if (invLoad_I >= INV_OVERLOAD_CURRENT
            || obj_invLoad_I.valAverage >= INV_OVERLOAD_AVG_CURRENT)
    {
        FLG.INV_OVERLOAD = 1;
    }
    else if (FLG.INV_OVERLOAD == 1 && invLoad_I < (INV_OVERLOAD_CURRENT - 5))
    {
        FLG.INV_OVERLOAD = 0;
    }
    else
        FLG.INV_OVERLOAD = 0;

    /*checking battChar_i*/
    if (battChrg_I >= CHGR_CURRENT_MAX)
    {
        FLG.CHGR_CURRENT_OK = 0;
    }
    else if (FLG.CHGR_CURRENT_OK == 0 && battChrg_I < (CHGR_CURRENT_MAX - 1))
    {
        FLG.CHGR_CURRENT_OK = 1;
    }
    else
        FLG.CHGR_CURRENT_OK = 1;

}
/*
 * LCD UPDATE
 */

void lcd_UI(uint16_t _SW_VAL)
{
    static bool lockLcd = false;
    static bool debugMode = 0;
    static uint32_t _lcdWindowcount = 0;
    static uint32_t _delayCount = 0;
    static uint32_t _lcdMemResetCount = 0;


    /* UP DOWN Navigation*/
    if(_SW_VAL==SW_DEBUG){
        debugMode=!debugMode;
        _lcdWindowcount=0;
        lockLcd=0;
        _delayCount=0;
    }
    else if(_SW_VAL==SW_ONOFF){
        lockLcd=0;
        _delayCount=0;
    }
    else if (_SW_VAL == SW_SCROLL)
    {
        lockLcd=0;
        _delayCount = 0;
        lcdDrive_initialise();
        lcdDrive_clearDisplay();
        _lcdWindowcount++;
    }
    else if (_SW_VAL == SW_LOCK)
    {
        _delayCount = 0;
        lcdDrive_initialise();
        lcdDrive_clearDisplay();
        lockLcd = !lockLcd;
    }
    else if (_delayCount == 1000)
    {
        _delayCount = 0;
        lcdDrive_initialise();
        lcdDrive_clearDisplay();
        _lcdWindowcount++;
    }

    /* Auto window change*/
    if (_delayCount >= 1000)
        _delayCount = 0;

    /* window display */
    if (_lcdMemResetCount == 10)
    {

        memset(LCD_DATA[0], 0, sizeof(LCD_DATA[0]));
        memset(LCD_DATA[1], 0, sizeof(LCD_DATA[1]));
        if (debugMode == 1)
        {
            /*TODO DEBUG_MODE WINDOW*/


            switch (_lcdWindowcount)
            {
            case 0:
                sprintf(LCD_DATA[0], "DEBUG_MODE");
                sprintf(LCD_DATA[1], "<>");
                break;
            case 1:
                parseFloatForLCD(obj_invInst_V.valRMS, &_lcdParsedVals[0]);
                sprintf(LCD_DATA[0], "<>invV: %u.%u%u%u", _lcdParsedVals[0],
                        _lcdParsedVals[1], _lcdParsedVals[2],
                        _lcdParsedVals[3]);

                parseFloatForLCD(obj_invLoad_I.valAverage, &_lcdParsedVals[0]);
                sprintf(LCD_DATA[1], "<>I_load: %u.%u%u%u", _lcdParsedVals[0],
                        _lcdParsedVals[1], _lcdParsedVals[2],
                        _lcdParsedVals[3]);
                break;
            case 2:
                parseFloatForLCD(battBus_V, &_lcdParsedVals[0]);
                sprintf(LCD_DATA[0], "<>Batt_V: %u.%u%u%u", _lcdParsedVals[0],
                        _lcdParsedVals[1], _lcdParsedVals[2],
                        _lcdParsedVals[3]);

                parseFloatForLCD(obj_mains_chgr_V_I.vRms, &_lcdParsedVals[0]);
                sprintf(LCD_DATA[1], "<>mains_V: %u.%u%u%u", _lcdParsedVals[0],
                        _lcdParsedVals[1], _lcdParsedVals[2],
                        _lcdParsedVals[3]);
                break;
            case 3:
                parseFloatForLCD(obj_invInst_V.valRMS, &_lcdParsedVals[0]);
                sprintf(LCD_DATA[0], "<>V_RMS: %u.%u%u%u", _lcdParsedVals[0],
                        _lcdParsedVals[1], _lcdParsedVals[2],
                        _lcdParsedVals[3]);

                parseFloatForLCD(obj_mains_chgr_V_I.iRms, &_lcdParsedVals[0]);
                sprintf(LCD_DATA[1], "<>Chrg_I: %u.%u%u%u", _lcdParsedVals[0],
                        _lcdParsedVals[1], _lcdParsedVals[2],
                        _lcdParsedVals[3]);
                break;

            case 4:

                sprintf(LCD_DATA[0], "<>MAINS_FG: %u", (uint16_t)FLG.MAINS_STATE);
                sprintf(LCD_DATA[1], "<>BATT_FG: %u", (uint16_t)FLG.BATTERY_STATE);

                break;

            case 5:

                sprintf(LCD_DATA[0], "<>INV_OL_FG: %u", (uint16_t)FLG.INV_OVERLOAD);
                sprintf(LCD_DATA[1], "<>CHGR_I_OK: %u", (uint16_t)FLG.CHGR_CURRENT_OK);

                break;

            case 6:

                sprintf(LCD_DATA[0], "<>>>>>><<<<<<");
                sprintf(LCD_DATA[1], "<>   :)    xx");

                break;

            }

            if (_lcdWindowcount > LCD_DEBUGMODE_WINDOW)
                _lcdWindowcount = 0;
            else if (_lcdWindowcount < 0)
                _lcdWindowcount = LCD_DEBUGMODE_WINDOW;
        }
        else if (STATE_PTR == &SYSTEM_OFF_STATE)
        {
            sprintf(LCD_DATA[0], "MY_UPS");
            sprintf(LCD_DATA[1], "OFF");



            /* ------------------------------------- */
        }
        else if (STATE_PTR == &STARTUP_STATE)
        {
            /* ------------------------------------- */
            sprintf(LCD_DATA[0], "MY_UPS");
            sprintf(LCD_DATA[1], "STARTING UP...");
            /* ------------------------------------- */
        }
        else if (STATE_PTR == &INV_MODE_TRANSITION_STATE)
        {
            /* ------------------------------------- */
            sprintf(LCD_DATA[0], "MY_UPS");
            sprintf(LCD_DATA[1], "INV MODE t");
            _lcdWindowcount = 0;
            /* ------------------------------------- */
        }
        else if (STATE_PTR == &INV_MODE_STATE)
        {
            /* ------------------------------------- */
            switch (_lcdWindowcount)
            {
            case 0:
                /* HOME*/
                sprintf(LCD_DATA[0], "MY_UPS");
                sprintf(LCD_DATA[1], "INV MODE");
                break;
            case 1:
                /* inv voltage, load_current */

                parseFloatForLCD(obj_invInst_V.valRMS, &_lcdParsedVals[0]);
                sprintf(LCD_DATA[0], "V_RMS: %u.%u%u%u", _lcdParsedVals[0],
                        _lcdParsedVals[1], _lcdParsedVals[2],
                        _lcdParsedVals[3]);

                parseFloatForLCD(obj_invLoad_I.valAverage, &_lcdParsedVals[0]);
                sprintf(LCD_DATA[1], "I_Load: %u.%u%u%u", _lcdParsedVals[0],
                        _lcdParsedVals[1], _lcdParsedVals[2],
                        _lcdParsedVals[3]);
                break;
            case 2:
                /* batt voltage, load percent*/

                parseFloatForLCD(battBus_V, &_lcdParsedVals[0]);
                sprintf(LCD_DATA[0], "Batt_V: %u.%u%u%u", _lcdParsedVals[0],
                        _lcdParsedVals[1], _lcdParsedVals[2],
                        _lcdParsedVals[3]);

                sprintf(LCD_DATA[1], "LOAD%");

                break;
            default:
                if (_lcdWindowcount < 0)
                    _lcdWindowcount = 2;
                else if (_lcdWindowcount > 2)
                    _lcdWindowcount = 0;
                break;
            }
            /* ------------------------------------- */
        }
        else if (STATE_PTR == &MAINS_MODE_TRANSITION_STATE)
        {
            /* ------------------------------------- */
            sprintf(LCD_DATA[0], "MY_UPS");
            sprintf(LCD_DATA[1], "MAINS MODE t");
            _lcdWindowcount = 0;
            /* ------------------------------------- */
        }
        else if (STATE_PTR == &MAINS_MODE_STATE)
        {
            /* ------------------------------------- */
            switch (_lcdWindowcount)
            {
            case 0:
                /* HOME*/
                sprintf(LCD_DATA[0], "MY_UPS");
                sprintf(LCD_DATA[1], "MAINS MODE");
                break;
            case 1:
                /* inv voltage, load_current */

                parseFloatForLCD(obj_mains_chgr_V_I.vRms, &_lcdParsedVals[0]);
                sprintf(LCD_DATA[0], "V_RMS: %u.%u%u%u", _lcdParsedVals[0],
                        _lcdParsedVals[1], _lcdParsedVals[2],
                        _lcdParsedVals[3]);

                parseFloatForLCD(obj_invLoad_I.valAverage, &_lcdParsedVals[0]);
                sprintf(LCD_DATA[1], "I_Load: %u.%u%u%u", _lcdParsedVals[0],
                        _lcdParsedVals[1], _lcdParsedVals[2],
                        _lcdParsedVals[3]);
                break;
            case 2:
                /* batt voltage, load percent*/

                parseFloatForLCD(battBus_V, &_lcdParsedVals[0]);
                sprintf(LCD_DATA[0], "Batt_V: %u.%u%u%u", _lcdParsedVals[0],
                        _lcdParsedVals[1], _lcdParsedVals[2],
                        _lcdParsedVals[3]);

                parseFloatForLCD(obj_mains_chgr_V_I.iRms, &_lcdParsedVals[0]);
                sprintf(LCD_DATA[0], "Chrg_I: %u.%u%u%u", _lcdParsedVals[0],
                        _lcdParsedVals[1], _lcdParsedVals[2],
                        _lcdParsedVals[3]);

                break;
            default:
                if (_lcdWindowcount < 0)
                    _lcdWindowcount = 2;
                else if (_lcdWindowcount > 2)
                    _lcdWindowcount = 0;
                break;
            }
            /* ------------------------------------- */
        }
        else if (STATE_PTR == &FAULT_MODE_STATE)
        {
            /* ------------------------------------- */
            sprintf(LCD_DATA[0], "FAULT MODE");
            sprintf(LCD_DATA[1], "-*-*-*-");
            /* ------------------------------------- */
        }
        if (lockLcd)
            LCD_DATA[0][15] = '#';

        lcdDrive_updateLCD_DATA(LCD_DATA);
    }
    /* LCD memReset*/
    if (!lockLcd)
        _delayCount++;
    _lcdMemResetCount++;
    if (_lcdMemResetCount > 10)
        _lcdMemResetCount = 0;
}

