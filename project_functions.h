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
#include "DCLF32.h"
#include "project_settings.h"



void setupDevice();
void setupADC();
void disablePWMCLKCounting();
void enablePWMCLKCounting();
void setupInverterPWM(uint32_t, uint32_t, uint16_t,
                      uint16_t, uint16_t);

interrupt void inverterISR();
interrupt void adcISR();
/*
 * ADC Read result for signal reading
 */
//#define _GETRES_SOC0 ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0)
#define _GETRES_IN_Idc ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER2);

#define _GETRES_SOC0 ADC_readPPBResult(ADCARESULT_BASE, ADC_PPB_NUMBER1)

#define _GETRES_SOC1 ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1)
//#define _GETRES_SOC2 ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER2)
//#define _GETRES_SOC3 ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER3)

#define ADC_HALFPT 4096/2

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

#define SW_A_Val  1600
#define SW_B_Val  800
#define SW_C_Val  3500

#define _GET_SWITCH_A0 ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER4)
#define _GET_SWITCH_A1 ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER5)
#define _GET_SWITCH_A2 ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER6)
#define _SWITCH_ADC_RANGE 200

inline uint16_t _GET_SWITCH_VAL();

//----------------------------------------------------------------------
//TODO updateInverterPWM()
static inline void updateInverterPWM(uint32_t base1, uint32_t base2,
                                     float32_t duty)
{
    uint16_t invDuty;
    uint16_t invDuty180;


    invDuty = ((float32_t) (INV_PWM_PERIOD / 2.0f)) * (1.0f - duty);
    invDuty180 = ((float32_t) (INV_PWM_PERIOD / 2.0f)) * (1.0f + duty);

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



//================================================================





#endif /* PROJECT_FUNCTIONS_H_ */
