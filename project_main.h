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
#define SAMPLENO 400

#include "project_functions.h"

#include "lcdDrive.h"
#include "string.h"
#include "stdio.h"

#include "DCLF32.h"
#include "DCL.h"
#include "dlog_4ch.h"
#include "emavg.h"
//#include "rampgen.h"
#include "spll_1ph_sogi.h"
#include "power_meas_sine_analyzer.h"

#include "sfra_f32.h"
#include "sfra_gui_scicomms_driverlib.h"

// display driver

/*
 * SCALINGS
 */

#define INV_SCALINGFACTOR_UP 325.2591193
#define INV_D_SCALEFACTOR 0.00260873195
float32_t INVVo_SCALEFACTOR = 294.1176470588;
float32_t INVVbus_SCALEFACTOR = 7.666666667;

int INV_CORRECTION_LIM = 150;

//----------------------------------------------------------------------------
/*
 * LCD DISPLAY DATA
 */
char LCD_DATA[2][17] = { { "HELLOWORLD" }, { "NICE2MEETU" } };
char LCD_VAL01[10] = { 0 };

uint16_t _lcdParsedVals[4] = { 0 }; //values from integer part, upto 3 decimal parts
//----------------------------------------------------------------------------

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
float32_t clMagVect[SFRA_FREQ_LENGTH];
float32_t clPhaseVect[SFRA_FREQ_LENGTH];
float32_t freqVect[SFRA_FREQ_LENGTH]; //16 for storing params values for matlab extraction
//volatile float32_t sfraParams[SFRA_FREQ_LENGTH];

extern long FPUsinTable[];
#endif
//----------------------------------------------------------------------------
#define PI_CONST  3.141592653589
//----------------------------------------------------------------------------
/*
 * SPWM reference generation Variables
 */
const float32_t _sineLTable[200] = { 0, 0.031410759, 0.06279052, 0.094108313,
                                     0.125333234, 0.156434465, 0.187381315,
                                     0.218143241, 0.248689887, 0.278991106,
                                     0.309016994, 0.33873792, 0.368124553,
                                     0.397147891, 0.425779292, 0.4539905,
                                     0.481753674, 0.509041416, 0.535826795,
                                     0.562083378, 0.587785252, 0.612907054,
                                     0.63742399, 0.661311865, 0.684547106,
                                     0.707106781, 0.728968627, 0.75011107,
                                     0.770513243, 0.790155012, 0.809016994,
                                     0.827080574, 0.844327926, 0.860742027,
                                     0.87630668, 0.891006524, 0.904827052,
                                     0.917754626, 0.929776486, 0.940880769,
                                     0.951056516, 0.960293686, 0.968583161,
                                     0.975916762, 0.982287251, 0.987688341,
                                     0.992114701, 0.995561965, 0.998026728,
                                     0.99950656, 1, 0.99950656, 0.998026728,
                                     0.995561965, 0.992114701, 0.987688341,
                                     0.982287251, 0.975916762, 0.968583161,
                                     0.960293686, 0.951056516, 0.940880769,
                                     0.929776486, 0.917754626, 0.904827052,
                                     0.891006524, 0.87630668, 0.860742027,
                                     0.844327926, 0.827080574, 0.809016994,
                                     0.790155012, 0.770513243, 0.75011107,
                                     0.728968627, 0.707106781, 0.684547106,
                                     0.661311865, 0.63742399, 0.612907054,
                                     0.587785252, 0.562083378, 0.535826795,
                                     0.509041416, 0.481753674, 0.4539905,
                                     0.425779292, 0.397147891, 0.368124553,
                                     0.33873792, 0.309016994, 0.278991106,
                                     0.248689887, 0.218143241, 0.187381315,
                                     0.156434465, 0.125333234, 0.094108313,
                                     0.06279052, 0.031410759, 5.66554E-16,
                                     -0.031410759, -0.06279052, -0.094108313,
                                     -0.125333234, -0.156434465, -0.187381315,
                                     -0.218143241, -0.248689887, -0.278991106,
                                     -0.309016994, -0.33873792, -0.368124553,
                                     -0.397147891, -0.425779292, -0.4539905,
                                     -0.481753674, -0.509041416, -0.535826795,
                                     -0.562083378, -0.587785252, -0.612907054,
                                     -0.63742399, -0.661311865, -0.684547106,
                                     -0.707106781, -0.728968627, -0.75011107,
                                     -0.770513243, -0.790155012, -0.809016994,
                                     -0.827080574, -0.844327926, -0.860742027,
                                     -0.87630668, -0.891006524, -0.904827052,
                                     -0.917754626, -0.929776486, -0.940880769,
                                     -0.951056516, -0.960293686, -0.968583161,
                                     -0.975916762, -0.982287251, -0.987688341,
                                     -0.992114701, -0.995561965, -0.998026728,
                                     -0.99950656, -1, -0.99950656, -0.998026728,
                                     -0.995561965, -0.992114701, -0.987688341,
                                     -0.982287251, -0.975916762, -0.968583161,
                                     -0.960293686, -0.951056516, -0.940880769,
                                     -0.929776486, -0.917754626, -0.904827052,
                                     -0.891006524, -0.87630668, -0.860742027,
                                     -0.844327926, -0.827080574, -0.809016994,
                                     -0.790155012, -0.770513243, -0.75011107,
                                     -0.728968627, -0.707106781, -0.684547106,
                                     -0.661311865, -0.63742399, -0.612907054,
                                     -0.587785252, -0.562083378, -0.535826795,
                                     -0.509041416, -0.481753674, -0.4539905,
                                     -0.425779292, -0.397147891, -0.368124553,
                                     -0.33873792, -0.309016994, -0.278991106,
                                     -0.248689887, -0.218143241, -0.187381315,
                                     -0.156434465, -0.125333234, -0.094108313,
                                     -0.06279052, -0.031410759

};

uint16_t _sineLTCounter = 0;

float32_t invSine = 0;
float32_t invDutyPU = 0;

//----------------------------------------------------------------------------
/*
 * GLOBAL CONTROL STATUS FLAGS
 */

uint16_t SW_PRESSED_VAL = 0;

//----------------------------------------------------------------------------
/*
 * GLOBAL MEASUREMENT VALUES
 */
//----------------------------------------------------------------------------
/*
 * FLAGS
 */

/* CUTOFF LIMITS */
#define MAINS_LO_LIM 210.0f
#define MAINS_HI_LIM  250.0f
#define MAINS_HYST  10.0f

#define BATT_LO_LIM   10.0f
#define BATT_HI_LIM 14.0f
#define BATT_HYST   1.0f

/* FAULTS */
#define MAINS_HI_FAULT   1
#define MAINS_LO_FAULT   2
#define MAINS_STATE_OK   3

#define BATT_HI_FAULT   1
#define BATT_LO_FAULT   2
#define BATT_STATE_OK   3

#define SYS_OFF 0
#define SYS_ON  1

#define INV_OVERLOAD_CURRENT 40.0f
#define INV_OVERLOAD_AVG_CURRENT 20.0f
#define CHGR_CURRENT_MAX 5.0f

/* FLAGS */

struct
{
    char MAINS_STATE;
    char BATTERY_STATE; //batt_ok: 3, batt_lo: 2, batt_hi:1
    char INV_OVERLOAD;
    char CHGR_CURRENT_OK;
    char ON_OFF;
} FLG;

float32_t invInst_V = 0;
float32_t mainsInst_V = 0;
float32_t invLoad_I = 0;
float32_t battBus_V = 0;
float32_t battChrg_I = 0;

volatile typedef struct
{
    /*INPUT VALS*/
    float32_t valInst;
    float32_t refSignal;
    float32_t refSignalScaledUP;
    uint32_t refSignalCounter;
    /*OUTPUT VALS*/
    float32_t valRMS;
    float32_t valAverage;
    /*CONTROLS*/
    float32_t valAvgPrev;
    float32_t valAvgPrevPrev;
    uint16_t halfCycleFlag;
    uint16_t sampleCount;
    float32_t valSumPHalf;
    float32_t valSumNHalf;
} SINE_MEASURMENT;

SINE_MEASURMENT obj_invInst_V; //for rms/avg of generated load voltage
SINE_MEASURMENT obj_invLoad_I; //for rms/avg of generated load current

POWER_MEAS_SINE_ANALYZER obj_mains_chgr_V_I; //for rms/avg of mains voltage + charging current

inline void SINE_MEASUREMENT_reset(SINE_MEASURMENT *measObj)
{
    measObj->valInst = 0;
    measObj->refSignal = 0;
    measObj->refSignalCounter = 0;
}
void SINE_MEASUREMENT_computeValues(SINE_MEASURMENT*);
//----------------------------------------------------------------------------
/*
 * COMPNSATOR
 */
DCL_DF22 PR_0 = DF22_DEFAULTS;
DCL_DF22 PR_3 = DF22_DEFAULTS;
DCL_DF22 PR_5 = DF22_DEFAULTS;
DCL_DF22 PR_7 = DF22_DEFAULTS;

DCL_DF22 DF_0 =DF22_DEFAULTS;

DCL_DF23 DF_23_ctrl =DF23_DEFAULTS;

DCL_PI CHGR_PI =PI_DEFAULTS;
float32_t BATT_CHGR_MAX_I=4.5;
float32_t BATT_CHGR_MIN_I=0;
float32_t BATT_CHGR_MAX_V=3.8;

float32_t BATT_CHGR_PWM_VAL_INST=0;

#define PI_VALUE 3.141592653589

                typedef struct
                {
                    float32_t KP;
                    float32_t Ki;
                    float32_t Wr;
                    float32_t Wo;
                }PR_VALS;

                PR_VALS PR_0_Vals,
PR_3_Vals, PR_5_Vals, PR_7_Vals;

float32_t PR_ControlVal;
float32_t errorInstant = 0;
float32_t pr_temp = 0;
float32_t totalControlVal = 0;
float32_t controllerGain = 1;

uint16_t resetPR = 0;

float32_t B0;
float32_t B1;
float32_t B2;
float32_t B3;
float32_t A1;
float32_t A2;
float32_t A3;

//----------------------------------------------------------------------------
/*
 * debugVars
 */
#pragma SET_DATA_SECTION("debugBuffers1")

#if DEBUG==1

uint32_t _Kval = 0;
float32_t graph_1[SAMPLENO] = { 0 };
float32_t graph_2[SAMPLENO] = { 0 };

#pragma SET_DATA_SECTION("debugBuffers2")
float32_t graph_3[SAMPLENO] = { 0 };
//float32_t graph_4[SAMPLENO] = { 0 };

float32_t ival = 0;

inline void writeToGraph(float32_t _g1, float32_t _g2, float32_t _g3)
{
    static int k = 0;

    if (k < SAMPLENO)
    {
        graph_1[k] = _g1;
        graph_2[k] = _g2;
        graph_3[k] = _g3;
        k++;
    }
    else
        k = 0;
}

#endif
/*-----------------------------------------*/
/*
 * State pointers
 */

void (*STATE_PTR)(void);    //BASE STATE PTR

void SYSTEM_OFF_STATE(void);
void STARTUP_STATE(void);
void INV_MODE_TRANSITION_STATE(void);
void INV_MODE_STATE(void);
void MAINS_MODE_TRANSITION_STATE(void);
void MAINS_MODE_STATE(void);
void FAULT_MODE_STATE(void);
/*
 * for inv_mode_transition_state
 */
float32_t softStartModIndex = 0;

/*------------------------------------------*/
/*LCD_UI*/
/*------------------------------------------*/

interrupt void inverterISR(void);
interrupt void adcISR(void);
void runSafetyCutoffCheck(float32_t, float32_t*);
void computeDF22_PRcontrollerCoeff(DCL_DF22*, float32_t, float32_t, float32_t,
                                   float32_t, float32_t);
void TEMP_computeDF22_PRcontrollerCoeff(DCL_DF22*, float32_t, float32_t,
                                        float32_t, float32_t, float32_t);
float32_t FILTER_SAMPLE(float32_t);
float32_t FILTER_SAMPLE2(float32_t);
void updateFlags();
void lcd_UI();

#endif /* PROJECT_MAIN_H_ */
