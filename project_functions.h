/*
 * project_functions.h
 *
 *  Created on: 15-Dec-2022
 *      Author: jauha
 */

#ifndef PROJECT_FUNCTIONS_H_
#define PROJECT_FUNCTIONS_H_


//================================================================
#include "device.h"
#include "driverlib.h"
#include "project_settings.h"


/*
 * ADC Read result for signal reading
 */
#define PPB_OFFSET_VAL 2048

#define TRIP_HIGH_VAL 1700
#define TRIP_LOW_VAL -1700

#define _GETRES_invInst_V ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0)
#define _GETRES_mainsInst_V ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1)
#define _GETRES_invLoad_I ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER2)
#define _GETRES_battBus_V ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER3)
#define _GETRES_battChrg_I  ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER4)

#define _GET_SWITCH_A0 ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER5)
#define _GET_SWITCH_A1 ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER6)


//----------------------------------------------------------------------
/* ADC BASED SWITCH KEYBOARD
 * values S1: 1500
 *        S2: 750
 *        S3: 3500
 * one switch output:
 *        ->Toggled
 *        ->ON
 *        ->OFF
 * output:
 *        0: NONE PRESSED
 *        5: S1 pressed, others OFF
 *        10: S2 Pressed, others OFF
 *        15: S3 Pressed, others OFF
 */
#define LCD_DEBUGMODE_WINDOW 6

#define SW01_SP 5
#define SW02_SP 10
#define SW03_SP 15
#define SW01_LP 50
#define SW02_LP 55
#define SW03_LP 60


#define SW_ONOFF    SW03_SP
#define SW_SCROLL   SW02_SP
#define SW_LOCK     SW01_SP
#define SW_DEBUG    SW01_LP

#define SW_01_Val  2600
#define SW_02_Val  3550
#define SW_03_Val  3200

#define SW_tolerance 100

inline void _GET_SWITCH_VAL(uint16_t *);

void setupDevice();
void setupADC();
void disablePWMCLKCounting();
void enablePWMCLKCounting();
void setupInverterPWM(uint32_t, uint32_t, uint16_t,
                      uint16_t, uint16_t);
void setupChargerPWM(uint32_t , uint32_t, uint16_t);
void parseFloatForLCD(float32_t temp, uint16_t *parsedVal);
void readADCValues(float32_t *,float32_t *,  float32_t *, float32_t *, float32_t *);
void runPowerMeasurement(uint16_t, float32_t, float32_t, float32_t *, float32_t *);

interrupt void inverterISR();
interrupt void adcISR();
//================================================================

//----------------------------------------------------------------------
//TODO updateInverterPWM()
static inline void updateInverterPWM(uint32_t base1, uint32_t base2,
                                     float32_t duty)
{
    uint16_t invDuty;
    uint16_t invDuty180;

    invDuty = ((float32_t) (INV_PWM_PERIOD / 2.0f)) * (1.0f + duty);
    invDuty180 = ((float32_t) (INV_PWM_PERIOD / 2.0f)) * (1.0f - duty);

    if (invDuty == (EPWM_getTimeBasePeriod(base1)))
    {
        invDuty = invDuty - 1;
    }
    if (invDuty180 == (EPWM_getTimeBasePeriod(base1)))
    {
        invDuty180 = invDuty180 - 1;
    }

    EPWM_setCounterCompareValue(base1, EPWM_COUNTER_COMPARE_A, invDuty);
    EPWM_setCounterCompareValue(base2, EPWM_COUNTER_COMPARE_A, invDuty180);

    /*
     *
     */
    EPWM_setActionQualifierActionComplete(
            base1,
            EPWM_AQ_OUTPUT_A,
            EPWM_AQ_OUTPUT_LOW_ZERO | EPWM_AQ_OUTPUT_NO_CHANGE_PERIOD
                    | EPWM_AQ_OUTPUT_HIGH_UP_CMPA
                    | EPWM_AQ_OUTPUT_LOW_DOWN_CMPA);

    EPWM_setActionQualifierActionComplete(
            base2,
            EPWM_AQ_OUTPUT_A,
            EPWM_AQ_OUTPUT_LOW_ZERO | EPWM_AQ_OUTPUT_NO_CHANGE_PERIOD
                    | EPWM_AQ_OUTPUT_HIGH_UP_CMPA
                    | EPWM_AQ_OUTPUT_LOW_DOWN_CMPA);
    /*
     *
     */

}
//----------------------------------------------------------------------
//TODO chargerPWM()
static inline void updateChargerPWM(uint32_t base1, uint32_t base2,
                                     float32_t duty)
{
    uint16_t invDuty;

    invDuty = ((float32_t) (INV_PWM_PERIOD) * duty);

    EPWM_setCounterCompareValue(base1, EPWM_COUNTER_COMPARE_A, invDuty);
    EPWM_setCounterCompareValue(base2, EPWM_COUNTER_COMPARE_A, invDuty);

    /*
     *
     */
    EPWM_setActionQualifierActionComplete(
            base1,
            EPWM_AQ_OUTPUT_A,
            EPWM_AQ_OUTPUT_LOW_ZERO | EPWM_AQ_OUTPUT_NO_CHANGE_PERIOD
                    | EPWM_AQ_OUTPUT_HIGH_UP_CMPA
                    | EPWM_AQ_OUTPUT_LOW_DOWN_CMPA);

    EPWM_setActionQualifierActionComplete(
            base2,
            EPWM_AQ_OUTPUT_A,
            EPWM_AQ_OUTPUT_LOW_ZERO | EPWM_AQ_OUTPUT_NO_CHANGE_PERIOD
                    | EPWM_AQ_OUTPUT_HIGH_UP_CMPA
                    | EPWM_AQ_OUTPUT_LOW_DOWN_CMPA);
    /*
     *
     */

}


//----------------------------------------------------------------------

//TODO clearInterrupt
static inline void clearInterruptEPWM(void)
{
    EPWM_clearEventTriggerInterruptFlag(INV_ISR_TRIG_PWM_BASE);
    Interrupt_clearACKGroup(INV_ISR_PIE_GROUP_NO);
}

static inline void clearInterruptADC(void){
    ADC_clearInterruptOverflowStatus(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}


//----------------------------------------------------------------------
/*
 *
 */
static inline void setupInterrupt(void)
{
//PWM11 INT is used to trigger the ISR
    DINT;

// INT on PRD event PWM

    Interrupt_register(INV_ISR, &inverterISR);
    EPWM_setInterruptSource(INV_ISR_TRIG_PWM_BASE, EPWM_INT_TBCTR_PERIOD);
    EPWM_setInterruptEventCount(INV_ISR_TRIG_PWM_BASE, CNTRL_ISR_FREQ_RATIO);
    EPWM_enableInterrupt(INV_ISR_TRIG_PWM_BASE);



// INT on ADC convCplt
    Interrupt_register(INT_ADCA1, &adcISR);
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1 , ADC_SOC_NUMBER0);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);

    ADC_enableConverter(ADCA_BASE);
    Interrupt_enable(INT_ADCA1);

    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);

    EPWM_clearEventTriggerInterruptFlag(INV_ISR_TRIG_PWM_BASE);
    Interrupt_enable(INT_EPWM1);

    clearInterruptADC();
    clearInterruptEPWM();

    EALLOW;
    EINT;
// Enable Global interrupt INTM
    ERTM;
    EDIS;
// Enable Global realtime interrupt DBGM
}

//----------------------------------------------------------------------

//================================================================




#endif /* PROJECT_FUNCTIONS_H_ */
