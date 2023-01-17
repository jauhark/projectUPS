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
#define ADC_CHANNEL_IN           ADC_CH_ADCIN4  //ADC_A4
#define ADC_SOC_TRIG             ADC_TRIGGER_EPWM1_SOCA    //adc soc trigger by epwm interrupt

//3 button switch settings
#define ADC_SWITCH_CHANNEL_IN   ADC_CH_ADCIN6   //ADCINA6

#define INV_ISR_TRIG_PWM_BASE           EPWM1_BASE         //interrupt module base
#define INV_ISR                         INT_EPWM1          //interrupt vector number


//================================================================




#endif /* PROJECT_SETTINGS_H_ */
