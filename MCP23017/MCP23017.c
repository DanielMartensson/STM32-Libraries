/*
 * MCP23017.c
 *
 *  Created on: 9 févr. 2020
 *      Author: Adafruit, Rewritten by Daniel Mårtensson
 */

#include "MCP23017.h"


/**
 * Bit number associated to a give Pin
 */
static uint8_t bitForPin(MCP23017_I2C* i2c, uint8_t pin) {
	return pin % 8;
}

/**
 * Register address, port dependent, for a given PIN
 */
static uint8_t regForPin(MCP23017_I2C* i2c, uint8_t pin, uint8_t portAaddr, uint8_t portBaddr) {
	return (pin < 8) ? portAaddr : portBaddr;
}

/**
 * Reads a given register
 */
static uint8_t readRegister(MCP23017_I2C* i2c, uint8_t addr) {
	// read the current GPINTEN
	uint8_t pData = {addr};
	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->i2caddr, &pData, 1, 10);
	HAL_I2C_Master_Receive(i2c->hi2c, i2c->i2caddr, &pData, 1, 10);
	return pData;
}

/**
 * Writes a given register
 */
static void writeRegister(MCP23017_I2C* i2c, uint8_t regAddr, uint8_t regValue) {
	// Write the register
	uint8_t pData[2] = {regAddr, regValue};
	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->i2caddr, pData, 2, 10);
}

/**
 * Helper to update a single bit of an A/B register.
 * - Reads the current register value
 * - Writes the new register value
 */
static void updateRegisterBit(MCP23017_I2C* i2c, uint8_t pin, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr) {
	uint8_t regValue;
	uint8_t regAddr = regForPin(i2c, pin, portAaddr, portBaddr);
	uint8_t bit = bitForPin(i2c, pin);
	regValue = readRegister(i2c, regAddr);

	// set the value for the particular bit
	MCP23017bitWrite(regValue, bit, pValue);

	writeRegister(i2c, regAddr, regValue);
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Initializes the MCP23017 given its HW selected address, see datasheet for Address selection.
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
void MCP23017begin(MCP23017_I2C* i2c, I2C_HandleTypeDef* hi2c, uint8_t addr) {
	if (addr > 7) {
		addr = 7;
	}
	i2c->i2caddr = (MCP23017_ADDRESS << 1 | addr << 1) ; // Important to have << 1 here for STM32
	i2c->hi2c = hi2c;

	if (HAL_I2C_IsDeviceReady(i2c->hi2c, i2c->i2caddr, 10, 10) != HAL_OK)
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // This MUST have GPIO PA5 ready to use - ERROR I2C - Wrong address


	// set defaults!
	// all inputs on port A and B
	writeRegister(i2c, MCP23017_IODIRA, 0xff);
	writeRegister(i2c, MCP23017_IODIRB, 0xff);
}

/**
 * Sets the pin mode to either d = MCP23017_INPUT or MCP23017_OUTPUT
 */
void MCP23017pinMode(MCP23017_I2C* i2c, uint8_t p, uint8_t d) {
	updateRegisterBit(i2c, p, (d == MCP23017_INPUT), MCP23017_IODIRA, MCP23017_IODIRB);
}

/**
 * Reads all 16 pins (port A and B) into a single 16 bits variable.
 */
uint16_t MCP23017readGPIOAB(MCP23017_I2C* i2c) {
	uint16_t ba = 0;
	uint8_t a;

	// read the current GPIO output latches
	uint8_t pData = MCP23017_GPIOA;
	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->i2caddr, &pData, 1, 10);

	uint8_t rxData[2] = {0, 0};
	HAL_I2C_Master_Receive(i2c->hi2c, i2c->i2caddr, rxData, 2, 10);
	a = rxData[0];
	ba = rxData[1];
	ba <<= 8;
	ba |= a;

	return ba;
}

/**
 * Read a single port, A or B, and return its current 8 bit value.
 * Parameter b should be 0 for GPIOA, and 1 for GPIOB.
 */
uint8_t MCP23017readGPIO(MCP23017_I2C* i2c, uint8_t b) {

	// read the current GPIO output latches
	uint8_t pData;
	if (b == 0){
		pData = MCP23017_GPIOA;
		HAL_I2C_Master_Transmit(i2c->hi2c, i2c->i2caddr, &pData, 1, 10);
	}else {
		pData = MCP23017_GPIOB;
		HAL_I2C_Master_Transmit(i2c->hi2c, i2c->i2caddr, &pData, 1, 10);
	}

	HAL_I2C_Master_Receive(i2c->hi2c, i2c->i2caddr, &pData, 1, 10);
	return pData;
}

/**
 * Writes all the pins in one go. This method is very useful if you are implementing a multiplexed matrix and want to get a decent refresh rate.
 */
void MCP23017writeGPIOAB(MCP23017_I2C* i2c, uint16_t ba) {
	uint8_t pData[3] = {MCP23017_GPIOA, ba & 0xFF, ba >> 8};
	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->i2caddr, pData, 3, 10);
}

void MCP23017digitalWrite(MCP23017_I2C* i2c, uint8_t pin, uint8_t d) {
	uint8_t gpio;
	uint8_t bit = bitForPin(i2c, pin);

	// read the current GPIO output latches
	uint8_t regAddr = regForPin(i2c, pin, MCP23017_OLATA, MCP23017_OLATB);
	gpio = readRegister(i2c, regAddr);

	// set the pin and direction
	MCP23017bitWrite(gpio, bit, d);

	// write the new GPIO
	regAddr = regForPin(i2c, pin, MCP23017_GPIOA, MCP23017_GPIOB);
	writeRegister(i2c, regAddr, gpio);
}

void MCP23017pullUp(MCP23017_I2C* i2c, uint8_t p, uint8_t d) {
	updateRegisterBit(i2c, p, d, MCP23017_GPPUA, MCP23017_GPPUB);
}

uint8_t MCP23017digitalRead(MCP23017_I2C* i2c, uint8_t pin) {
	uint8_t bit = bitForPin(i2c, pin);
	uint8_t regAddr = regForPin(i2c, pin, MCP23017_GPIOA, MCP23017_GPIOB);
	return (readRegister(i2c, regAddr) >> bit) & 0x1;
}

/**
 * Configures the interrupt system. both port A and B are assigned the same configuration.
 * Mirroring will OR both INTA and INTB pins.
 * Opendrain will set the INT pin to value or open drain.
 * polarity will set LOW or HIGH on interrupt.
 * Default values after Power On Reset are: (false, false, LOW)
 * If you are connecting the INTA/B pin to arduino 2/3, you should configure the interupt handling as FALLING with
 * the default configuration.
 */
void MCP23017setupInterrupts(MCP23017_I2C* i2c, uint8_t mirroring, uint8_t openDrain, uint8_t polarity) {
	// configure the port A
	uint8_t ioconfValue = readRegister(i2c, MCP23017_IOCONA);
	MCP23017bitWrite(ioconfValue, 6, mirroring);
	MCP23017bitWrite(ioconfValue, 2, openDrain);
	MCP23017bitWrite(ioconfValue, 1, polarity);
	writeRegister(i2c, MCP23017_IOCONA, ioconfValue);

	// Configure the port B
	ioconfValue = readRegister(i2c, MCP23017_IOCONB);
	MCP23017bitWrite(ioconfValue, 6, mirroring);
	MCP23017bitWrite(ioconfValue, 2, openDrain);
	MCP23017bitWrite(ioconfValue, 1, polarity);
	writeRegister(i2c, MCP23017_IOCONB, ioconfValue);
}

/**
 * Set's up a pin for interrupt. uses arduino MODEs: MCP23017_CHANGE, MCP23017_FALLING, MCP23017_RISING.
 *
 * Note that the interrupt condition finishes when you read the information about the port / value
 * that caused the interrupt or you read the port itself. Check the datasheet can be confusing.
 *
 */
void MCP23017setupInterruptPin(MCP23017_I2C* i2c, uint8_t pin, uint8_t mode) {

	// set the pin interrupt control (0 means change, 1 means compare against given value);
	updateRegisterBit(i2c, pin, (mode != MCP23017_CHANGE), MCP23017_INTCONA, MCP23017_INTCONB);
	// if the mode is not CHANGE, we need to set up a default value, different value triggers interrupt

	// In a RISING interrupt the default value is 0, interrupt is triggered when the pin goes to 1.
	// In a FALLING interrupt the default value is 1, interrupt is triggered when pin goes to 0.
	updateRegisterBit(i2c, pin, (mode == MCP23017_FALLING), MCP23017_DEFVALA, MCP23017_DEFVALB);

	// enable the pin for interrupt
	updateRegisterBit(i2c, pin, MCP23017_HIGH, MCP23017_GPINTENA, MCP23017_GPINTENB);

}

uint8_t MCP23017getLastInterruptPin(MCP23017_I2C* i2c) {
	uint8_t intf;

	// try port A
	intf = readRegister(i2c, MCP23017_INTFA);
	for (int i = 0; i < 8; i++)
		if (MCP23017bitRead(intf, i))
			return i;

	// try port B
	intf = readRegister(i2c, MCP23017_INTFB);
	for (int i = 0; i < 8; i++)
		if (MCP23017bitRead(intf, i))
			return i + 8;

	return MCP23017_INT_ERR;

}
uint8_t MCP23017getLastInterruptPinValue(MCP23017_I2C* i2c) {
	uint8_t intPin = MCP23017getLastInterruptPin(i2c);
	if (intPin != MCP23017_INT_ERR) {
		uint8_t intcapreg = regForPin(i2c, intPin, MCP23017_INTCAPA, MCP23017_INTCAPB);
		uint8_t bit = bitForPin(i2c, intPin);
		return (readRegister(i2c, intcapreg) >> bit) & (0x01);
	}

	return MCP23017_INT_ERR;
}
