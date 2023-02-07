/*
 * project_settings.h
 *
 *  Created on: 15-Dec-2022
 *      Author: jauha
 */

#ifndef PROJECT_SETTINGS_H_
#define PROJECT_SETTINGS_H_


//================================================================
//*****************************************************************************
//defines
//*****************************************************************************

/* Device Related Defines */
#define CPU_SYS_CLOCK (100*1000000)
#define PWMSYSCLOCK_FREQ (100*1000000)
#define ECAPSYSCLOCK_FREQ   (100*1000000)

/* Project Options*/
//*****************************************************************************
/* defining control mode */

/* CONTROL_MODE*/
#define AVERAGE_CONTROL 0
#define INSTANT_CONTROL 1
#define OPEN_LOOP       2

/* CONTROL_TYPE */
#define TYPE3_CONTROL   0
#define TYPE2_CONTROL 1
#define PI_CONTROL  2

/*change here*/
#define CONTROL_MODE AVERAGE_CONTROL

#if CONTROL_MODE==INSTANT_CONTROL

/*change here*/
#define CONTROL_TYPE TYPE3_CONTROL

#endif

/*control mode ends here */
//*****************************************************************************


/* Power Stage Related Values*/
#define AC_FREQ             50

#define INV_PWM_SWITCHING_FREQUENCY ((float)20*1000)       //20KHz pwm freq

#define INV_DEADBAND_US 2
#define INV_PWM_PERIOD (PWMSYSCLOCK_FREQ)/(INV_PWM_SWITCHING_FREQUENCY)
#define INV_DEADBAND_PWM_COUNT (int)((float)INV_DEADBAND_US*(float)PWMSYSCLOCK_FREQ*(float)0.000001)

#define CNTRL_ISR_FREQ_RATIO    1
#define INV_ISR_PIE_GROUP_NO    INTERRUPT_ACK_GROUP3

#define INV_PWM1_BASE               EPWM1_BASE      //epwm module base
#define INV_PWM1_H_GPIO             0               //pin number
#define INV_PWM1_H_GPIO_PIN_CONFIG  GPIO_0_EPWM1A   //alternate pin function

#define INV_PWM1_L_GPIO             1
#define INV_PWM1_L_GPIO_PIN_CONFIG  GPIO_1_EPWM1B

#define INV_PWM2_BASE               EPWM2_BASE

#define INV_PWM2_H_GPIO             2
#define INV_PWM2_H_GPIO_PIN_CONFIG  GPIO_2_EPWM2A

#define INV_PWM2_L_GPIO             3
#define INV_PWM2_L_GPIO_PIN_CONFIG  GPIO_3_EPWM2B

#define ACQPS_SYS_CLKS    50  //ADC ACQUISITION CLOCK CYCLES
#define ADC_CHANNEL_IN_Vmains          ADC_CH_ADCIN4  //ADC_A4 voltage measurement
#define ADC_CHANNEL_IN_Idc      ADC_CH_ADCIN8   //CurrentMeasuremetn
#define ADC_SOC_TRIG             ADC_TRIGGER_EPWM1_SOCA    //adc soc trigger by epwm interrupt

#define INV_ISR_TRIG_PWM_BASE           EPWM1_BASE         //interrupt module base
#define INV_ISR                         INT_EPWM1          //interrupt vector number


//3 button switch settings
#define ADC_SWITCH_CHANNEL_IN   ADC_CH_ADCIN6   //ADCINA6
#define RELAY_SWITCH    58
//LED output for indication
#define LED_PIN_SWITCH 34  //onboard led
#define LED_PIN_ZCROSS_DETECTION    39



/*
 * SFRA SETTINGS
 */
#define SFRA_ENABLED 1

#if SFRA_ENABLED==1

#define SFRA_ISR_FREQ   INV_PWM_SWITCHING_FREQUENCY/CNTRL_ISR_FREQ_RATIO
#define SFRA_FREQ_START 2
#define SFRA_FREQ_STEP_MULTIPLY (float32_t)1.047
#define SFRA_AMPLITUDE (float32_t)0.01
#define SFRA_FREQ_LENGTH 200

/* SFRA GUI SETTINGS */
#define SFRA_GUI_SCI_BASE SCIA_BASE
#define SFRA_GUI_VBUS_CLK 50000000
#define SFRA_GUI_SCI_BAUDRATE   57600
#define SFRA_GUI_SCIRX_GPIO 28
#define SFRA_GUI_SCITX_GPIO 29
#define SFRA_GUI_SCIRX_GPIO_PIN_CONFIG GPIO_28_SCIRXDA
#define SFRA_GUI_SCITX_GPIO_PIN_CONFIG GPIO_29_SCITXDA

/* LED EN FOR SFRA COMM*/
#define SFRA_GUI_LED_INDICATOR 1
#define SFRA_GUI_LED_GPIO   LED_PIN_SWITCH
#define SFRA_GUI_LED_GPIO_PIN_CONFIG GPIO_34_GPIO34

#endif
/*
 * SFRA SETTINGS END
 */



//================================================================




#endif /* PROJECT_SETTINGS_H_ */
