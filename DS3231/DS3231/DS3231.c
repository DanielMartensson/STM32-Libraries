/*
 * DS3231.c
 *
 *  Created on: 6 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "DS3231.h"

// Private functions
static void setAlarmSecond(DS3231_I2C* i2c, uint8_t seconds, uint8_t address);
static void setAlarmMinutes(DS3231_I2C* i2c, uint8_t minutes, uint8_t address);
static void setAlarmHours(DS3231_I2C* i2c, uint8_t hours, uint8_t address);
static void setAlarmDay(DS3231_I2C* i2c, uint8_t days, uint8_t address);
static void setAlarmDate(DS3231_I2C* i2c, uint8_t dates, uint8_t address);



/* Create the structure
 * Address table:
 * A2, A1, A0 = 000 = 0x0
 * A2, A1, A0 = 001 = 0x1
 * A2, A1, A0 = 010 = 0x2
 * A2, A1, A0 = 011 = 0x3
 * A2, A1, A0 = 100 = 0x4
 * A2, A1, A0 = 101 = 0x5
 * A2, A1, A0 = 110 = 0x6
 * A2, A1, A0 = 111 = 0x7
*/
void DS3231_init(DS3231_I2C* i2c, I2C_HandleTypeDef* hi2c, uint8_t addr, uint8_t enableWeekAlarm, uint8_t enableDateAlarm){
	i2c->addr = 0xD0 | (addr << 1);
	i2c->hi2c = hi2c;

	// Check if we have the correct connection with our RTC
	if (HAL_I2C_IsDeviceReady(i2c->hi2c, i2c->addr, 10, 10) != HAL_OK){
	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // This MUST have GPIO PA5 ready to use - ERROR I2C - Wrong address
	    return;
	}

	// Set the control. Enable/disable the alarms and enable alarm output SQW
	uint8_t pData[2] = {0x0E, 0x1C | (enableWeekAlarm << 1) | enableDateAlarm};
	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->addr, pData, 2, 10);
	DS3231_offTriggAlarm(i2c);
}

/*
 * Set the clock.
 * Second:
 * Variable second can be 0-59.
 *
 * Minute:
 * Variable minutes can be 0-59.
 *
 * Hour:
 * Variable hours can be 0-23.
 *
 * Day:
 * Variable days can be 1-7.
 *
 * Date:
 * Variable dates can be 1-31.
 *
 * Month:
 * Variable months can be 1-12
 *
 * Year:
 * Variable years can be 0-99
 */
void DS3231_setClock(DS3231_I2C* i2c, uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t days, uint8_t dates, uint8_t months, uint8_t years){

	uint8_t pData[8];
	pData[0] = 0x00; // Address for the first register

	// Seconds
	uint8_t ten = seconds/10; // This will result 0, 1, 2, 3, 4, 5
	uint8_t part = seconds - ten*10; // This will result 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	pData[1] = (ten << 4) | part;

	// Minutes
	ten = minutes/10; // This will result 0, 1, 2, 3, 4, 5
	part = minutes - ten*10; // This will result 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	pData[2] = (ten << 4) | part;

	// Hours
	ten = hours/10; // This will result 0, 1, 2.
	part = hours - ten*10; // This will result 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	pData[3] = (ten << 4) | part;

	// Day
	pData[4] = days;

	// Date
	ten = dates/10; // This will result 0, 1, 2, 3.
	part = dates - ten*10; // This will result 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	pData[5] = (ten << 4) | part;

	// Month
	ten = months/10;
	part = months - ten*10;
	pData[6] = (ten << 4) | part;

	// Year
	ten = years/10;
	part = years - ten*10;
	pData[7] = (ten << 4) | part;

	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->addr, pData, 8, 10);

}

void DS3231_getClock(DS3231_I2C* i2c){

	uint8_t rData[7];
	HAL_I2C_Mem_Read(i2c->hi2c, i2c->addr, 0x00, 1, rData, 7, 1000);

	// Second
	i2c->second = (10*(rData[0] >> 4)) + (rData[0] & 0xF);
	i2c->minute = (10*(rData[1] >> 4)) + (rData[1] & 0xF);
	i2c->hour = (10*(rData[2] >> 4)) + (rData[2] & 0xF); // 24-hour clock
	i2c->day = rData[3];
	i2c->date = (10*(rData[4] >> 4)) + (rData[4] & 0xF);
	i2c->month = (10*(rData[5] >> 4)) + (rData[5] & 0xF);
	i2c->year = (10*(rData[6] >> 4)) + (rData[6] & 0xF);

}

// Exactly the same as DS3231_setClock arguments.
void DS3231_setDateAlarm(DS3231_I2C* i2c, uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t dates){
	setAlarmSecond(i2c, seconds, 0x07);
	setAlarmMinutes(i2c, minutes, 0x08);
	setAlarmHours(i2c, hours, 0x09);
	setAlarmDate(i2c, dates, 0xA);
}

// Exactly the same as DS3231_setClock arguments.
void DS3231_setWeekAlarm(DS3231_I2C* i2c, uint8_t minutes, uint8_t hours, uint8_t days){
	setAlarmMinutes(i2c, minutes, 0xB);
	setAlarmHours(i2c, hours, 0xC);
	setAlarmDay(i2c, days, 0xD);
}

// Un-Trigger of the alarm
void DS3231_offTriggAlarm(DS3231_I2C* i2c){
	uint8_t pData[2] = {0x0F, 0b00000000};
	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->addr, pData, 2, 10);
}

// Read the alarm status. 0b11 = Both alarms are on. 0b1 = Only date alarm is on. 0b10 = Only week alarm is on
uint8_t DS3231_readALarmStatus(DS3231_I2C* i2c){
	uint8_t rData = 0xF;
	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->addr, &rData, 1, 10);
	HAL_I2C_Master_Receive(i2c->hi2c, i2c->addr, &rData, 1, 10);
	rData = rData << 6;
	rData = rData >> 6;
	return rData; // We only want 11, 10, 01 or 00
}

static void setAlarmSecond(DS3231_I2C* i2c, uint8_t seconds, uint8_t address){
	uint8_t ten = seconds/10;
	uint8_t second = seconds - ten*10;
	uint8_t pData[2] = {address, (ten << 4) | second};
	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->addr, pData, 2, 10);
}

static void setAlarmMinutes(DS3231_I2C* i2c, uint8_t minutes, uint8_t address){
	uint8_t ten = minutes/10;
	uint8_t minute = minutes - ten*10;
	uint8_t pData[2] = {address, (ten << 4) | minute};
	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->addr, pData, 2, 10);
}

static void setAlarmHours(DS3231_I2C* i2c, uint8_t hours, uint8_t address){
	uint8_t ten = hours/10;
	uint8_t hour = hours - ten*10;
	uint8_t pData[2];
	pData[0] = address;
	pData[1] = (ten << 4) | hour;
	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->addr, pData, 2, 10);

}

static void setAlarmDay(DS3231_I2C* i2c, uint8_t days, uint8_t address){
	uint8_t ten = days/10;
	uint8_t day = days - ten*10;
	uint8_t pData[2] = {address, (1 << 6) | (ten << 4) | day};
	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->addr, pData, 2, 10);
}

static void setAlarmDate(DS3231_I2C* i2c, uint8_t dates, uint8_t address){
	uint8_t ten = dates/10;
	uint8_t date = dates - ten*10;
	uint8_t pData[2] = {address, (ten << 4) | date};
	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->addr, pData, 2, 10);
}
