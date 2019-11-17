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

#include "MAX31865.h"

// Private
static void reconfigure(SPI* spi);

/**
 * The constructor for the MAX31865_RTD class registers the CS pin and
 * configures it as an output.
 *
 * @param [in] cs_pin Arduino pin selected for the CS signal.
 */
void MAX31865(SPI* spi, ptd type, uint16_t cs_pin, GPIO_TypeDef* GPIOx, SPI_HandleTypeDef* hspi) {
	/* Set the type of PTD. */
	spi->type = type;

	// SPI handler for the SPI device
	spi->hspi = hspi;

	/* CS pin for the SPI device. */
	spi->cs_pin = cs_pin;
	spi->GPIOx = GPIOx;

	/* Pull the CS pin high to avoid conflicts on SPI bus. */
	HAL_GPIO_WritePin(spi->GPIOx, spi->cs_pin, GPIO_PIN_SET);
}

/**
 * Configure the MAX31865.  The parameters correspond to Table 2 in the MAX31865
 * datasheet.  The parameters are combined into a control bit-field that is stored
 * internally in the class for later reconfiguration, as are the fault threshold values.
 *
 * @param [in] v_bias Vbias enabled (@a true) or disabled (@a false).
 * @param [in] conversion_mode Conversion mode auto (@a true) or off (@a false).
 * @param [in] one_shot 1-shot measurement enabled (@a true) or disabled (@a false).
 * @param [in] three_wire 3-wire enabled (@a true) or 2-wire/4-wire (@a false).
 * @param [in] fault_detection Fault detection cycle control (see Table 3 in the MAX31865
 *             datasheet).
 * @param [in] fault_clear Fault status auto-clear (@a true) or manual clear (@a false).
 * @param [in] filter_50hz 50 Hz filter enabled (@a true) or 60 Hz filter enabled
 *             (@a false).
 */
void configure(SPI* spi, uint8_t v_bias, uint8_t conversion_mode, uint8_t one_shot, uint8_t three_wire, uint8_t fault_cycle, uint8_t fault_clear, uint8_t filter_50hz) {
	uint8_t control_bits = 0;

	/* Assemble the control bit mask. */
	control_bits |= (v_bias ? 0x80 : 0);
	control_bits |= (conversion_mode ? 0x40 : 0);
	control_bits |= (one_shot ? 0x20 : 0);
	control_bits |= (three_wire ? 0x10 : 0);
	control_bits |= fault_cycle & 0b00001100;
	control_bits |= (fault_clear ? 0x02 : 0);
	control_bits |= (filter_50hz ? 0x01 : 0);

	/* Store the control bits. */
	spi->configuration_control_bits = control_bits;

	/* Perform an initial "reconfiguration." */
	reconfigure(spi);
}

/**
 * Reconfigure the MAX31865 by writing the stored control bits and the stored fault
 * threshold values back to the chip.
 */
static void reconfigure(SPI* spi) {
	/* Write the configuration to the MAX31865. */
	HAL_GPIO_WritePin(spi->GPIOx, spi->cs_pin, GPIO_PIN_RESET);
	uint8_t data[1] = {0x80}; // Configuration address
	HAL_SPI_Transmit(spi->hspi, data, 1, 5);
	data[0] = spi->configuration_control_bits;
	HAL_SPI_Transmit(spi->hspi, data, 1, 5);
	HAL_GPIO_WritePin(spi->GPIOx, spi->cs_pin, GPIO_PIN_SET);

	/* Write the threshold values. */
	HAL_GPIO_WritePin(spi->GPIOx, spi->cs_pin, GPIO_PIN_RESET);
	data[0] = 0x83; // MAX High Fault Threshold MSB address
	HAL_SPI_Transmit(spi->hspi, data, 1, 5);
	data[0] = 0xff; // 255
	HAL_SPI_Transmit(spi->hspi, data, 1, 5);
	data[0] = 0xfe; // 127
	HAL_SPI_Transmit(spi->hspi, data, 1, 5);
	// These are the MIN low fault threshold values
	data[0] = 0x00;
	HAL_SPI_Transmit(spi->hspi, data, 1, 5);
	data[0] = 0x00;
	HAL_SPI_Transmit(spi->hspi, data, 1, 5);
	HAL_GPIO_WritePin(spi->GPIOx, spi->cs_pin, GPIO_PIN_SET);
}

/**
 * Apply the Callendar-Van Dusen equation to convert the RTD resistance
 * to temperature:
 *
 *   \f[
 *   t=\frac{-A\pm \sqrt{A^2-4B\left(1-\frac{R_t}{R_0}\right)}}{2B}
 *   \f],
 *
 * where
 *
 * \f$A\f$ and \f$B\f$ are the RTD coefficients, \f$R_t\f$ is the current
 * resistance of the RTD, and \f$R_0\f$ is the resistance of the RTD at 0
 * degrees Celcius.
 *
 * For more information on measuring with an RTD, see:
 * <http://newton.ex.ac.uk/teaching/CDHW/Sensors/an046.pdf>.
 *
 * @param [in] resistance The measured RTD resistance.
 * @return Temperature in degrees Celcius.
 */
float temperature(SPI* spi) {
	static const float a2 = 2.0 * RTD_B;
	static const float b_sq = RTD_A * RTD_A;

	const float rtd_resistance = (spi->type == RTD_PT100) ? RTD_RESISTANCE_PT100 : RTD_RESISTANCE_PT1000;

	float c = 1.0 - resistance(spi) / rtd_resistance;
	float D = b_sq - 2.0 * a2 * c;
	float temperature_deg_C = (-RTD_A + sqrt(D)) / a2;

	return (temperature_deg_C);
}

/**
 * Read all settings and measurements from the MAX31865 and store them
 * internally in the class.
 *
 * @return Fault status byte
 */
uint8_t read_all(SPI* spi) {
	uint16_t combined_bytes;

	/* Start the read operation. */
	HAL_GPIO_WritePin(spi->GPIOx, spi->cs_pin, GPIO_PIN_RESET);
	/* Tell the MAX31865 that we want to read, starting at register 0. */
	uint8_t data[1] = {0x00}; // Configure address
	HAL_SPI_Transmit(spi->hspi, data, 1, 5);

	/* Read the MAX31865 registers in the following order:
	 Configuration
	 RTD
	 High Fault Threshold
	 Low Fault Threshold
	 Fault Status */

	uint8_t address[1] = {0x00};
	HAL_SPI_TransmitReceive(spi->hspi, address, data, 1, 5);
	spi->measured_configuration = data[0];

	HAL_SPI_TransmitReceive(spi->hspi, address, data, 1, 5);
	combined_bytes = data[0] << 8;
	HAL_SPI_TransmitReceive(spi->hspi, address, data, 1, 5);
	combined_bytes |= data[0];
	spi->measured_resistance = combined_bytes << 1;

	HAL_SPI_TransmitReceive(spi->hspi, address, data, 1, 5);
	combined_bytes = data[0] << 8;
	HAL_SPI_TransmitReceive(spi->hspi, address, data, 1, 5);
	combined_bytes |= data[0];
	spi->measured_high_threshold = combined_bytes << 1;

	HAL_SPI_TransmitReceive(spi->hspi, address, data, 1, 5);
	combined_bytes = data[0] << 8;
	HAL_SPI_TransmitReceive(spi->hspi, address, data, 1, 5);
	combined_bytes |= data[0];
	spi->measured_low_threshold = combined_bytes << 1;

	HAL_SPI_TransmitReceive(spi->hspi, address, data, 1, 5);
	spi->measured_status = data[0];

	HAL_GPIO_WritePin(spi->GPIOx, spi->cs_pin, GPIO_PIN_SET);

	/* Reset the configuration if the measured resistance is
	 zero or a fault occurred. */
	if ((spi->measured_resistance == 0) || (spi->measured_status != 0)) {
		reconfigure(spi);
	}

	return (status(spi));
}

/**
 * Get the status of transmitter
 */
uint8_t status(SPI* spi) {
	return spi->measured_status;
}

/**
 * Get the low threshold
 */
uint16_t low_threshold(SPI* spi) {
	return spi->measured_low_threshold;
}

/**
 * Get the high threshold
 */
uint16_t high_threshold(SPI* spi) {
	return spi->measured_high_threshold;
}

/**
 * Get the measure resistance
 */
uint16_t raw_resistance(SPI* spi) {
	return spi->measured_resistance;
}

/**
 * Get the resistance
 */
float resistance(SPI* spi) {
	float rtd_rref = (spi->type == RTD_PT100) ? (float) RTD_RREF_PT100 : (float) RTD_RREF_PT1000;
	return ((float) raw_resistance(spi) * rtd_rref / (float) RTD_ADC_RESOLUTION);
}
