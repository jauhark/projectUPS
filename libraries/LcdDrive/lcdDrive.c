/*
 * lcdDrive.cpp
 *
 *  Created on: Oct 6, 2020
 *      Author: jauha
 */

#include "lcdDrive.h"

//extern LCD_Drive lcdPin[4], lcdEN, lcdRS;
//char tempString[2][17] = { "loading", "please wait" };

char (*DataString)[17];
//==================================================================================
void lcdDrive_initialise() {
    lcdDrive_writePin(lcd_EN, 0);
    DEVICE_DELAY_US(50000);
	lcdDrive_writeLCD(0x3, 0);
	DEVICE_DELAY_US(10000);
	lcdDrive_writeLCD(0x3, 0);
	lcdDrive_writeLCD(0x3, 0);
	lcdDrive_writeLCD(0x2, 0);
	lcdDrive_writeLCD(0x28, 0);	//function set
	lcdDrive_writeLCD(0x08, 0); //display OFF
	lcdDrive_writeLCD(0x01, 0);	//clear display
	lcdDrive_writeLCD(0x06, 0);	//entry mode set
	lcdDrive_writeLCD(0xE, 0);	//display on
}
//==================================================================================
void lcdDrive_writeLCD(uint16_t LCD_DATA, uint16_t bit_RS) {
    lcdDrive_writePin(lcd_RS, bit_RS);
	uint16_t HIGH_BITS = LCD_DATA >> 4;
	uint16_t LOW_BITS = LCD_DATA & 0xF;
	lcdDrive_send4Bits(HIGH_BITS);
	pulse_lcdEN();
	DEVICE_DELAY_US(1000);
	lcdDrive_send4Bits(LOW_BITS);
	pulse_lcdEN();
	DEVICE_DELAY_US(2000);
}
//==================================================================================
//void lcdDrive_writeString(const char DATA_STRING, uint16_t cursorLine) {
//	if (cursorLine == 1)
//		lcdDrive_writeLCD(0x80, 0);
//	else
//		lcdDrive_writeLCD(0xC0, 0);
//	lcdDrive_writeLCD(DATA_STRING, 1);
//}
//==================================================================================
void lcdDrive_updateLCD_DATA(char received_DATA[][17]) {
	//lcdDrive_clearDisplay();
	DataString=received_DATA;
	//tempStringN = received_DATA;
}
//==================================================================================
void lcdDrive_updateLCD(){
	static int i=0, j=0;

	char Data=*(*(DataString+i)+j);
	if(Data!='\0') lcdDrive_writeLCD(Data, 1);
	else lcdDrive_writeLCD(0x10,1 );
	j++;
	if(j>15){
		j=0;
		if(i==0){
			lcdDrive_writeLCD(0xC0, 0);
			i=1;
		}
		else {
			i=0;
			lcdDrive_writeLCD(0x80, 0);//starting address of lcd
		}
	}
}
//==================================================================================
void lcdDrive_send4Bits(uint16_t LCD_DATA) {	//0B00001111
	if (LCD_DATA & 0x1)
	    lcdDrive_writePin(lcd_DB4, 1);
	else
        lcdDrive_writePin(lcd_DB4, 0);
	if (LCD_DATA & 0x2)
        lcdDrive_writePin(lcd_DB5, 1);
	else
        lcdDrive_writePin(lcd_DB5, 0);
	if (LCD_DATA & 0x4)
        lcdDrive_writePin(lcd_DB6, 1);
	else
        lcdDrive_writePin(lcd_DB6, 0);
	if (LCD_DATA & 0x8)
        lcdDrive_writePin(lcd_DB7, 1);
	else
        lcdDrive_writePin(lcd_DB7, 0);
}
//==================================================================================
void lcdDrive_clearDisplay() {
	lcdDrive_writeLCD(0x01, 0);
}
//==================================================================================
void pulse_lcdEN() {
    lcdDrive_writePin(lcd_EN, 1);
    lcdDrive_writePin(lcd_EN, 1);
    lcdDrive_writePin(lcd_EN, 1);
    lcdDrive_writePin(lcd_EN, 1);
    lcdDrive_writePin(lcd_EN, 0);
}
//==================================================================================

