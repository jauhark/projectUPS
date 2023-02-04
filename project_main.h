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
#include "lcdDrive.h"
#include "string.h"
#include "stdio.h"

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
/*
 * LCD DISPLAY DATA
 */
char LCD_DATA[2][17]={{"HELLOWORLD"},{"NICE2MEETU"}};
char LCD_VAL01[10]={0};
uint16_t lcdVal1p000;
uint16_t lcdVal0p100;
uint16_t lcdVal0p010;
uint16_t lcdVal0p001;

#pragma SET_DATA_SECTION()

/*
 * Sfra Variables
 */
#if SFRA_ENABLED==1
SFRA_F32 sfra1;
float32_t plantMagVect[SFRA_FREQ_LENGTH];
float32_t plantPhaseVect[SFRA_FREQ_LENGTH];
float32_t olMagVect[SFRA_FREQ_LENGTH];
float32_t olPhaseVect[SFRA_FREQ_LENGTH];
float32_t clMagVect[SFRA_FREQ_LENGTH] ;
float32_t clPhaseVect[SFRA_FREQ_LENGTH];
float32_t freqVect[SFRA_FREQ_LENGTH];

extern long FPUsinTable[];
#endif

#pragma SET_DATA_SECTION("controlVariables")

/*
 * SPWM reference generation Variables
 */
RAMPGEN rgen1;
float32_t invSine=0;
float32_t invDutyPU=0;
float32_t invModIndex=0;
float32_t INVMOD_AVG =0.7;

/*
 * CONTROL TYPE
 * -> AVERAGE CONTROL 1
 * -> INSTANTANEOUS CONTROL 0
 */

/*===============================================================*/
#if CONTROL_MODE ==INSTANT_CONTROL
/* CODE FOR INSTANTANEOUS CONTROL */
/*
 * DCL Control variables
 */

#if CONTROL_TYPE ==TYPE3_CONTROL
/* IF CONTROL MODE IS TYPE 3*/
#define _dclCoeff_B0  0.0757265
#define _dclCoeff_B1  0.0364835
#define _dclCoeff_B2  -0.0284922
#define _dclCoeff_B3  0.0037021
#define _dclCoeff_A1  -0.3472386
#define _dclCoeff_A2  -0.5571259
#define _dclCoeff_A3  -0.0956355


DCL_DF23 myCtrl = DF23_DEFAULTS;
#endif

#if CONTROL_TYPE==TYPE2_CONTROL
/* IF CONTRL MODE IS TYPE 2*/
#define _dclCoeff_B0  0.0250841
#define _dclCoeff_B1  0.0749941
#define _dclCoeff_B2  0.0747368
#define _dclCoeff_B3  0.0248267
#define _dclCoeff_A1  0.9964153
#define _dclCoeff_A2  -0.9999971
#define _dclCoeff_A3  -0.9964182

DCL_DF22 myCtrl;

#endif

#if CONTROL_TYPE==PI_CONTROL
/* IF CONTROL MODE IS PARALLEL PI */
#define _dclCoeff_Kp   -0.0195850
#define _dclCoeff_Ki    0.0392699

DCL_PI  myCtrl=PI_DEFAULTS;
#endif

#endif


#define _dclCoeff_Kp   0.5
#define _dclCoeff_Ki   0.2

DCL_PI  myCtrl=PI_DEFAULTS;


/*===============================================================*/
#if CONTROL_MODE == AVERAGE_CONTROL
float32_t ref_peakVal =0.7;
float32_t OFFSET_CONTROL_VAL=0;
float32_t OFFSET_ERROR=0;
uint16_t halfCycleFlag=0;
uint16_t sampleCount=0;
volatile float32_t voltSumPHalf=0;
volatile float32_t voltSumNHalf=0;
volatile float32_t voltRMSTotalCycle=0;
volatile float32_t voltPeakTotalCycle=0;
volatile float32_t voltAverage=0;
volatile float32_t voltAveragePrev=0;
//#define MAINSADCGAINFACTOR -0.0012
#endif

/*
 * Variables
 */

float32_t MAINSADCGAINFACTOR=-0.0012;

int16_t adcRes=0;  //stores adc read result
int16_t adcResPrev=0;
float32_t error=0;

int16_t adcRes_Iin=0;
int16_t adcRes_Iin_Buffer[SAMPLENO]={0};

float32_t voltVal=0;
float32_t voltCompVal=0;





//int16_t mainsVoltage[SAMPLENO] ={0}; //array to log adc
float32_t mainsVoltage[SAMPLENO] ={0}; //array to log adcDataData

float32_t rmsVoltage[SAMPLENO]={0};
#pragma SET_DATA_SECTION("debugBuffers1")
float32_t errorVals[SAMPLENO]={0};
#pragma SET_DATA_SECTION("debugBuffers2")
float32_t referenceVoltage[SAMPLENO]={0};
uint16_t SW_PRESSED_VAL=0;  //stores switch state value, updated every pwm cycle

int16_t sinReading[3]={0};

//================================================================



#endif /* PROJECT_MAIN_H_ */
