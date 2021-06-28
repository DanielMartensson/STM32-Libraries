/*
 * VL6180X.c
 *
 *  Created on: Jun 23, 2021
 *      Author: Daniel Mårtensson
 */

#include "VL6180X.h"

// Private functions
void write16(uint16_t address, uint16_t data);
void write8(uint16_t address, uint8_t data);
uint16_t read16(uint16_t address);
uint8_t read8(uint16_t address);
void loadSettings(void);
static uint8_t _i2caddr;
static I2C_HandleTypeDef *VL6180X_i2c;

// Define some additional registers mentioned in application notes and we use
///! period between each measurement when in continuous mode
#define SYSRANGE_INTERMEASUREMENT_PERIOD 0x001b // P19 application notes

/**************************************************************************/
/*!
 @brief  Initializes VL6180X and check if it is found and resets chip.
 @param  Pointer of i2c
 @returns True if chip found and initialized, False otherwise
 */
/**************************************************************************/
bool VL6180X_Init(I2C_HandleTypeDef *hi2c) {
	VL6180X_i2c = hi2c;
	_i2caddr = VL6180X_DEFAULT_I2C_ADDR;

	if (read8(VL6180X_REG_IDENTIFICATION_MODEL_ID) != 0xB4) {
		return false;
	}

	// if (read8(VL6180X_REG_SYSTEM_FRESH_OUT_OF_RESET) == 0x01) {
	loadSettings();
	//}

	write8(VL6180X_REG_SYSTEM_FRESH_OUT_OF_RESET, 0x00);

	return true;
}

/**************************************************************************/
/*!
 @brief  Sets the address of the device to a different address chip.
 @param newAddr new I2C address for the device.
 @returns True if write succeeded.
 */
/**************************************************************************/
bool VL6180X_setAddress(uint8_t newAddr) {
	// BUGBUG - not sure if we detect errors or not...
	// The register is mentioned in app notes.
	write8(VL6180X_REG_SLAVE_DEVICE_ADDRESS, newAddr & 0x7F);
	_i2caddr = newAddr;
	return true;
}

/**************************************************************************/
/*!
 @brief  gets the address of the device
 chip.
 @returns the address
 */
/**************************************************************************/
uint8_t VL6180X_getAddress(void) {
	return _i2caddr;
}

/**************************************************************************/
/*!
 @brief  Load the settings for proximity/distance ranging
 */
/**************************************************************************/

void loadSettings(void) {
	// load settings!

	// private settings from page 24 of app note
	write8(0x0207, 0x01);
	write8(0x0208, 0x01);
	write8(0x0096, 0x00);
	write8(0x0097, 0xfd);
	write8(0x00e3, 0x00);
	write8(0x00e4, 0x04);
	write8(0x00e5, 0x02);
	write8(0x00e6, 0x01);
	write8(0x00e7, 0x03);
	write8(0x00f5, 0x02);
	write8(0x00d9, 0x05);
	write8(0x00db, 0xce);
	write8(0x00dc, 0x03);
	write8(0x00dd, 0xf8);
	write8(0x009f, 0x00);
	write8(0x00a3, 0x3c);
	write8(0x00b7, 0x00);
	write8(0x00bb, 0x3c);
	write8(0x00b2, 0x09);
	write8(0x00ca, 0x09);
	write8(0x0198, 0x01);
	write8(0x01b0, 0x17);
	write8(0x01ad, 0x00);
	write8(0x00ff, 0x05);
	write8(0x0100, 0x05);
	write8(0x0199, 0x05);
	write8(0x01a6, 0x1b);
	write8(0x01ac, 0x3e);
	write8(0x01a7, 0x1f);
	write8(0x0030, 0x00);

	// Recommended _ Public registers - See data sheet for more detail
	write8(0x0011, 0x10); // Enables polling for 'New Sample ready' when measurement completes
	write8(0x010a, 0x30); // Set the averaging sample period (compromise between lower noise and increased execution time)
	write8(0x003f, 0x46); // Sets the light and dark gain (upper nibble). Dark gain should not be changed.
	write8(0x0031, 0xFF); // sets the # of range measurements after which auto calibration of system is performed
	write8(0x0040, 0x63); // Set ALS integration time to 100ms
	write8(0x002e, 0x01); // perform a single temperature calibration of the ranging sensor

	// Optional_ Public registers - See data sheet for more detail
	write8(SYSRANGE_INTERMEASUREMENT_PERIOD, 0x09); // Set default ranging inter-measurement period to 100ms
	write8(0x003e, 0x31); 	// Set default ALS inter-measurement period to 500ms
	write8(0x0014, 0x24); // Configures interrupt on 'New Sample Ready threshold event'
}

/**************************************************************************/
/*!
 @brief  Single shot ranging. Be sure to check the return of {@link
 readRangeStatus} to before using the return value!
 @return Distance in millimeters if valid
 */
/**************************************************************************/

uint8_t VL6180X_readRange(void) {
	// wait for device to be ready for range measurement
	while (!(read8(VL6180X_REG_RESULT_RANGE_STATUS) & 0x01))
		;

	// Start a range measurement
	write8(VL6180X_REG_SYSRANGE_START, 0x01);

	// Poll until bit 2 is set
	while (!(read8(VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO) & 0x04))
		;

	// read range in mm
	uint8_t range = read8(VL6180X_REG_RESULT_RANGE_VAL);

	// clear interrupt
	write8(VL6180X_REG_SYSTEM_INTERRUPT_CLEAR, 0x07);

	return range;
}

/**************************************************************************/
/*!
 @brief  start Single shot ranging. The caller of this should have code
 that waits until the read completes, by either calling
 {@link waitRangeComplete} or calling {@link isRangeComplete} until it
 returns true.  And then the code should call {@link readRangeResult}
 to retrieve the range value and clear out the internal status.
 @return true if range completed.
 */
/**************************************************************************/

bool VL6180X_startRange(void) {
	// wait for device to be ready for range measurement
	while (!(read8(VL6180X_REG_RESULT_RANGE_STATUS) & 0x01))
		;

	// Start a range measurement
	write8(VL6180X_REG_SYSRANGE_START, 0x01);

	return true;
}

/**************************************************************************/
/*!
 @brief  Check to see if the range command completed.
 @return true if range completed.
 */
/**************************************************************************/

bool VL6180X_isRangeComplete(void) {

	// Poll until bit 2 is set
	if ((read8(VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO) & 0x04))
		return true;

	return false;
}

/**************************************************************************/
/*!
 @brief  Wait until Range completed
 @return true if range completed.
 */
/**************************************************************************/

bool VL6180X_waitRangeComplete(void) {

	// Poll until bit 2 is set
	while (!(read8(VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO) & 0x04))
		;

	return true;
}

/**************************************************************************/
/*!
 @brief  Return results of read reqyest also clears out the interrupt
 Be sure to check the return of {@link readRangeStatus} to before using
 the return value!
 @return if range started.
 */
/**************************************************************************/

uint8_t VL6180X_readRangeResult(void) {

	// read range in mm
	uint8_t range = read8(VL6180X_REG_RESULT_RANGE_VAL);

	// clear interrupt
	write8(VL6180X_REG_SYSTEM_INTERRUPT_CLEAR, 0x07);

	return range;
}

/**************************************************************************/
/*!
 @brief  Start continuous ranging
 @param  period_ms Optional Period between ranges in ms.  Values will
 be rounded down to 10ms units with minimum of 10ms.  Default is 50
 */
/**************************************************************************/

void VL6180X_startRangeContinuous(uint16_t period_ms) {
	uint8_t period_reg = 0;
	if (period_ms > 10) {
		if (period_ms < 2550)
			period_reg = (period_ms / 10) - 1;
		else
			period_reg = 254;
	}
	// Set  ranging inter-measurement
	write8(SYSRANGE_INTERMEASUREMENT_PERIOD, period_reg);

	// Start a continuous range measurement
	write8(VL6180X_REG_SYSRANGE_START, 0x03);
}

/**************************************************************************/
/*!
 @brief stop continuous range operation.
 */
/**************************************************************************/

void VL6180X_stopRangeContinuous(void) {
	// stop the continuous range operation, by setting the range register
	// back to 1, Page 7 of appication notes
	write8(VL6180X_REG_SYSRANGE_START, 0x01);
}

/**************************************************************************/
/*!
 @brief  Request ranging success/error message (retreive after ranging)
 @returns One of possible VL6180X_ERROR_* values
 */
/**************************************************************************/

uint8_t VL6180X_readRangeStatus(void) {
	return (read8(VL6180X_REG_RESULT_RANGE_STATUS) >> 4);
}

/**************************************************************************/
/*!
 @brief  Single shot lux measurement
 @param  gain Gain setting, one of VL6180X_ALS_GAIN_*
 @returns Lux reading
 */
/**************************************************************************/

float VL6180X_readLux(uint8_t gain) {
	uint8_t reg;

	reg = read8(VL6180X_REG_SYSTEM_INTERRUPT_CONFIG);
	reg &= ~0x38;
	reg |= (0x4 << 3); // IRQ on ALS ready
	write8(VL6180X_REG_SYSTEM_INTERRUPT_CONFIG, reg);

	// 100 ms integration period
	write8(VL6180X_REG_SYSALS_INTEGRATION_PERIOD_HI, 0);
	write8(VL6180X_REG_SYSALS_INTEGRATION_PERIOD_LO, 100);

	// analog gain
	if (gain > VL6180X_ALS_GAIN_40) {
		gain = VL6180X_ALS_GAIN_40;
	}
	write8(VL6180X_REG_SYSALS_ANALOGUE_GAIN, 0x40 | gain);

	// start ALS
	write8(VL6180X_REG_SYSALS_START, 0x1);

	// Poll until "New Sample Ready threshold event" is set
	while (4 != ((read8(VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO) >> 3) & 0x7))
		;

	// read lux!
	float lux = read16(VL6180X_REG_RESULT_ALS_VAL);

	// clear interrupt
	write8(VL6180X_REG_SYSTEM_INTERRUPT_CLEAR, 0x07);

	lux *= 0.32; // calibrated count/lux
	switch (gain) {
	case VL6180X_ALS_GAIN_1:
		break;
	case VL6180X_ALS_GAIN_1_25:
		lux /= 1.25;
		break;
	case VL6180X_ALS_GAIN_1_67:
		lux /= 1.67;
		break;
	case VL6180X_ALS_GAIN_2_5:
		lux /= 2.5;
		break;
	case VL6180X_ALS_GAIN_5:
		lux /= 5;
		break;
	case VL6180X_ALS_GAIN_10:
		lux /= 10;
		break;
	case VL6180X_ALS_GAIN_20:
		lux /= 20;
		break;
	case VL6180X_ALS_GAIN_40:
		lux /= 40;
		break;
	}
	lux *= 100;
	lux /= 100; // integration time in ms

	return lux;
}

/**************************************************************************/
/*!
 @brief  I2C low level interfacing
 */
/**************************************************************************/

// Read 1 byte from the VL6180X at 'address'
uint8_t read8(uint16_t address) {
	uint8_t data;
	uint8_t pData[2] = { address >> 8, address };
	HAL_I2C_Master_Transmit(VL6180X_i2c, _i2caddr, pData, 2, 10);
	HAL_I2C_Master_Receive(VL6180X_i2c, _i2caddr, pData, 1, 10);
	data = pData[0];
	return data;
}

// Read 2 byte from the VL6180X at 'address'
uint16_t read16(uint16_t address) {
	uint16_t data;
	uint8_t pData[2] = { address >> 8, address };
	HAL_I2C_Master_Transmit(VL6180X_i2c, _i2caddr, pData, 2, 10);
	HAL_I2C_Master_Receive(VL6180X_i2c, _i2caddr, pData, 2, 10);
	data = pData[0];
	data <<= 8;
	data |= pData[1];
	return data;
}

// write 1 byte
void write8(uint16_t address, uint8_t data) {
	uint8_t pData[3] = { address >> 8, address, data };
	HAL_I2C_Master_Transmit(VL6180X_i2c, _i2caddr, pData, 3, 10);
}

// write 2 bytes
void write16(uint16_t address, uint16_t data) {
	uint8_t pData[4] = { address >> 8, address, data >> 8, data };
	HAL_I2C_Master_Transmit(VL6180X_i2c, _i2caddr, pData, 4, 10);
}
