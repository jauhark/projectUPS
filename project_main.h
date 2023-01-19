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
RAMPGEN rgen1;
float32_t invSine=0;
float32_t invDutyPU=0;
float32_t invModIndex=0.7;

int16_t adcRes=0;  //stores adc read result
int16_t adcResPrev=0;

int16_t adcRes_Iin=0;
int16_t adcRes_Iin_Buffer[SAMPLENO]={0};

float32_t voltVal=0;
uint16_t halfCycleFlag=0;
uint16_t sampleCount=0;
float32_t voltSumPHalf=0;
float32_t voltSumNHalf=0;
float32_t voltRMSTotalCycle=0;
float32_t voltAverage=0;
float32_t voltAveragePrev=0;

//int16_t mainsVoltage[SAMPLENO] ={0}; //array to log adc
float32_t mainsVoltage[SAMPLENO] ={0}; //array to log adcDataData
float32_t rmsVoltage[SAMPLENO]={0};
uint16_t SW_PRESSED_VAL=0;  //stores switch state value, updated every pwm cycle

int16_t sinReading[3]={0};

//================================================================



#endif /* PROJECT_MAIN_H_ */
