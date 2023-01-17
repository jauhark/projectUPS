/*
 * project_main.h
 *
 *  Created on: 15-Dec-2022
 *      Author: jauha
 */

#ifndef PROJECT_MAIN_H_
#define PROJECT_MAIN_H_


//================================================================
//DEBUG VERSION IF DEBUG=1
#define DEBUG 1
//#define DEBUG 0


#include "project_functions.h"

#include "dlog_4ch.h"
#include "emavg.h"
#include "rampgen.h"
#include "spll_1ph_sogi.h"
#include "DCLF32.h"
//#include "DCLF32.h"



interrupt void inverterISR(void);
interrupt void adcISR(void);

/*
 * Macros
 */
#define SAMPLENO 500
/*
 * Variables
 */
uint16_t adcRes=0;  //stores adc read result
uint16_t mainsVoltage[SAMPLENO] ={0}; //array to log adcData
uint16_t SW_PRESSED_VAL=0;  //stores switch state value, updated every pwm cycle

//================================================================



#endif /* PROJECT_MAIN_H_ */
