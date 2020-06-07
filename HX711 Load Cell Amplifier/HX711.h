/*
 * HX711.h
 *
 *  Created on: Jun 6, 2020
 *      Author: Daniel Mårtensson
 */

#ifndef SRC_HX711_HX711_H_
#define SRC_HX711_HX711_H_

#include "main.h"
#include "stdbool.h"

#define SMALL_DELAY 10
#define LSBFIRST 0
#define MSBFIRST 1
typedef struct {
	// Power Down and Serial Clock Input Pin - This must be output!
	GPIO_TypeDef *PD_SCK_Port;
	uint16_t PD_SCK_Pin;

	// Serial Data Output Pin - This must be pull-up!
	GPIO_TypeDef *DOUT_Port;
	uint16_t DOUT_Pin;

	uint8_t GAIN;		// amplification factor
	long OFFSET;		// used for tare weight
	float SCALE;		// used to return weight in grams, kg, ounces, whatever
} HX711;

// Initialize library with data output pin, clock input pin and gain factor.
// Channel selection is made by passing the appropriate gain:
// - With a gain factor of 64 or 128, channel A is selected
// - With a gain factor of 32, channel B is selected
// The library default is "128" (Channel A).
void HX711_begin(HX711 *hx, GPIO_TypeDef* PD_SCK_Port, uint16_t PD_SCK_Pin, GPIO_TypeDef* DOUT_Port, uint16_t DOUT_Pin, uint8_t gain);

// Check if HX711 is ready
// from the datasheet: When output data is not ready for retrieval, digital output pin DOUT is high. Serial clock
// input PD_SCK should be low. When DOUT goes to low, it indicates data is ready for retrieval.
bool HX711_is_ready(HX711 *hx);

// Wait for the HX711 to become ready
void HX711_wait_ready(HX711 *hx, unsigned long delay_ms);
bool HX711_wait_ready_retry(HX711 *hx, int retries, unsigned long delay_ms);
bool HX711_wait_ready_timeout(HX711 *hx, unsigned long timeout, unsigned long delay_ms);

// set the gain factor; takes effect only after a call to read()
// channel A can be set for a 128 or 64 gain; channel B has a fixed 32 gain
// depending on the parameter, the channel is also set to either A or B
void HX711_set_gain(HX711 *hx, uint8_t gain);

// waits for the chip to be ready and returns a reading
long HX711_read(HX711 *hx);

// returns an average reading; times = how many times to read
long HX711_read_average(HX711 *hx, uint8_t times);

// returns (read_average() - OFFSET), that is the current value without the tare weight; times = how many readings to do
float HX711_get_value(HX711 *hx, uint8_t times);

// returns get_value() divided by SCALE, that is the raw value divided by a value obtained via calibration
// times = how many readings to do
float HX711_get_units(HX711 *hx, uint8_t times);

// set the OFFSET value for tare weight; times = how many times to read the tare value
void HX711_tare(HX711 *hx, uint8_t times);

// set the SCALE value; this value is used to convert the raw data to "human readable" data (measure units)
void HX711_set_scale(HX711 *hx, float scale);

// get the current SCALE
float HX711_get_scale(HX711 *hx);

// set OFFSET, the value that's subtracted from the actual reading (tare weight)
void HX711_set_offset(HX711 *hx, long offset);

// get the current OFFSET
long HX711_get_offset(HX711 *hx);

// puts the chip into power down mode
void HX711_power_down(HX711 *hx);

// wakes up the chip after power down mode
void HX711_power_up(HX711 *hx);

#endif /* SRC_HX711_HX711_H_ */
