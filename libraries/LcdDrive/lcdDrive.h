/*
 * lcdDrive.h
 *
 *  Created on: Oct 6, 2020
 *      Author: jauha
 */

#ifndef SRC_LCDDRIVE_H_
#define SRC_LCDDRIVE_H_
//==================================================================
#include "project_functions.h"

//=======================CONFIG======================================

#define lcd_DB7 10
#define lcd_DB6 11
#define lcd_DB5 8
#define lcd_DB4 9
#define lcd_RS  4
#define lcd_EN  5

//======================LCD Drive Functions=========================
inline void lcdDrive_writePin(uint16_t PIN, uint16_t stats){
    if(stats==1)GPIO_writePin(PIN, 1);
    else GPIO_writePin(PIN, 0);
}

void lcdDrive_initialise();
void lcdDrive_writeLCD(uint16_t, uint16_t);
//void lcdDrive_writeString(const char, uint16_t);
void lcdDrive_send4Bits(uint16_t);
void lcdDrive_clearDisplay();
void pulse_lcdEN();
//void lcdDrive_updateLCD(char (*DATA_STRING)[17]=tempString);
void lcdDrive_updateLCD();
void lcdDrive_updateLCD_DATA(char [][17]);

#endif /* SRC_LCDDRIVE_H_ */
