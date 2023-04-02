/*
 * project_functions.c
 *
 *  Created on: 15-Dec-2022
 *      Author: jauha
 */

#include "project_functions.h"

/*------------------------------------------------------------*/
/* adc reading to adc voltage value conversion */
float32_t ADC_BASIC_SCALEFACTOR = 0.0008056640625;

/* scaling from adc voltage values to original line values */

/*------------------------------------------------------------*/

//TODO device_setup()
void setupDevice(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pull-ups.
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    // initialize CPU timers

    //
    // Initialize timer period to 100Hz
    //
    CPUTimer_setPeriod(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ / 100);
    //
    // Initialize timer period to 10Hz
    //
    CPUTimer_setPeriod(CPUTIMER1_BASE, DEVICE_SYSCLK_FREQ / 10);
    //
    // Initialize timer period to 10KHz
    //
    CPUTimer_setPeriod(CPUTIMER2_BASE, DEVICE_SYSCLK_FREQ / 10000); // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)

    CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);
    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
    CPUTimer_setPreScaler(CPUTIMER1_BASE, 0);
    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
    CPUTimer_setPreScaler(CPUTIMER2_BASE, 0);
    // Make sure timer is stopped
    CPUTimer_stopTimer(CPUTIMER0_BASE);
    CPUTimer_stopTimer(CPUTIMER1_BASE);
    CPUTimer_stopTimer(CPUTIMER2_BASE);
    CPUTimer_setEmulationMode(CPUTIMER0_BASE,
                              CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    CPUTimer_setEmulationMode(CPUTIMER1_BASE,
                              CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    CPUTimer_setEmulationMode(CPUTIMER2_BASE,
                              CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    // Reload all counter register with period value
    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);
    CPUTimer_reloadTimerCounter(CPUTIMER1_BASE);
    CPUTimer_reloadTimerCounter(CPUTIMER2_BASE);

    CPUTimer_resumeTimer(CPUTIMER0_BASE);
    CPUTimer_resumeTimer(CPUTIMER1_BASE);
    CPUTimer_resumeTimer(CPUTIMER2_BASE);
}
//----------------------------------------------------------------------
//TODO setupADC()
void setupADC(void)
{
    //Write ADC configurations and power up the ADC for both ADC A
    uint16_t i;

    //write configurations for ADC-A
    //power up the ADC

    //delay for > 1ms to allow ADC time to power up
    for (i = 0; i < 1000; i++)
    {
        asm("   RPT#255 || NOP");
    }
//    --- Reset the ADC.  This is good programming practice.
    SysCtl_resetPeripheral(SYSCTL_PERIPH_RES_ADCA);     // Reset ADC

    ADC_disableConverter(ADCA_BASE);
    ADC_setVREF(ADCA_BASE, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_2_0); // ADC clock prescaler = CPUCLK/4

    /*-------------------------------------------------*/
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_SOC_TRIG,
    ADC_CHANNEL_IN_invInst_V,
                 ACQPS_SYS_CLKS);

    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_SOC_TRIG,
    ADC_CHANNEL_IN_mainsInst_V,
                 ACQPS_SYS_CLKS);

    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER2, ADC_SOC_TRIG,
    ADC_CHANNEL_IN_invLoad_I,
                 ACQPS_SYS_CLKS);

    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER3, ADC_SOC_TRIG,
    ADC_CHANNEL_IN_battBus_V,
                 ACQPS_SYS_CLKS);

    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER4, ADC_SOC_TRIG,
    ADC_CHANNEL_IN_invLoad_I,
                 ACQPS_SYS_CLKS);

    /* SETTING UP ADC FOR SWTICH 3 BUTTON */
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER5, ADC_SOC_TRIG,
    ADC_CHANNEL_IN_clickSwitch,
                 10);
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER6, ADC_SOC_TRIG,
    ADC_CHANNEL_IN_clickSwitch,
                 10);

    /*
     *========================================
     */
    GPIO_setPadConfig(63, GPIO_PIN_TYPE_PULLUP); // disable pull up

//    ADC_enablePPBEvent(ADCA_BASE, ADC_PPB_NUMBER1, ADC_EVT_ZERO);
    ADC_enableConverter(ADCA_BASE);             // Power up the ADC

    DEVICE_DELAY_US(1000);      // Wait 1 ms after power-up before using the ADC

}
//----------------------------------------------------------------------
//TODO disablePWMCLKCounting
void disablePWMCLKCounting(void)
{
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
}
//----------------------------------------------------------------------
//TODO enablePWMCLKCounting
void enablePWMCLKCounting(void)
{
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
}
//----------------------------------------------------------------------
//TODO setupInverterPWM
void setupInverterPWM(uint32_t base1, uint32_t base2, uint16_t pwm_period_ticks,
                      uint16_t pwm_dbred_ticks, uint16_t pwm_dbfed_ticks)
{
    //
    // PWM clock on F2837x is divided by 2
    // Deadband needs to be 0.5us => 10ns*50=500ns
    // Time Base SubModule Registers
    EPWM_setPeriodLoadMode(base1, EPWM_PERIOD_SHADOW_LOAD);
    EPWM_setTimeBasePeriod(base1, pwm_period_ticks);
    EPWM_setTimeBaseCounter(base1, 0);
    EPWM_setPhaseShift(base1, 0);
    EPWM_setTimeBaseCounterMode(base1, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_setClockPrescaler(base1, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);
    EPWM_disablePhaseShiftLoad(base1);

    EPWM_setPeriodLoadMode(base2, EPWM_PERIOD_SHADOW_LOAD);
    EPWM_setTimeBasePeriod(base2, pwm_period_ticks);
    EPWM_setTimeBaseCounter(base2, 0);
    EPWM_setPhaseShift(base2, 0);
    EPWM_setTimeBaseCounterMode(base2, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_setClockPrescaler(base2, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);
    EPWM_disablePhaseShiftLoad(base2); // Disable phase shift load.

    //
    // Counter Compare Submodule Registers
    //
    EPWM_setCounterCompareValue(base1, EPWM_COUNTER_COMPARE_A,
                                pwm_period_ticks); // set duty 0% initially
    EPWM_setCounterCompareShadowLoadMode(base1, EPWM_COUNTER_COMPARE_A,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);

    EPWM_setCounterCompareValue(base2, EPWM_COUNTER_COMPARE_A,
                                pwm_period_ticks); // set duty 0% initially
    EPWM_setCounterCompareShadowLoadMode(base1, EPWM_COUNTER_COMPARE_A,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);

    //
    // Action Qualifier SubModule Registers
    //

    //
    // to start don't configure the PWM to do anything
    //
    HWREGH(base1 + EPWM_O_AQCTLA) = 0;
    HWREGH(base2 + EPWM_O_AQCTLA) = 0;

    //
    // Active high complementary PWMs - Set up the deadband
    //
    EPWM_setDeadBandCounterClock(base1, EPWM_DB_COUNTER_CLOCK_FULL_CYCLE);
    EPWM_setRisingEdgeDelayCount(base1, pwm_dbred_ticks);
    EPWM_setFallingEdgeDelayCount(base1, pwm_dbred_ticks);
    EPWM_setDeadBandDelayMode(base1, EPWM_DB_RED, true);
    EPWM_setDeadBandDelayMode(base1, EPWM_DB_FED, true);
    EPWM_setRisingEdgeDeadBandDelayInput(base1, EPWM_DB_INPUT_EPWMA);
    EPWM_setFallingEdgeDeadBandDelayInput(base1, EPWM_DB_INPUT_EPWMA);
    EPWM_setDeadBandDelayPolarity(base1, EPWM_DB_FED,
                                  EPWM_DB_POLARITY_ACTIVE_LOW);
    EPWM_setDeadBandDelayPolarity(base1, EPWM_DB_RED,
                                  EPWM_DB_POLARITY_ACTIVE_HIGH);

    EPWM_setDeadBandCounterClock(base2, EPWM_DB_COUNTER_CLOCK_FULL_CYCLE);
    EPWM_setRisingEdgeDelayCount(base2, pwm_dbred_ticks);
    EPWM_setFallingEdgeDelayCount(base2, pwm_dbred_ticks);
    EPWM_setDeadBandDelayMode(base2, EPWM_DB_RED, true);
    EPWM_setDeadBandDelayMode(base2, EPWM_DB_FED, true);
    EPWM_setRisingEdgeDeadBandDelayInput(base2, EPWM_DB_INPUT_EPWMA);
    EPWM_setFallingEdgeDeadBandDelayInput(base2, EPWM_DB_INPUT_EPWMA);
    EPWM_setDeadBandDelayPolarity(base2, EPWM_DB_FED,
                                  EPWM_DB_POLARITY_ACTIVE_LOW);
    EPWM_setDeadBandDelayPolarity(base2, EPWM_DB_RED,
                                  EPWM_DB_POLARITY_ACTIVE_HIGH);

    //
    // configure PWM 1 as master and Phase 2 as slaves and let it
    // pass the sync in pulse from PWM1
    //

    EPWM_selectPeriodLoadEvent(base1, EPWM_SHADOW_LOAD_MODE_COUNTER_ZERO);
    EPWM_setSyncOutPulseMode(base1, EPWM_SYNC_OUT_PULSE_ON_COUNTER_ZERO);

    EPWM_selectPeriodLoadEvent(base2, EPWM_SHADOW_LOAD_MODE_SYNC);
    EPWM_setPhaseShift(base2, 1);
    EPWM_enablePhaseShiftLoad(base2);
    EPWM_setCountModeAfterSync(base2, EPWM_COUNT_MODE_UP_AFTER_SYNC);

    /*ADC EVENT TRIGGER */

    EPWM_enableADCTrigger(base1, EPWM_SOC_A);
    EPWM_setADCTriggerEventPrescale(base1, EPWM_SOC_A, 1);
    EPWM_setADCTriggerSource(base1, EPWM_SOC_A, EPWM_SOC_TBCTR_ZERO);

    /*GPIO SETTINGS */

    GPIO_setDirectionMode(INV_PWM1_H_GPIO, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(INV_PWM1_H_GPIO, GPIO_PIN_TYPE_STD); // disable pull up
    GPIO_setPinConfig(INV_PWM1_H_GPIO_PIN_CONFIG);

    GPIO_setDirectionMode(INV_PWM1_L_GPIO, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(INV_PWM1_L_GPIO, GPIO_PIN_TYPE_STD); // disable pull up
    GPIO_setPinConfig(INV_PWM1_L_GPIO_PIN_CONFIG);

    GPIO_setDirectionMode(INV_PWM2_H_GPIO, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(INV_PWM2_H_GPIO, GPIO_PIN_TYPE_STD); // disable pull up
    GPIO_setPinConfig(INV_PWM2_H_GPIO_PIN_CONFIG);

    GPIO_setDirectionMode(INV_PWM2_L_GPIO, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(INV_PWM2_L_GPIO, GPIO_PIN_TYPE_STD); // disable pull up
    GPIO_setPinConfig(INV_PWM2_L_GPIO_PIN_CONFIG);

}

//----------------------------------------------------------------------
//TODO setupChargerPWM
void setupChargerPWM(uint32_t base1, uint32_t base2, uint16_t pwm_period_ticks)
{
    //
    // PWM clock on F2837x is divided by 2
    // Deadband needs to be 0.5us => 10ns*50=500ns
    // Time Base SubModule Registers
    EPWM_setPeriodLoadMode(base1, EPWM_PERIOD_SHADOW_LOAD);
    EPWM_setTimeBasePeriod(base1, pwm_period_ticks);
    EPWM_setTimeBaseCounter(base1, 0);
    EPWM_setPhaseShift(base1, 0);
    EPWM_setTimeBaseCounterMode(base1, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_setClockPrescaler(base1, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);
    EPWM_disablePhaseShiftLoad(base1);

    EPWM_setPeriodLoadMode(base2, EPWM_PERIOD_SHADOW_LOAD);
    EPWM_setTimeBasePeriod(base2, pwm_period_ticks);
    EPWM_setTimeBaseCounter(base2, 0);
    EPWM_setPhaseShift(base2, 0);
    EPWM_setTimeBaseCounterMode(base2, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_setClockPrescaler(base2, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);
    EPWM_disablePhaseShiftLoad(base2); // Disable phase shift load.

    //
    // Counter Compare Submodule Registers
    //
    EPWM_setCounterCompareValue(base1, EPWM_COUNTER_COMPARE_A,
                                pwm_period_ticks); // set duty 0% initially
    EPWM_setCounterCompareShadowLoadMode(base1, EPWM_COUNTER_COMPARE_A,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);

    EPWM_setCounterCompareValue(base2, EPWM_COUNTER_COMPARE_A,
                                pwm_period_ticks); // set duty 0% initially
    EPWM_setCounterCompareShadowLoadMode(base1, EPWM_COUNTER_COMPARE_A,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);

    //
    // Action Qualifier SubModule Registers
    //

    //
    // to start don't configure the PWM to do anything
    //
    HWREGH(base1 + EPWM_O_AQCTLA) = 0;
    HWREGH(base2 + EPWM_O_AQCTLA) = 0;

    //
    // Active high complementary PWMs - Set up the deadband
    //
    EPWM_setDeadBandCounterClock(base1, EPWM_DB_COUNTER_CLOCK_FULL_CYCLE);
    EPWM_setDeadBandDelayMode(base1, EPWM_DB_RED, false);
    EPWM_setDeadBandDelayMode(base1, EPWM_DB_FED, false);

    EPWM_setDeadBandCounterClock(base2, EPWM_DB_COUNTER_CLOCK_FULL_CYCLE);
    EPWM_setDeadBandDelayMode(base2, EPWM_DB_RED, false);
    EPWM_setDeadBandDelayMode(base2, EPWM_DB_FED, false);

    //
    // configure PWM 1 as master and Phase 2 as slaves and let it
    // pass the sync in pulse from PWM1
    //

    EPWM_selectPeriodLoadEvent(base1, EPWM_SHADOW_LOAD_MODE_COUNTER_ZERO);
    EPWM_setSyncOutPulseMode(base1, EPWM_SYNC_OUT_PULSE_ON_COUNTER_ZERO);

    EPWM_selectPeriodLoadEvent(base2, EPWM_SHADOW_LOAD_MODE_SYNC);
    EPWM_setPhaseShift(base2, 1);
    EPWM_enablePhaseShiftLoad(base2);
    EPWM_setCountModeAfterSync(base2, EPWM_COUNT_MODE_UP_AFTER_SYNC);

    /*ADC EVENT TRIGGER */

    EPWM_enableADCTrigger(base1, EPWM_SOC_A);
    EPWM_setADCTriggerEventPrescale(base1, EPWM_SOC_A, 1);
    EPWM_setADCTriggerSource(base1, EPWM_SOC_A, EPWM_SOC_TBCTR_ZERO);

    /*GPIO SETTINGS */

    GPIO_setDirectionMode(INV_PWM1_H_GPIO, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(INV_PWM1_H_GPIO, GPIO_PIN_TYPE_STD); // disable pull up
    GPIO_setPinConfig(INV_PWM1_H_GPIO_PIN_CONFIG);

    GPIO_setDirectionMode(INV_PWM1_L_GPIO, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(INV_PWM1_L_GPIO, GPIO_PIN_TYPE_STD); // disable pull up
    GPIO_setPinConfig(INV_PWM1_L_GPIO_PIN_CONFIG);

    GPIO_setDirectionMode(INV_PWM2_H_GPIO, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(INV_PWM2_H_GPIO, GPIO_PIN_TYPE_STD); // disable pull up
    GPIO_setPinConfig(INV_PWM2_H_GPIO_PIN_CONFIG);

    GPIO_setDirectionMode(INV_PWM2_L_GPIO, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(INV_PWM2_L_GPIO, GPIO_PIN_TYPE_STD); // disable pull up
    GPIO_setPinConfig(INV_PWM2_L_GPIO_PIN_CONFIG);

}

//----------------------------------------------------------------------
/*
 * SWITCH ADC KEYBOARD
 * if return is:
 * 5: switch one
 * 10: switch two
 * 15: switch three
 * 95: longPress switch three
 */

//uint16_t SA0, SA1, temp, _sw_Stats;
//const int _SNO = 100;
//float32_t longPressBuff[_SNO] = { 0 };
//float32_t longPressSum = 0.0;
//float32_t longPressAvg = 0;
//uint32_t longPressIndex = 0;
//uint32_t delayCounter = 0;
//uint16_t countFlag = 0;
//inline void _GET_SWITCH_VAL(uint16_t *VAL)
//{
//    static uint16_t previous_State2 = 0;
//    static uint16_t previous_State3 = 0;
//    static uint16_t previous_State = 0;
//    static uint16_t current_State = 0;
//
//
//
//    *VAL=0;
//
//    SA0 = (uint16_t) _GET_SWITCH_A0;
//    SA1 = (uint16_t) _GET_SWITCH_A1;
//
//    temp = (SA0 + SA1) / 2;
//
//    //code for short press
//    if (temp >= SW_A_Val - _SWITCH_ADC_RANGE
//            && temp <= SW_A_Val + _SWITCH_ADC_RANGE)
//    {
//        current_State = 5;
//    }
//    else if (temp >= SW_B_Val - _SWITCH_ADC_RANGE
//            && temp <= SW_B_Val + _SWITCH_ADC_RANGE)
//    {
//        current_State = 10;
//    }
//    else if (temp >= SW_C_Val - _SWITCH_ADC_RANGE
//            && temp <= SW_C_Val + _SWITCH_ADC_RANGE)
//    {
//        current_State = 15;
//    }
//    else
//        current_State = 0;
//
//    if (previous_State == 0 && previous_State2 == 0 && previous_State3 == 0)
//    {
//        _sw_Stats = current_State;
//    }
//    else
//        _sw_Stats = 0;
//
//    previous_State3 = previous_State2;
//    previous_State2 = previous_State;
//    previous_State = current_State;
//
//    //code for long press
//
//    delayCounter++;
//    if (delayCounter == 5)
//    {
//        longPressSum = longPressSum - longPressBuff[longPressIndex] + temp;
//        longPressBuff[longPressIndex] = temp;
//        longPressIndex++;
//        if (longPressIndex >= _SNO)
//            longPressIndex = 0;
//
//        longPressAvg = longPressSum / ((float32_t) _SNO);
//
//        if (longPressAvg >= 4000)
//            countFlag = 1;
//
//        if (countFlag == 1)
//        {
//            if (longPressAvg >= SW_A_Val - _SWITCH_ADC_RANGE
//                    && longPressAvg <= SW_A_Val + _SWITCH_ADC_RANGE)
//            {
//                _sw_Stats = 95;
//            }
//        }
//        delayCounter = 0;
//    }
//    if (delayCounter >= 5)
//        delayCounter = 0;
//
//    *(VAL)=_sw_Stats;
//}
static uint16_t SA0, SA1, sw_temp = 0;
static uint16_t SW01_count = 0;
static uint16_t SW02_count = 0;
static uint16_t SW03_count = 0;
static uint16_t NONE_pressed_count=0;

static bool SW01_shortPressed = 0;
static bool SW02_shortPressed = 0;
static bool SW03_shortPressed = 0;

static bool SW01_longPressed = 0;
static bool SW02_longPressed = 0;
static bool SW03_longPressed = 0;

static uint16_t shortCount = 10;
static uint16_t longCount = 1000;
static uint16_t countLimit=1100;

inline void _GET_SWITCH_VAL(uint16_t *val)
{


    SA0 = (uint16_t) _GET_SWITCH_A0;
    SA1 = (uint16_t) _GET_SWITCH_A1;
    sw_temp = (SA0 + SA1) / 2;

    SW01_count++;
    SW02_count++;
    SW03_count++;

    /* checking if value between switch thresholds*/
    if (sw_temp <= (SW_01_Val - SW_tolerance)
            || sw_temp >= (SW_01_Val + SW_tolerance))
        SW01_count = 0;
    if (sw_temp <= (SW_02_Val - SW_tolerance)
            || sw_temp >= (SW_02_Val + SW_tolerance))
        SW02_count = 0;
    if (sw_temp <= (SW_03_Val - SW_tolerance)
            || sw_temp >= (SW_03_Val + SW_tolerance))
        SW03_count = 0;

    /* clearing flags if no button pressed*/
    if(sw_temp>=4000){
        NONE_pressed_count++;
    }else NONE_pressed_count=0;

    if(NONE_pressed_count>100){
        NONE_pressed_count=100;
        SW01_shortPressed=0;
        SW02_shortPressed=0;
        SW03_shortPressed=0;

        SW01_longPressed = false;
        SW02_longPressed = false;
        SW03_longPressed = false;
    }

    /* checking if count matches shortcount */
    if (SW01_count == shortCount)
    {
        SW01_shortPressed = true;
        SW01_longPressed = false;
    }
    if (SW02_count == shortCount)
    {
        SW02_shortPressed = true;
        SW02_longPressed = false;
    }
    if (SW03_count == shortCount)
    {
        SW03_shortPressed = true;
        SW03_longPressed = false;
    }

    /* checking for longpress*/
    if (SW01_count == longCount)
    {
        SW01_longPressed = true;
    }
    if (SW02_count == longCount)
    {
        SW02_longPressed = true;
    }
    if (SW03_count == longCount)
    {
        SW03_longPressed = true;
    }

    /*limiting*/
    if(SW01_count>countLimit)SW01_count=countLimit;
    if(SW02_count>countLimit)SW02_count=countLimit;
    if(SW03_count>countLimit)SW03_count=countLimit;

    /* final output value*/
    if (SW01_shortPressed == true && SW01_count == 0)
    {
        /*shortpressed and then released*/
        *val = SW01_SP;
        SW01_shortPressed = false;
    }
    else if (SW02_shortPressed == true && SW02_count == 0)
    {
        *val = SW02_SP;
        SW02_shortPressed = false;
    }
    else if (SW03_shortPressed == true && SW03_count == 0)
    {
        *val = SW03_SP;
        SW03_shortPressed = false;
    }
    else if (SW01_longPressed == true)
    {
        *val = SW01_LP;
        SW01_longPressed = false;
    }
    else if (SW02_longPressed == true)
    {
        *val = SW02_LP;
        SW02_longPressed = false;
    }
    else if (SW03_longPressed == true)
    {
        *val = SW03_LP;
        SW03_longPressed = false;
    }
    else
    {
        *val = 0;
    }

}
//----------------------------------------------------------------------
/*
 * reading ADC Values
 */
float32_t correctionValue = 1.6456;
float32_t currentSensorGain = 19.8807157;

//void readADCValues(float32_t *VoInst, float32_t *mainsInst, float32_t *LiInst,
//                   float32_t *Vbus)
//{
//    *(VoInst) = (((float32_t) _GETRES_Vo) * ADC_BASIC_SCALEFACTOR);
//
//    *(mainsInst) = (((float32_t) _GETRES_Vmains) * ADC_BASIC_SCALEFACTOR);
//
//    *(LiInst) = ((((float32_t) (_GETRES_Li1)) * ADC_BASIC_SCALEFACTOR)
//            - correctionValue) * 19.8807;
//
//    *(Vbus) = ((float32_t) _GETRES_Vbus1) * ADC_BASIC_SCALEFACTOR;
//}

void readADCValues(float32_t *_invInst_V, float32_t *_mainsInst_V,
                   float32_t *_invLoad_I, float32_t *_battBus_V,
                   float32_t *_battChrg_I)
{
    float32_t _INVVo_SCALEFACTOR = 294.1176470588;
    float32_t _INVVbus_SCALEFACTOR = 7.666666667;

    *(_invInst_V) = ((((float32_t) _GETRES_invInst_V) * ADC_BASIC_SCALEFACTOR)
            - 1.66) * _INVVo_SCALEFACTOR;

    *(_mainsInst_V) = ((((float32_t) _GETRES_mainsInst_V)
            * ADC_BASIC_SCALEFACTOR) - 1.66) * _INVVo_SCALEFACTOR;

    *(_invLoad_I) = ((((float32_t) (_GETRES_invLoad_I)) * ADC_BASIC_SCALEFACTOR)
            - correctionValue) * currentSensorGain;

    *(_battBus_V) = ((float32_t) _GETRES_battBus_V) * ADC_BASIC_SCALEFACTOR;

    *(_battChrg_I) = ((((float32_t) (_GETRES_battChrg_I))
            * ADC_BASIC_SCALEFACTOR) - correctionValue) * currentSensorGain;
}

//----------------------------------------------------------------------
/*
 * lcdUpdate
 */
void parseFloatForLCD(float32_t temp, uint16_t *parsedVal)
{
    static int i;
    i = 0;
    while (i < 4)
    {
        (*parsedVal) = (uint16_t) (temp);
        temp = temp - (float32_t) (*parsedVal);
        temp = temp * 10;
        parsedVal++;
        i++;
    }
}
//----------------------------------------------------------------------
/*
 * powerMeasurement
 */

void runPowerMeasurement(uint16_t sinIndex, float32_t Vinst, float32_t Iinst,
                         float32_t *Vrms, float32_t *Irms)
{
    static voltVal = 0;

    if (sinIndex >= 0 && sinIndex < 101)
    {
        /* during positive half cycle */
        voltVal += Vinst;

    }
    else if (sinIndex >= 101 && sinIndex < 200)
    {
        /* during negative half cycle */

    }
}

