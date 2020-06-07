/*
 * HX711.c
 *
 *  Created on: Jun 6, 2020
 *      Author: Daniel Mårtensson
 */

#include "HX711.h"

// Make shiftIn() be aware of clockspeed for
// faster CPUs like ESP32, Teensy 3.x and friends.
// See also:
// - https://github.com/bogde/HX711/issues/75
// - https://github.com/arduino/Arduino/issues/6561
// - https://community.hiveeyes.org/t/using-bogdans-canonical-hx711-library-on-the-esp32/539
static uint8_t shiftIn(HX711 *hx, uint8_t bitOrder) {
	uint8_t value = 0;
	uint8_t i;

	for (i = 0; i < 8; ++i) {
		HAL_GPIO_WritePin(hx->PD_SCK_Port, hx->PD_SCK_Pin, GPIO_PIN_SET);
		for (uint16_t i = 0; i < SMALL_DELAY; i++) {} // Small delay
		if (bitOrder == LSBFIRST)
			value |= HAL_GPIO_ReadPin(hx->DOUT_Port, hx->DOUT_Pin) << i;
		else
			value |= HAL_GPIO_ReadPin(hx->DOUT_Port, hx->DOUT_Pin) << (7 - i);
		HAL_GPIO_WritePin(hx->PD_SCK_Port, hx->PD_SCK_Pin, GPIO_PIN_RESET);
		for (uint16_t i = 0; i < SMALL_DELAY; i++) {} // Small delay
	}
	return value;
}

void HX711_begin(HX711 *hx, GPIO_TypeDef* PD_SCK_Port, uint16_t PD_SCK_Pin, GPIO_TypeDef* DOUT_Port, uint16_t DOUT_Pin, uint8_t gain) {
	// Digital output
	hx->PD_SCK_Port = PD_SCK_Port;
	hx->PD_SCK_Pin = PD_SCK_Pin;
	// Digital pull-up input
	hx->DOUT_Port = DOUT_Port;
	hx->DOUT_Pin = DOUT_Pin;
	// Set gain
	HX711_set_gain(hx, gain);
}

bool HX711_is_ready(HX711 *hx) {
	return !HAL_GPIO_ReadPin(hx->DOUT_Port, hx->DOUT_Pin);
}

void HX711_set_gain(HX711 *hx, uint8_t gain) {
	switch (gain) {
	case 128:		// channel A, gain factor 128
		hx->GAIN = 1;
		break;
	case 64:		// channel A, gain factor 64
		hx->GAIN = 3;
		break;
	case 32:		// channel B, gain factor 32
		hx->GAIN = 2;
		break;
	}
}

long HX711_read(HX711 *hx) {

	// Wait for the chip to become ready.
	HX711_wait_ready(hx, 0);

	// Define structures for reading data into.
	unsigned long value = 0;
	uint8_t data[3] = { 0 };
	uint8_t filler = 0x00;

	// Pulse the clock pin 24 times to read the data.
	data[2] = shiftIn(hx, MSBFIRST);
	data[1] = shiftIn(hx, MSBFIRST);
	data[0] = shiftIn(hx, MSBFIRST);

	// Set the channel and the gain factor for the next reading using the clock pin.
	for (unsigned int i = 0; i < hx->GAIN; i++) {
		HAL_GPIO_WritePin(hx->PD_SCK_Port, hx->PD_SCK_Pin, GPIO_PIN_SET);
		for (uint16_t i = 0; i < SMALL_DELAY; i++) {} // Small delay
		HAL_GPIO_WritePin(hx->PD_SCK_Port, hx->PD_SCK_Pin, GPIO_PIN_RESET);
		for (uint16_t i = 0; i < SMALL_DELAY; i++) { } // Small delay
	}

	// Replicate the most significant bit to pad out a 32-bit signed integer
	if (data[2] & 0x80) {
		filler = 0xFF;
	} else {
		filler = 0x00;
	}

	// Construct a 32-bit signed integer
	value = (unsigned long) (filler << 24)
			| (unsigned long) (data[2] << 16)
			| (unsigned long) (data[1] << 8)
			| (unsigned long) (data[0]);

	return (long) value;
}

void HX711_wait_ready(HX711 *hx, unsigned long delay_ms) {
	// Wait for the chip to become ready.
	// This is a blocking implementation and will
	// halt the sketch until a load cell is connected.
	while (!HX711_is_ready(hx)) {
		// Probably will do no harm on AVR but will feed the Watchdog Timer (WDT) on ESP.
		// https://github.com/bogde/HX711/issues/73
		HAL_Delay(delay_ms);
	}
}

bool HX711_wait_ready_retry(HX711 *hx, int retries, unsigned long delay_ms) {
	// Wait for the chip to become ready by
	// retrying for a specified amount of attempts.
	// https://github.com/bogde/HX711/issues/76
	int count = 0;
	while (count < retries) {
		if (HX711_is_ready(hx)) {
			return true;
		}
		HAL_Delay(delay_ms);
		count++;
	}
	return false;
}

bool HX711_wait_ready_timeout(HX711 *hx, unsigned long timeout, unsigned long delay_ms) {
	// Wait for the chip to become ready until timeout.
	// https://github.com/bogde/HX711/pull/96
	unsigned long millisStarted = HAL_GetTick();
	while (HAL_GetTick() - millisStarted < timeout) {
		if (HX711_is_ready(hx)) {
			return true;
		}
		HAL_Delay(delay_ms);
	}
	return false;
}

long HX711_read_average(HX711 *hx, uint8_t times) {
	long sum = 0;
	for (uint8_t i = 0; i < times; i++) {
		sum += HX711_read(hx);
		// Probably will do no harm on AVR but will feed the Watchdog Timer (WDT) on ESP.
		// https://github.com/bogde/HX711/issues/73
		HAL_Delay(0);
	}
	return sum / times;
}

float HX711_get_value(HX711 *hx, uint8_t times) {
	return HX711_read_average(hx, times) - hx->OFFSET;
}

float HX711_get_units(HX711 *hx, uint8_t times) {
	return HX711_get_value(hx, times) / hx->SCALE;
}

void HX711_tare(HX711 *hx, uint8_t times) {
	float sum = HX711_read_average(hx, times);
	HX711_set_offset(hx, sum);
}

void HX711_set_scale(HX711 *hx, float scale) {
	hx->SCALE = scale;
}

float HX711_get_scale(HX711 *hx) {
	return hx->SCALE;
}

void HX711_set_offset(HX711 *hx, long offset) {
	hx->OFFSET = offset;
}

long HX711_get_offset(HX711 *hx) {
	return hx->OFFSET;
}

void HX711_power_down(HX711 *hx) {
	HAL_GPIO_WritePin(hx->PD_SCK_Port, hx->PD_SCK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(hx->PD_SCK_Port, hx->PD_SCK_Pin, GPIO_PIN_SET);
}

void HX711_power_up(HX711 *hx) {
	HAL_GPIO_WritePin(hx->PD_SCK_Port, hx->PD_SCK_Pin, GPIO_PIN_RESET);
}
