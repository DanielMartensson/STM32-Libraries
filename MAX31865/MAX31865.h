/**************************************************************************
 * STM32 driver library for the MAX31865.
 *
 * Copyright (C) 2015 Ole Wolf <wolf@blazingangles.com>
 * Rewritten by Daniel Mårtensson 2019 for STM32
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **************************************************************************/

#ifndef _MAX31865_H
#define _MAX31865_H

#include "main.h"
#include <math.h>

#define MAX31865_FAULT_HIGH_THRESHOLD  ( 1 << 7 )
#define MAX31865_FAULT_LOW_THRESHOLD   ( 1 << 6 )
#define MAX31865_FAULT_REFIN           ( 1 << 5 )
#define MAX31865_FAULT_REFIN_FORCE     ( 1 << 4 )
#define MAX31865_FAULT_RTDIN_FORCE     ( 1 << 3 )
#define MAX31865_FAULT_VOLTAGE         ( 1 << 2 )

#define MAX31865_FAULT_DETECTION_NONE      ( 0x00 << 2 )
#define MAX31865_FAULT_DETECTION_AUTO      ( 0x01 << 2 )
#define MAX31865_FAULT_DETECTION_MANUAL_1  ( 0x02 << 2 )
#define MAX31865_FAULT_DETECTION_MANUAL_2  ( 0x03 << 2 )

/* RTD data, RTD current, and measurement reference
 voltage. The ITS-90 standard is used; other RTDs
 may have coefficients defined by the DIN 43760 or
 the U.S. Industrial (American) standard. */
#define RTD_A_ITS90         3.9080e-3
#define RTD_A_USINDUSTRIAL  3.9692e-3
#define RTD_A_DIN43760      3.9848e-3
#define RTD_B_ITS90         -5.870e-7
#define RTD_B_USINDUSTRIAL  -5.8495e-7
#define RTD_B_DIN43760      -5.8019e-7

/* USER CONFIGURATION:
 *
 * RTD coefficient C is required only for temperatures
 below 0 deg. C.  The selected RTD coefficient set
 is specified below. */
#define SELECT_RTD_HELPER(x) x
#define SELECT_RTD(x) SELECT_RTD_HELPER(x)
#define RTD_A         SELECT_RTD(RTD_A_DIN43760)
#define RTD_B         SELECT_RTD(RTD_B_DIN43760)

/*
 * USER CALIBRATION:
 * The reference resistor on the hardware; see the MAX31865 datasheet
 * for details.  The values 400 and 4000 Ohm are recommended values for
 * the PT100 and PT1000. For Adafruit MAX31865: RTD100 = 430 Ohm, RTD1000 = 4300 ohm
 * Or you can set a 220 Ohm resistor and change the RTD_PREF until the raw_resicance is the same
 */
#define RTD_RREF_PT100         218 /* Ohm - This need to be calibrated by you */
#define RTD_RREF_PT1000       4300 /* Ohm - Same as here as well*/


/*
 * The RTD resistance at 0 degrees Celcius.  For the PT100, this is 100 Ohm;
 * for the PT1000, it is 1000 Ohm.
 */
#define RTD_RESISTANCE_PT100   100 /* Ohm */
#define RTD_RESISTANCE_PT1000 1000 /* Ohm */

#define RTD_ADC_RESOLUTION  ( 1u << 15 ) /* 15 bits */

// Booleans
#define TRUE 1
#define FALSE 0

enum ptd_types {
	RTD_PT100, RTD_PT1000
};
typedef uint8_t ptd;

// struct
typedef struct {
	/* Our configuration. */
	uint16_t cs_pin;
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* GPIOx;
	ptd type;
	uint8_t configuration_control_bits;
	uint16_t configuration_low_threshold;
	uint16_t configuration_high_threshold;

	/* Values read from the device. */
	uint8_t measured_configuration;
	uint16_t measured_resistance;
	uint16_t measured_high_threshold;
	uint16_t measured_low_threshold;
	uint8_t measured_status;
} SPI;

void MAX31865(SPI* spi, ptd type, uint16_t cs_pin, GPIO_TypeDef* GPIOx, SPI_HandleTypeDef* hspi);
void configure(SPI* spi, uint8_t v_bias, uint8_t conversion_mode, uint8_t one_shot, uint8_t three_wire, uint8_t fault_cycle, uint8_t fault_clear, uint8_t filter_50hz);
uint8_t read_all(SPI* spi);
float temperature(SPI* spi);
uint8_t status(SPI* spi);
uint16_t low_threshold(SPI* spi);
uint16_t high_threshold(SPI* spi);
uint16_t raw_resistance(SPI* spi);
float resistance(SPI* spi);

#endif /* _MAX31865_H */
