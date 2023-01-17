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

    /*
     * TODO
     * SETTING UP CONTROL LOOP
     */
    /*
     *
     */
    //
    // ISR Mapping

    setupInterrupt();
    // Enable PWM Clocks
    enablePWMCLKCounting();
    //

    /*
     *SETUP LEDPIN at pin34
     */
    GPIO_setDirectionMode(LED_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(LED_PIN, GPIO_PIN_TYPE_STD);
    while (1)
    {
        SW_PRESSED_VAL=_GET_SWITCH_VAL();
        DEVICE_DELAY_US(50000); //10mS delay
    }
}


//===============================================================
//TODO Inverter ISR Code
interrupt void inverterISR(void)
{

    //TODO-Implement control system
    //DONE ADC
    //DONE PWM

    clearInterruptEPWM();
}    // MainISR Ends Here

//================================================================

interrupt void adcISR(void)
{
    adcRes=(float32_t)_GETRES_SOC0;

#if DEBUG==1
    /*
     * DEBUG
     */
    volatile static uint32_t kinit = 0;
    volatile static uint16_t logData = 0;
    volatile static uint16_t k =0;

    //=======================================

    if(SW_PRESSED_VAL==5){
        kinit=0;
        logData=1;
        GPIO_togglePin(LED_PIN);
        SW_PRESSED_VAL=0;
    }

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
                mainsVoltage[k]=(int16_t)adcRes;
                k++;
            }

            if (k >= SAMPLENO)
            {
                NOP;
                logData=0;
            }
        }


    }
    /*
     * END DEBUG
     */

#endif

    updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, 0.5);
//    updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, invSine);
    //TODO: CONTROL SYSTEM



    clearInterruptADC();
}

//================================================================

