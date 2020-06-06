/*
 * PCA9685.c
 *
 *  Created on: Jun 6, 2020
 *      Author: Daniel Mårtensson
 */

#include "../PCA9865 PWM Servo/PCA9685.h"

static uint8_t read8(PCA9685_I2C *i2c, uint8_t addr) {
	uint8_t pData[] = { addr };
	if(HAL_I2C_Master_Transmit(i2c->hi2c, i2c->address, pData, 1, 10) != HAL_OK)
		return 0;
	if(HAL_I2C_Master_Receive(i2c->hi2c, i2c->address, pData, 1, 10) != HAL_OK)
		return 0;

	return pData[0];
}

static void write8(PCA9685_I2C *i2c, uint8_t addr, uint8_t d) {
	uint8_t pData[2] = { addr, d };
	if(HAL_I2C_Master_Transmit(i2c->hi2c, i2c->address, pData, 2, 10) != HAL_OK)
		return; // For debugging tool in STM
}

void PCA9685_setOscillatorFrequency(PCA9685_I2C *i2c, uint32_t freq) {
	i2c->oscillator_freq = freq;
}

uint32_t PCA9685_getOscillatorFrequency(PCA9685_I2C *i2c) {
	return i2c->oscillator_freq;
}

void PCA9685_writeMicroseconds(PCA9685_I2C *i2c, uint8_t num, uint16_t Microseconds) {
	float pulse = Microseconds;
	float pulselength = 1000000; // 1,000,000 us per second

	// Read prescale
	uint16_t prescale = PCA9685_readPrescale(i2c);

	// Calculate the pulse for PWM based on Equation 1 from the datasheet section
	// 7.3.5
	prescale += 1;
	pulselength *= prescale;
	pulselength /= i2c->oscillator_freq;
	pulse /= pulselength;
	PCA9685_setPWM(i2c, num, 0, pulse);
}

void PCA9685_setPin(PCA9685_I2C *i2c, uint8_t num, uint16_t val, bool invert) {
	// Clamp value between 0 and 4095 inclusive.
	if(val > 4095)
		val = 4095;
	if (invert) {
		if (val == 0) {
			// Special value for signal fully on.
			PCA9685_setPWM(i2c, num, 4096, 0);
		} else if (val == 4095) {
			// Special value for signal fully off.
			PCA9685_setPWM(i2c, num, 0, 4096);
		} else {
			PCA9685_setPWM(i2c, num, 0, 4095 - val);
		}
	} else {
		if (val == 4095) {
			// Special value for signal fully on.
			PCA9685_setPWM(i2c, num, 4096, 0);
		} else if (val == 0) {
			// Special value for signal fully off.
			PCA9685_setPWM(i2c, num, 0, 4096);
		} else {
			PCA9685_setPWM(i2c, num, 0, val);
		}
	}
}

void PCA9685_setPWM(PCA9685_I2C *i2c, uint8_t num, uint16_t on, uint16_t off) {
	uint8_t pData[5] = { PCA9685_LED0_ON_L + 4 * num, on, on >> 8, off, off >> 8 };
	if(HAL_I2C_Master_Transmit(i2c->hi2c, i2c->address, pData, 5, 10) != HAL_OK)
		return; // For debugging tool in STM
}

uint8_t PCA9685_getPWM(PCA9685_I2C *i2c, uint8_t num) {
	uint8_t Size = PCA9685_LED0_ON_L + 4 * num;
	uint8_t pData[Size];
	if(HAL_I2C_Master_Receive(i2c->hi2c, i2c->address, pData, Size, 10) != HAL_OK)
		return 0;
	return pData[0];
}

uint8_t PCA9685_readPrescale(PCA9685_I2C *i2c) {
	return read8(i2c, PCA9685_PRESCALE);
}

void PCA9685_setOutputMode(PCA9685_I2C *i2c, bool totempole) {
	uint8_t oldmode = read8(i2c, PCA9685_MODE2);
	uint8_t newmode;
	if (totempole) {
		newmode = oldmode | MODE2_OUTDRV;
	} else {
		newmode = oldmode & ~MODE2_OUTDRV;
	}
	write8(i2c, PCA9685_MODE2, newmode);
}

void PCA9685_setPWMFreq(PCA9685_I2C *i2c, float freq) {

	// Range output modulation frequency is dependant on oscillator
	if (freq < 1)
		freq = 1;
	if (freq > 3500)
		freq = 3500; // Datasheet limit is 3052=50MHz/(4*4096)

	float prescaleval = ((i2c->oscillator_freq / (freq * 4096.0)) + 0.5) - 1;
	if (prescaleval < PCA9685_PRESCALE_MIN)
		prescaleval = PCA9685_PRESCALE_MIN;
	if (prescaleval > PCA9685_PRESCALE_MAX)
		prescaleval = PCA9685_PRESCALE_MAX;
	uint8_t prescale = (uint8_t) prescaleval;

	uint8_t oldmode = read8(i2c, PCA9685_MODE1);
	uint8_t newmode = (oldmode & ~MODE1_RESTART) | MODE1_SLEEP; // sleep
	write8(i2c, PCA9685_MODE1, newmode);                          // go to sleep
	write8(i2c, PCA9685_PRESCALE, prescale); // set the prescaler
	write8(i2c, PCA9685_MODE1, oldmode);
	HAL_Delay(5);

	// This sets the MODE1 register to turn on auto increment.
	write8(i2c, PCA9685_MODE1, oldmode | MODE1_RESTART | MODE1_AI);

}

void PCA9685_setExtClk(PCA9685_I2C *i2c, uint8_t prescale) {
	uint8_t oldmode = read8(i2c, PCA9685_MODE1);
	uint8_t newmode = (oldmode & ~MODE1_RESTART) | MODE1_SLEEP; // sleep
	write8(i2c, PCA9685_MODE1, newmode); // go to sleep, turn off internal oscillator

	// This sets both the SLEEP and EXTCLK bits of the MODE1 register to switch to
	// use the external clock.
	write8(i2c, PCA9685_MODE1, (newmode |= MODE1_EXTCLK));
	write8(i2c, PCA9685_PRESCALE, prescale); // set the prescaler
	HAL_Delay(5);

	// clear the SLEEP bit to start
	write8(i2c, PCA9685_MODE1, (newmode & ~MODE1_SLEEP) | MODE1_RESTART | MODE1_AI);
}

void PCA9685_wakeup(PCA9685_I2C *i2c) {
	uint8_t sleep = read8(i2c, PCA9685_MODE1);
	uint8_t wakeup = sleep & ~MODE1_SLEEP; // set sleep bit low
	write8(i2c, PCA9685_MODE1, wakeup);
}

void PCA9685_sleep(PCA9685_I2C *i2c) {
	uint8_t awake = read8(i2c, PCA9685_MODE1);
	uint8_t sleep = awake | MODE1_SLEEP; // set sleep bit high
	write8(i2c, PCA9685_MODE1, sleep);
	HAL_Delay(5); // wait until cycle ends for sleep to be active
}

void PCA9685_reset(PCA9685_I2C *i2c) {
	write8(i2c, PCA9685_MODE1, MODE1_RESTART);
	HAL_Delay(10);
}

void PCA9685_begin(PCA9685_I2C *i2c, I2C_HandleTypeDef *hi2c, uint8_t address, uint8_t prescale) {
	i2c->hi2c = hi2c;
	i2c->address = address;
	PCA9685_reset(i2c);
	if (prescale) {
		PCA9685_setExtClk(i2c, prescale);
	} else {
		// set a default frequency
		PCA9685_setPWMFreq(i2c, 1000);
	}
	// set the default internal frequency
	PCA9685_setOscillatorFrequency(i2c, FREQUENCY_OSCILLATOR);
}
