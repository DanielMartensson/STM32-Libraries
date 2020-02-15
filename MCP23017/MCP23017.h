/*
 * MCP23017.h
 *
 *  Created on: 9 févr. 2020
 *      Author: Adafruit, Rewritten by Daniel Mårtensson
 */

#ifndef SRC_MCP23017_MCP23017_H_
#define SRC_MCP23017_MCP23017_H_

#include "main.h"

typedef struct {
	uint8_t i2caddr;
	I2C_HandleTypeDef *hi2c;    // Handle for I2C
} MCP23017_I2C;

void MCP23017begin(MCP23017_I2C* i2c, I2C_HandleTypeDef* hi2c, uint8_t addr);


void MCP23017pinMode(MCP23017_I2C* i2c, uint8_t p, uint8_t d);
void MCP23017digitalWrite(MCP23017_I2C* i2c, uint8_t p, uint8_t d);
void MCP23017pullUp(MCP23017_I2C* i2c, uint8_t p, uint8_t d);
uint8_t MCP23017digitalRead(MCP23017_I2C* i2c, uint8_t p);

void MCP23017writeGPIOAB(MCP23017_I2C* i2c, uint16_t);
uint16_t MCP23017readGPIOAB(MCP23017_I2C* i2c);
uint8_t MCP23017readGPIO(MCP23017_I2C* i2c, uint8_t b);

void MCP23017setupInterrupts(MCP23017_I2C* i2c, uint8_t mirroring, uint8_t open, uint8_t polarity);
void MCP23017setupInterruptPin(MCP23017_I2C* i2c, uint8_t p, uint8_t mode);
uint8_t MCP23017getLastInterruptPin(MCP23017_I2C* i2c);
uint8_t MCP23017getLastInterruptPinValue(MCP23017_I2C* i2c);


#define MCP23017bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define MCP23017bitSet(value, bit) ((value) |= (1UL << (bit)))
#define MCP23017bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define MCP23017bitWrite(value, bit, bitvalue) (bitvalue ? MCP23017bitSet(value, bit) : MCP23017bitClear(value, bit))

#define MCP23017_ADDRESS 0x20

// registers
#define MCP23017_IODIRA 0x00
#define MCP23017_IPOLA 0x02
#define MCP23017_GPINTENA 0x04
#define MCP23017_DEFVALA 0x06
#define MCP23017_INTCONA 0x08
#define MCP23017_IOCONA 0x0A
#define MCP23017_GPPUA 0x0C
#define MCP23017_INTFA 0x0E
#define MCP23017_INTCAPA 0x10
#define MCP23017_GPIOA 0x12
#define MCP23017_OLATA 0x14

#define MCP23017_IODIRB 0x01
#define MCP23017_IPOLB 0x03
#define MCP23017_GPINTENB 0x05
#define MCP23017_DEFVALB 0x07
#define MCP23017_INTCONB 0x09
#define MCP23017_IOCONB 0x0B
#define MCP23017_GPPUB 0x0D
#define MCP23017_INTFB 0x0F
#define MCP23017_INTCAPB 0x11
#define MCP23017_GPIOB 0x13
#define MCP23017_OLATB 0x15

#define MCP23017_INT_ERR 255

#define MCP23017_CHANGE 0
#define MCP23017_FALLING 1
#define MCP23017_RISING 0
#define MCP23017_HIGH 1
#define MCP23017_LOW 0
#define MCP23017_INPUT 0
#define MCP23017_OUTPUT 1

#endif /* SRC_MCP23017_MCP23017_H_ */
