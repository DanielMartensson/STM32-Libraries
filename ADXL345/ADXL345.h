/*
 * ADXL345.h
 *
 *  Created on: Feb 15, 2020
 *      Author: SparkFun, Rewritten by Daniel Mårtensson
 */

#ifndef SRC_ADXL345_ADXL345_H_
#define SRC_ADXL345_ADXL345_H_

#include "main.h"
#include "stdbool.h"
#include "math.h"

/*************************** REGISTER MAP ***************************/
#define ADXL345_DEVID			0x00		// Device ID
#define ADXL345_RESERVED1		0x01		// Reserved. Do Not Access.
#define ADXL345_THRESH_TAP		0x1D		// Tap Threshold.
#define ADXL345_OFSX			0x1E		// X-Axis Offset.
#define ADXL345_OFSY			0x1F		// Y-Axis Offset.
#define ADXL345_OFSZ			0x20		// Z- Axis Offset.
#define ADXL345_DUR				0x21		// Tap Duration.
#define ADXL345_LATENT			0x22		// Tap Latency.
#define ADXL345_WINDOW			0x23		// Tap Window.
#define ADXL345_THRESH_ACT		0x24		// Activity Threshold
#define ADXL345_THRESH_INACT	0x25		// Inactivity Threshold
#define ADXL345_TIME_INACT		0x26		// Inactivity Time
#define ADXL345_ACT_INACT_CTL	0x27		// Axis Enable Control for Activity and Inactivity Detection
#define ADXL345_THRESH_FF		0x28		// Free-Fall Threshold.
#define ADXL345_TIME_FF			0x29		// Free-Fall Time.
#define ADXL345_TAP_AXES		0x2A		// Axis Control for Tap/Double Tap.
#define ADXL345_ACT_TAP_STATUS	0x2B		// Source of Tap/Double Tap
#define ADXL345_BW_RATE			0x2C		// Data Rate and Power mode Control
#define ADXL345_POWER_CTL		0x2D		// Power-Saving Features Control
#define ADXL345_INT_ENABLE		0x2E		// Interrupt Enable Control
#define ADXL345_INT_MAP			0x2F		// Interrupt Mapping Control
#define ADXL345_INT_SOURCE		0x30		// Source of Interrupts
#define ADXL345_DATA_FORMAT		0x31		// Data Format Control
#define ADXL345_DATAX0			0x32		// X-Axis Data 0
#define ADXL345_DATAX1			0x33		// X-Axis Data 1
#define ADXL345_DATAY0			0x34		// Y-Axis Data 0
#define ADXL345_DATAY1			0x35		// Y-Axis Data 1
#define ADXL345_DATAZ0			0x36		// Z-Axis Data 0
#define ADXL345_DATAZ1			0x37		// Z-Axis Data 1
#define ADXL345_FIFO_CTL		0x38		// FIFO Control
#define ADXL345_FIFO_STATUS		0x39		// FIFO Status

#define ADXL345_BW_1600			0xF			// 1111		IDD = 40uA
#define ADXL345_BW_800			0xE			// 1110		IDD = 90uA
#define ADXL345_BW_400			0xD			// 1101		IDD = 140uA
#define ADXL345_BW_200			0xC			// 1100		IDD = 140uA
#define ADXL345_BW_100			0xB			// 1011		IDD = 140uA
#define ADXL345_BW_50			0xA			// 1010		IDD = 140uA
#define ADXL345_BW_25			0x9			// 1001		IDD = 90uA
#define ADXL345_BW_12_5		    0x8			// 1000		IDD = 60uA
#define ADXL345_BW_6_25			0x7			// 0111		IDD = 50uA
#define ADXL345_BW_3_13			0x6			// 0110		IDD = 45uA
#define ADXL345_BW_1_56			0x5			// 0101		IDD = 40uA
#define ADXL345_BW_0_78			0x4			// 0100		IDD = 34uA
#define ADXL345_BW_0_39			0x3			// 0011		IDD = 23uA
#define ADXL345_BW_0_20			0x2			// 0010		IDD = 23uA
#define ADXL345_BW_0_10			0x1			// 0001		IDD = 23uA
#define ADXL345_BW_0_05			0x0			// 0000		IDD = 23uA

/************************** INTERRUPT PINS **************************/
#define ADXL345_INT1_PIN		0x00		//INT1: 0
#define ADXL345_INT2_PIN		0x01		//INT2: 1

/********************** INTERRUPT BIT POSITION **********************/
#define ADXL345_INT_DATA_READY_BIT		0x07
#define ADXL345_INT_SINGLE_TAP_BIT		0x06
#define ADXL345_INT_DOUBLE_TAP_BIT		0x05
#define ADXL345_INT_ACTIVITY_BIT		0x04
#define ADXL345_INT_INACTIVITY_BIT		0x03
#define ADXL345_INT_FREE_FALL_BIT		0x02
#define ADXL345_INT_WATERMARK_BIT		0x01
#define ADXL345_INT_OVERRUNY_BIT		0x00

#define ADXL345_DATA_READY				0x07
#define ADXL345_SINGLE_TAP				0x06
#define ADXL345_DOUBLE_TAP				0x05
#define ADXL345_ACTIVITY				0x04
#define ADXL345_INACTIVITY				0x03
#define ADXL345_FREE_FALL				0x02
#define ADXL345_WATERMARK				0x01
#define ADXL345_OVERRUNY				0x00

/****************************** ERRORS ******************************/
#define ADXL345_OK			1		// No Error
#define ADXL345_ERROR		0		// Error Exists

#define ADXL345_NO_ERROR	0		// Initial State
#define ADXL345_READ_ERROR	1		// Accelerometer Reading Error
#define ADXL345_BAD_ARG		2		// Bad Argument

typedef struct {
	uint8_t ADXL345_DEVICE;
	bool status;					// Set When Error Exists
	uint8_t _buff[6];				//	6 Bytes Buffer
	bool I2C;
	unsigned long SPIfreq; 			// = 5000000;
	uint8_t error_code;				// Initial State
	float gains[3];					// Counts to Gs
	SPI_HandleTypeDef *hspi;
	I2C_HandleTypeDef *hi2c;
	GPIO_TypeDef *CS_PORT;
	uint16_t CS_PIN;
} ADXL345_SPI_I2C;

// Publika funktioner
void ADXL345_I2C(ADXL345_SPI_I2C *adxl345, I2C_HandleTypeDef *hi2c, uint8_t address);
void ADXL345_SPI(ADXL345_SPI_I2C *adxl345, SPI_HandleTypeDef *hspi, GPIO_TypeDef *CS_PORT, uint16_t CS_PIN);
void ADXL345powerOn(ADXL345_SPI_I2C *adxl345);
void ADXL345readAccel_xyz(ADXL345_SPI_I2C *adxl345, int *xyz);
void ADXL345readAccel_x_y_z(ADXL345_SPI_I2C *adxl345, int *x, int *y, int *z);
void ADXL345get_Gxyz(ADXL345_SPI_I2C *adxl345, float *xyz);

void ADXL345setTapThreshold(ADXL345_SPI_I2C *adxl34, int tapThreshold);
int ADXL345getTapThreshold(ADXL345_SPI_I2C *adxl34);
void ADXL345setAxisGains(ADXL345_SPI_I2C *adxl34, float *_gains);
void ADXL345getAxisGains(ADXL345_SPI_I2C *adxl34, float *_gains);
void ADXL345setAxisOffset(ADXL345_SPI_I2C *adxl34, int x, int y, int z);
void ADXL345getAxisOffset(ADXL345_SPI_I2C *adxl34, int *x, int *y, int *z);
void ADXL345setTapDuration(ADXL345_SPI_I2C *adxl34, int tapDuration);
int ADXL345getTapDuration(ADXL345_SPI_I2C *adxl34);
void ADXL345setDoubleTapLatency(ADXL345_SPI_I2C *adxl34, int doubleTapLatency);
int ADXL345getDoubleTapLatency(ADXL345_SPI_I2C *adxl34);
void ADXL345setDoubleTapWindow(ADXL345_SPI_I2C *adxl34, int doubleTapWindow);
int ADXL345getDoubleTapWindow(ADXL345_SPI_I2C *adxl34);
void ADXL345setActivityThreshold(ADXL345_SPI_I2C *adxl34, int activityThreshold);
int ADXL345getActivityThreshold(ADXL345_SPI_I2C *adxl34);
void ADXL345setInactivityThreshold(ADXL345_SPI_I2C *adxl34, int inactivityThreshold);
int ADXL345getInactivityThreshold(ADXL345_SPI_I2C *adxl34);
void ADXL345setTimeInactivity(ADXL345_SPI_I2C *adxl34, int timeInactivity);
int ADXL345getTimeInactivity(ADXL345_SPI_I2C *adxl34);
void ADXL345setFreeFallThreshold(ADXL345_SPI_I2C *adxl34, int freeFallthreshold);
int ADXL345getFreeFallThreshold(ADXL345_SPI_I2C *adxl34);
void ADXL345setFreeFallDuration(ADXL345_SPI_I2C *adxl34, int freeFallDuration);
int ADXL345getFreeFallDuration(ADXL345_SPI_I2C *adxl34);

bool ADXL345isActivityXEnabled(ADXL345_SPI_I2C *adxl34);
bool ADXL345isActivityYEnabled(ADXL345_SPI_I2C *adxl34);
bool ADXL345isActivityZEnabled(ADXL345_SPI_I2C *adxl34);
bool ADXL345isInactivityXEnabled(ADXL345_SPI_I2C *adxl34);
bool ADXL345isInactivityYEnabled(ADXL345_SPI_I2C *adxl34);
bool ADXL345isInactivityZEnabled(ADXL345_SPI_I2C *adxl34);
bool ADXL345isActivityAc(ADXL345_SPI_I2C *adxl34);
bool ADXL345isInactivityAc(ADXL345_SPI_I2C *adxl34);
void ADXL345setActivityAc(ADXL345_SPI_I2C *adxl34, bool state);
void ADXL345setInactivityAc(ADXL345_SPI_I2C *adxl34, bool state);

bool ADXL345getSuppressBit(ADXL345_SPI_I2C *adxl34);
void ADXL345setSuppressBit(ADXL345_SPI_I2C *adxl34, bool state);
bool ADXL345isTapDetectionOnX(ADXL345_SPI_I2C *adxl34);
void ADXL345setTapDetectionOnX(ADXL345_SPI_I2C *adxl34, bool state);
bool ADXL345isTapDetectionOnY(ADXL345_SPI_I2C *adxl34);
void ADXL345setTapDetectionOnY(ADXL345_SPI_I2C *adxl34, bool state);
bool ADXL345isTapDetectionOnZ(ADXL345_SPI_I2C *adxl34);
void ADXL345setTapDetectionOnZ(ADXL345_SPI_I2C *adxl34, bool state);
void ADXL345setTapDetectionOnXYZ(ADXL345_SPI_I2C *adxl34, bool stateX, bool stateY, bool stateZ);

void ADXL345setActivityX(ADXL345_SPI_I2C *adxl34, bool state);
void ADXL345setActivityY(ADXL345_SPI_I2C *adxl34, bool state);
void ADXL345setActivityZ(ADXL345_SPI_I2C *adxl34, bool state);
void ADXL345setActivityXYZ(ADXL345_SPI_I2C *adxl34, bool stateX, bool stateY, bool stateZ);
void ADXL345setInactivityX(ADXL345_SPI_I2C *adxl34, bool state);
void ADXL345setInactivityY(ADXL345_SPI_I2C *adxl34, bool state);
void ADXL345setInactivityZ(ADXL345_SPI_I2C *adxl34, bool state);
void ADXL345setInactivityXYZ(ADXL345_SPI_I2C *adxl34, bool stateX, bool stateY, bool stateZ);

bool ADXL345isActivitySourceOnX(ADXL345_SPI_I2C *adxl34);
bool ADXL345isActivitySourceOnY(ADXL345_SPI_I2C *adxl34);
bool ADXL345isActivitySourceOnZ(ADXL345_SPI_I2C *adxl34);
bool ADXL345isTapSourceOnX(ADXL345_SPI_I2C *adxl34);
bool ADXL345isTapSourceOnY(ADXL345_SPI_I2C *adxl34);
bool ADXL345isTapSourceOnZ(ADXL345_SPI_I2C *adxl34);
bool ADXL345isAsleep(ADXL345_SPI_I2C *adxl34);

bool ADXL345isLowPower(ADXL345_SPI_I2C *adxl34);
void ADXL345setLowPower(ADXL345_SPI_I2C *adxl34, bool state);
double ADXL345getRate(ADXL345_SPI_I2C *adxl34);
void ADXL345setRate(ADXL345_SPI_I2C *adxl34, float rate);
void ADXL345set_bw(ADXL345_SPI_I2C *adxl34, uint8_t bw_code);
uint8_t ADXL345get_bw_code(ADXL345_SPI_I2C *adxl34);

bool ADXL345triggered(ADXL345_SPI_I2C *adxl34, uint8_t interrupts, int mask);

uint8_t ADXL345getInterruptSource(ADXL345_SPI_I2C *adxl34);
bool ADXL345getInterruptSource_interrupt(ADXL345_SPI_I2C *adxl34, uint8_t interruptBit);
bool ADXL345getInterruptMapping(ADXL345_SPI_I2C *adxl34, uint8_t interruptBit);
void ADXL345setInterruptMapping(ADXL345_SPI_I2C *adxl34, uint8_t interruptBit, bool interruptPin);
bool ADXL345isInterruptEnabled(ADXL345_SPI_I2C *adxl34, uint8_t interruptBit);
void ADXL345setInterrupt(ADXL345_SPI_I2C *adxl34, uint8_t interruptBit, bool state);
void ADXL345setImportantInterruptMapping(ADXL345_SPI_I2C *adxl34, int single_tap, int double_tap, int free_fall, int activity, int inactivity);
void ADXL345InactivityINT(ADXL345_SPI_I2C *adxl34, bool status);
void ADXL345ActivityINT(ADXL345_SPI_I2C *adxl34, bool status);
void ADXL345FreeFallINT(ADXL345_SPI_I2C *adxl34, bool status);
void ADXL345doubleTapINT(ADXL345_SPI_I2C *adxl34, bool status);
void ADXL345singleTapINT(ADXL345_SPI_I2C *adxl34, bool status);

void ADXL345getRangeSetting(ADXL345_SPI_I2C *adxl34, uint8_t *rangeSetting);
void ADXL345setRangeSetting(ADXL345_SPI_I2C *adxl34, int val);
bool ADXL345getSelfTestBit(ADXL345_SPI_I2C *adxl34);
void ADXL345setSelfTestBit(ADXL345_SPI_I2C *adxl34, bool selfTestBit);
bool ADXL345getSpiBit(ADXL345_SPI_I2C *adxl34);
void ADXL345setSpiBit(ADXL345_SPI_I2C *adxl34, bool spiBit);
bool ADXL345getInterruptLevelBit(ADXL345_SPI_I2C *adxl34);
void ADXL345setInterruptLevelBit(ADXL345_SPI_I2C *adxl34, bool interruptLevelBit);
bool ADXL345getFullResBit(ADXL345_SPI_I2C *adxl34);
void ADXL345setFullResBit(ADXL345_SPI_I2C *adxl34, bool fullResBit);
bool ADXL345getJustifyBit(ADXL345_SPI_I2C *adxl34);
void ADXL345setJustifyBit(ADXL345_SPI_I2C *adxl34, bool justifyBit);

#endif /* SRC_ADXL345_ADXL345_H_ */
