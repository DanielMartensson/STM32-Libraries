/*
 * DS3231.h
 *
 *  Created on: 6 mars 2020
 *      Author: Daniel Mårtensson
 */

#ifndef SRC_DS3231_DS3231_H_
#define SRC_DS3231_DS3231_H_

#include "main.h"
#include "stdbool.h"

typedef struct {
	uint8_t addr;		 /* Address of the RTC */
	I2C_HandleTypeDef *hi2c; /* Handler for the I2C */
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
}DS3231_I2C;

void DS3231_init(DS3231_I2C* i2c, I2C_HandleTypeDef* hi2c, uint8_t addr,uint8_t enableWeekAlarm, uint8_t enableDateAlarm);
void DS3231_setClock(DS3231_I2C* i2c, uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t days, uint8_t dates, uint8_t months, uint8_t years);
void DS3231_getClock(DS3231_I2C* i2c);
void DS3231_setDateAlarm(DS3231_I2C* i2c, uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t dates);
void DS3231_setWeekAlarm(DS3231_I2C* i2c, uint8_t minutes, uint8_t hours, uint8_t days);
void DS3231_offTriggAlarm(DS3231_I2C* i2c);
uint8_t DS3231_readALarmStatus(DS3231_I2C* i2c);

#endif /* SRC_DS3231_DS3231_H_ */
