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

//================================================================



#endif /* PROJECT_MAIN_H_ */
