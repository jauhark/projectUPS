/*
 * Jauhar K
 * 15-12-2022
 * PROJECT UPS
 * tms320F280049C
 */


#include "project_main.h"


//================================================================

// RAMP to generate forced angle when grid is not present
RAMPGEN rgen1;

float32_t invSine=0;
float32_t invModIndex=0.5;
float32_t invDutyPU=0;

volatile uint32_t adcRes=0;


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
    rgen1.stepAngleMax = (float32_t) (1.0 / INV_PWM_SWITCHING_FREQUENCY); //updates ramp at every pwm period

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



//TODO Inverter ISR Code
interrupt void inverterISR(void)
{
    RAMPGEN_run(&rgen1);
    //
    // Use the angle value to compute the sine value
    //
    invSine = sinf((rgen1.out) * 6.283185307f);
//    invDutyPU = SFRA_F32_inject(invSine * invModIndex);
    invDutyPU=invSine*invModIndex;


    //-------------------------------------------------------------------------
    // PWM Driver for the inverter
    //-------------------------------------------------------------------------
    updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, invDutyPU);
//    updateInverterPWM(EPWM1_BASE, EPWM2_BASE, adcRes);
    clearInterruptEPWM();
    adcRes =ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);



}    // MainISR Ends Here

//================================================================

//interrupt void adcISR(void){
//    adcRes =ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
//    clearInterruptADC();
//}

//================================================================


