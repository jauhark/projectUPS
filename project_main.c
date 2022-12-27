/*
 * Jauhar K
 * 15-12-2022
 * PROJECT UPS
 * tms320F280049C
 * THIS IS THE MAIN PROJECT
 */

#include "project_main.h"

//================================================================

// RAMP to generate forced angle when grid is not present
RAMPGEN rgen1;

float32_t invSine = 0;
float32_t invModIndex = 0.3;
float32_t invDutyPU = 0;

//volatile float32_t adcRes = 0;
volatile uint16_t adcRes=0;

/*
 * ############################################################################
 */
void main(void)

{

    setupDevice();
    disablePWMCLKCounting();
//
//    /*
//     * setupInverterPWM()
//     * @params: Base1, Base2, PWM_PERIOD, deadBand_risingEdge, deadband_fallingEdge
//     */
    setupInverterPWM(EPWM1_BASE, EPWM2_BASE,
    INV_PWM_PERIOD,
                     INV_DEADBAND_PWM_COUNT,
                     INV_DEADBAND_PWM_COUNT);

    setupADC();

    RAMPGEN_reset(&rgen1);
    rgen1.freq = (float32_t) (AC_FREQ);
    rgen1.stepAngleMax = (float32_t) (2.0 / INV_PWM_SWITCHING_FREQUENCY); //updates ramp at every pwm period

    //
    // ISR Mapping

    setupInterrupt();
    // Enable PWM Clocks
    enablePWMCLKCounting();
    //

    while (1)
    {

    }
}


#if DEBUG ==1
/*
 * DEBUG
 */
#define SAMPLENO 5000
volatile uint16_t a[SAMPLENO]={0};
volatile uint32_t b[SAMPLENO]={0};
volatile static int k=0;
/*
 * END DEBUG
 */
#endif

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

interrupt void adcISR(void)
{

    invDutyPU = invSine * invModIndex;

    updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, invDutyPU);

//    adcRes = (float32_t) (_GETRES_SOC0 + _GETRES_SOC1 + _GETRES_SOC2
//            + _GETRES_SOC3) * 0.25f;
    adcRes=(uint16_t)ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);

#if DEBUG==1
    /*
     * DEBUG
     */
    static uint64_t kinit=0;
    if(kinit<10000){
        kinit++;
    }
    else {
        if(k<SAMPLENO){
            a[k]=adcRes;
//            b[k]=(int32_t)(1000.0f*invDutyPU);
            k++;
        }

        if(k>=SAMPLENO){
            NOP;
        }
    }
    /*
     * END DEBUG
     */
#endif

    clearInterruptADC();
}

//================================================================

