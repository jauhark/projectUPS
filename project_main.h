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

#define AVG_CONTROL 0


#include "project_functions.h"


#include "DCLF32.h"
#include "DCL.h"
#include "dlog_4ch.h"
#include "emavg.h"
#include "rampgen.h"
#include "spll_1ph_sogi.h"
//#include "DCLF32.h"

/*
 * SFRA CODE
 */
#include "sfra_f32.h"
#include "sfra_gui_scicomms_driverlib.h"
/*
 * END
 */



interrupt void inverterISR(void);
interrupt void adcISR(void);

/*
 * Macros
 */
#define SAMPLENO 400

#pragma SET_DATA_SECTION()
/*
 * Sfra Variables
 */
SFRA_F32 sfra1;
float32_t plantMagVect[SFRA_FREQ_LENGTH];
float32_t plantPhaseVect[SFRA_FREQ_LENGTH];
float32_t olMagVect[SFRA_FREQ_LENGTH];
float32_t olPhaseVect[SFRA_FREQ_LENGTH];
float32_t clMagVect[SFRA_FREQ_LENGTH] ;
float32_t clPhaseVect[SFRA_FREQ_LENGTH];
float32_t freqVect[SFRA_FREQ_LENGTH];

extern long FPUsinTable[];

#pragma SET_DATA_SECTION("controlVariables")

/*
 * SPWM reference generation Variables
 */
RAMPGEN rgen1;
float32_t invSine=0;
float32_t invDutyPU=0;
float32_t invModIndex=0.7;

/*
 * DCL Control variables
 */
#define _dclCoeff_B0  0.1429782
#define _dclCoeff_B1  0.1618109
#define _dclCoeff_B2  -0.1016189
#define _dclCoeff_B3  -0.1204558
#define _dclCoeff_A1  -0.2399017
#define _dclCoeff_A2  -0.8659085
#define _dclCoeff_A3  0.1058102


DCL_DF23 myCtrl = DF23_DEFAULTS;

DCL_PI myPiCtrl=PI_DEFAULTS;


/*
 * Variables
 */
int16_t adcRes=0;  //stores adc read result
int16_t adcResPrev=0;
float32_t error=0;

int16_t adcRes_Iin=0;
int16_t adcRes_Iin_Buffer[SAMPLENO]={0};

float32_t voltVal=0;
float32_t voltCompVal=0;
uint16_t halfCycleFlag=0;
uint16_t sampleCount=0;
float32_t voltSumPHalf=0;
float32_t voltSumNHalf=0;
float32_t voltRMSTotalCycle=0;
float32_t voltAverage=0;
float32_t voltAveragePrev=0;
#define MAINSADCGAINFACTOR -0.0012


//int16_t mainsVoltage[SAMPLENO] ={0}; //array to log adc
float32_t mainsVoltage[SAMPLENO] ={0}; //array to log adcDataData

float32_t rmsVoltage[SAMPLENO]={0};
#pragma SET_DATA_SECTION("debugBuffers1")
float32_t errorVals[SAMPLENO]={0};
#pragma SET_DATA_SECTION("debugBuffers2")
float32_t controlVals[SAMPLENO]={0};
uint16_t SW_PRESSED_VAL=0;  //stores switch state value, updated every pwm cycle

int16_t sinReading[3]={0};

//================================================================



#endif /* PROJECT_MAIN_H_ */
