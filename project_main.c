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
float32_t invModIndex = 0.7;
float32_t invDutyPU = 0;
float32_t refSig=0;

//volatile float32_t adcRes = 0;
volatile float32_t adcRes = 0;
/*
 *
 */


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

    RAMPGEN_reset(&rgen1);
    rgen1.freq = (float32_t) (AC_FREQ);
    rgen1.stepAngleMax = (float32_t) (2.0 / INV_PWM_SWITCHING_FREQUENCY); //updates ramp at every pwm period

    //
    // ISR Mapping

    setupInterrupt();
    // Enable PWM Clocks
    enablePWMCLKCounting();
    //

#if DEBUG==1
    /*
     * switch input for debug
     */
#define SWITCH_GPIO 14
#define LED_PIN 34

    GPIO_setDirectionMode(SWITCH_GPIO, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(SWITCH_GPIO, GPIO_PIN_TYPE_PULLUP); // disable pull up

    GPIO_setDirectionMode(LED_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(LED_PIN, GPIO_PIN_TYPE_STD); // disable pull up

    /*
     * debug ends
     */
#endif

    while (1)
    {

    }
}

#if DEBUG ==1
/*
 * DEBUG
 */
#define SAMPLENO 500
volatile uint16_t a[SAMPLENO] = { 0 };
//volatile uint32_t b[SAMPLENO] = { 0 };
volatile int16_t mainsVoltage[SAMPLENO]={0};
volatile int16_t genVoltage[SAMPLENO]={0};
volatile float32_t volts[SAMPLENO]={0};
volatile int16_t errorVoltage[SAMPLENO]={0};
volatile int16_t controlVoltage[SAMPLENO]={0};
volatile float32_t pureControlVoltage[SAMPLENO]={0};

volatile static int k = 0;
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
volatile uint16_t _SWITCH_STATS =0;

volatile float32_t prev_feedbackVolt=0;
volatile float32_t prev_controlOUT=0;
volatile float32_t err =0;
volatile float32_t prev_err=0;
volatile float32_t integral=0;
volatile float32_t controlOUT=0;
volatile float32_t feedbackVolt=0;

#define LARGE_SIG_VAL   0.7

#define MAX_DT  0.95
#define MIN_DT  -0.95

#define MAX_CT_VAL  0.25        //so total control output will stay within 0.95 and 0.1
#define MIN_CT_VAL  -0.6

#define dT   0.0001     //0.1 milli Seconds
#define fpi     1000        //1Khz bandwidth

#define Kp      0.5
#define Ki      0.1

volatile float32_t A1=Kp*((fpi*dT/2)+1);
volatile float32_t A0=Kp*((fpi*dT/2)-1);


uint16_t BT1_status=0;
uint16_t BT1_FLAG=0;

inline uint16_t checkBT1Pressed(){
    BT1_FLAG=0;
    BT1_status=(uint16_t)_GET_SWITCH_STATS_ADC;
    if(BT1_status<=1600 && BT1_status >=1400)BT1_FLAG=1;
    else BT1_FLAG=0;

    clearInterruptADC();
    return BT1_FLAG;
}


interrupt void adcISR(void)
{

//    adcRes = (uint16_t) ((_GETRES_SOC0 + _GETRES_SOC1 + _GETRES_SOC2
//            + _GETRES_SOC3) * 0.25f);
//    adcRes = (uint16_t) ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
//
    invDutyPU = invSine * invModIndex;

    refSig= (float32_t)(-1.0f*invSine*325.27f);

    adcRes=(float32_t)(((float32_t)_GETRES_SOC0)*3.2f)/(4095.0f);

    feedbackVolt=(float32_t)((adcRes-1.6f)*(1.0f/0.0032f));

    err=(refSig-feedbackVolt);
//
      controlOUT=invDutyPU;
////    controlOUT=-Kp*err;
//    if(controlOUT>0.95)controlOUT=0.95;
//    else if(controlOUT<-0.95)controlOUT=-0.95;



#if DEBUG==1
    /*
     * DEBUG
     */
    volatile static uint64_t kinit = 0;
    volatile static uint16_t reset_ = 0;

    //=======================================
    static uint16_t count = 0;


    while (checkBT1Pressed()==1)
    {
        count++;
        if (count == 10)
        {
            //TODO after switch press make switch ONE
            reset_ = 1;
            GPIO_togglePin(LED_PIN);
            kinit=0;

        }
        if (count > 10)
            count = 20;
    }
    count = 0;
    //=======================================
    if (reset_ == 1)
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
//                volts[k] = adcRes;
//                a[k]=(uint16_t)(1000.0f*adcRes);
//                mainsVoltage[k]=(int16_t)((adcRes-1.6f)*(1.0f/0.0032f));
//                genVoltage[k]=(int16_t)(-1.0f*invSine*325.27f);
                mainsVoltage[k]=(int16_t)feedbackVolt;
                genVoltage[k]=(int16_t)refSig;
                controlVoltage[k]=(int16_t)(1000.0f*controlOUT);
                pureControlVoltage[k]=controlOUT;
                errorVoltage[k]=(int16_t)err;
                k++;
            }

            if (k >= SAMPLENO)
            {
                NOP;
                reset_=0;
            }
        }


    }
    /*
     * END DEBUG
     */

#endif

    updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, controlOUT);
//    updateInverterPWM(INV_PWM1_BASE, INV_PWM2_BASE, invSine);
    //TODO: CONTROL SYSTEM



    clearInterruptADC();
}

//================================================================

