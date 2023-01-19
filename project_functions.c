/*
 * project_functions.c
 *
 *  Created on: 15-Dec-2022
 *      Author: jauha
 */

#include "project_functions.h"

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

    // Initialize timer period to maximum
    CPUTimer_setPeriod(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ / 1000); // 1Khz
    // Initialize timer period to maximum
    CPUTimer_setPeriod(CPUTIMER1_BASE, DEVICE_SYSCLK_FREQ / 100); // 100Hz
    // Initialize timer period to maximum
    CPUTimer_setPeriod(CPUTIMER2_BASE, DEVICE_SYSCLK_FREQ / 10); // 10Hz
    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
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

//TODO setupADC()
void setupADC(void)
{
    //Write ADC configurations and power up the ADC for both ADC A
    uint16_t i;

    //write configurations for ADC-A
//
//    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_2_0);
//
//    ASysCtl_setAnalogReferenceInternal(ASYSCTL_VREFHIA);
//
//    //power up the ADC
//    ADC_enableConverter(ADCA_BASE);
//
    //delay for > 1ms to allow ADC time to power up
    for (i = 0; i < 1000; i++)
    {
        asm("   RPT#255 || NOP");
    }
//
//    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_SOC_TRIG,
//                 ADC_CHANNEL_IN, ACQPS_SYS_CLKS);

//    --- Reset the ADC.  This is good programming practice.
    SysCtl_resetPeripheral(SYSCTL_PERIPH_RES_ADCA);     // Reset ADC

    ADC_disableConverter(ADCA_BASE);
    ADC_setVREF(ADCA_BASE, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
//    ADC_setVREF(ADCA_BASE, ADC_REFERENCE_EXTERNAL);
    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_2_0); // ADC clock prescaler = CPUCLK/4

    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_SOC_TRIG, ADC_CHANNEL_IN_Vmains,
    ACQPS_SYS_CLKS);

    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER2, ADC_SOC_TRIG, ADC_CHANNEL_IN_Idc,
    ACQPS_SYS_CLKS);
//    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER2, ADC_SOC_TRIG, ADC_CHANNEL_IN,
//    ACQPS_SYS_CLKS);
//    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER3, ADC_SOC_TRIG, ADC_CHANNEL_IN,
//    ACQPS_SYS_CLKS);
//        ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER4, ADC_SOC_TRIG, ADC_CHANNEL_IN, ACQPS_SYS_CLKS);
//
//        ADC_setBurstModeConfig(ADCA_BASE, ADC_SOC_TRIG,5);
//        ADC_enableBurstMode(ADCA_BASE);

    /*========================================*/
    /* SETTING UP ADC FOR SWTICH 3 BUTTON */
    /*
     *==========================================
     */
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER4, ADC_SOC_TRIG,
                 ADC_SWITCH_CHANNEL_IN, 10);
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER5, ADC_SOC_TRIG,
                 ADC_SWITCH_CHANNEL_IN, 10);
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER6, ADC_SOC_TRIG,
                 ADC_SWITCH_CHANNEL_IN, 10);
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER7, ADC_SOC_TRIG,
                 ADC_SWITCH_CHANNEL_IN, 10);

    /*
     *========================================
     */
    GPIO_setPadConfig(63, GPIO_PIN_TYPE_PULLUP); // disable pull up

    /*
     * SETTING UP PPB
     */
    ADC_setupPPB(ADCA_BASE, ADC_PPB_NUMBER1, ADC_SOC_NUMBER0);
//    ADC_enablePPBEvent(ADCA_BASE, ADC_PPB_NUMBER1, evtFlags);
    ADC_setPPBReferenceOffset(ADCA_BASE, ADC_PPB_NUMBER1, 2047);
    ADC_setPPBTripLimits(ADCA_BASE, ADC_PPB_NUMBER1, 1200, -1200);
//    ADC_enablePPBEvent(ADCA_BASE, ADC_PPB_NUMBER1, ADC_EVT_ZERO);
    ADC_enableConverter(ADCA_BASE);             // Power up the ADC

    DEVICE_DELAY_US(1000);      // Wait 1 ms after power-up before using the ADC

    //Write ADC configurations and power up the ADC for both ADC A

    //write configurations for ADC-A

//    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_2_0);
//
//    ASysCtl_setAnalogReferenceInternal(ASYSCTL_VREFHIA);
//
//    //power up the ADC
//    ADC_enableConverter(ADCA_BASE);
//
//    //delay for > 1ms to allow ADC time to power up
//    for(i = 0; i < 1000; i++){
//    asm("   RPT#255 || NOP");
//    }
//
//    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM1_SOCA,
//                 ADC_CH_ADCIN4, ACQPS_SYS_CLKS);

}

//TODO disablePWMCLKCounting
void disablePWMCLKCounting(void)
{
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
}

//TODO enablePWMCLKCounting
void enablePWMCLKCounting(void)
{
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
}

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

/*
 * SWITCH ADC KEYBOARD
 */
uint16_t previous_State = 0;
uint16_t current_State = 0;

inline uint16_t _GET_SWITCH_VAL()
{
    uint16_t SA0, SA1, SA2, temp, _sw_Stats;

    SA0 = (uint16_t) _GET_SWITCH_A0;
    SA1 = (uint16_t) _GET_SWITCH_A1;
    SA2 = (uint16_t) _GET_SWITCH_A2;

    temp = (SA0 + SA1 + SA2) / 3;

    if (temp >= SW_A_Val - _SWITCH_ADC_RANGE
            && temp <= SW_A_Val + _SWITCH_ADC_RANGE)
    {
        current_State = 5;
    }
    else if (temp >= SW_B_Val - _SWITCH_ADC_RANGE
            && temp <= SW_B_Val + _SWITCH_ADC_RANGE)
    {
        current_State = 10;
    }
    else if (temp >= SW_C_Val - _SWITCH_ADC_RANGE
            && temp <= SW_C_Val + _SWITCH_ADC_RANGE)
    {
        current_State = 15;
    }
    else
        current_State = 0;

    if (previous_State == 0)
    {
        _sw_Stats = current_State;
    }
    else
        _sw_Stats = 0;

    previous_State = current_State;
    return _sw_Stats;
    //if return is 6, its switch one pressed and valid
    //if return is 11, its switch two pressed and valid... and so on

}
