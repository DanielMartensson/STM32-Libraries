/*
 * ADXL345.c
 *
 *  Created on: Feb 15, 2020
 *      Author: SparkFun, Rewritten by Daniel Mårtensson
 */


#include "ADXL345.h"

// Private functions
static void writeTo(ADXL345_SPI_I2C* adxl345, uint8_t address, uint8_t val);
static void writeToI2C(ADXL345_SPI_I2C* adxl345, uint8_t address, uint8_t val);
static void writeToSPI(ADXL345_SPI_I2C* adxl345, uint8_t address, uint8_t val);
static void readFrom(ADXL345_SPI_I2C* adxl345, uint8_t address, int num, uint8_t buff[]);
static void readFromI2C(ADXL345_SPI_I2C* adxl345, uint8_t address, int num, uint8_t buff[]);
static void readFromSPI(ADXL345_SPI_I2C* adxl345, uint8_t address, int num, uint8_t buff[]);
static void setRegisterBit(ADXL345_SPI_I2C* adxl345, uint8_t regAdress, int bitPos, bool state);
static bool getRegisterBit(ADXL345_SPI_I2C* adxl345, uint8_t regAdress, int bitPos);
static void setRegisterBit(ADXL345_SPI_I2C* adxl345, uint8_t regAdress, int bitPos, bool state) ;
static bool getRegisterBit(ADXL345_SPI_I2C* adxl345 , uint8_t regAdress, int bitPos);
static int constrain(ADXL345_SPI_I2C* adxl345, int tapThreshold, int lower, int upper);


#define ADXL345_TO_READ (6)	// Number of Bytes Read - Two Bytes Per Axis

void ADXL345_I2C(ADXL345_SPI_I2C* adxl345, I2C_HandleTypeDef* hi2c, uint8_t address) {
	adxl345->status = ADXL345_OK;
	adxl345->error_code = ADXL345_NO_ERROR;
	adxl345->hi2c = hi2c;
	adxl345->ADXL345_DEVICE = address << 1; // Device Address for ADXL345 - Important with << 1 for setting the address!

	adxl345->gains[0] = 0.00376390;		// Original gain 0.00376390
	adxl345->gains[1] = 0.00376009;		// Original gain 0.00376009
	adxl345->gains[2] = 0.00349265;		// Original gain 0.00349265
	adxl345->I2C = true;

	// Check if we have trouble with I2C connection
	if (HAL_I2C_IsDeviceReady(adxl345->hi2c, adxl345->ADXL345_DEVICE, 10, 10) != HAL_OK)
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // This MUST have GPIO PA5 ready to use - ERROR I2C - Wrong address
}

/*
 * Use SPI:
 * CPOL = High
 * Prescaler = 64
 * CPHA = 2 Edge
 * Data size = 8 bit
 * First bit = MSB
 */
void ADXL345_SPI(ADXL345_SPI_I2C* adxl345, SPI_HandleTypeDef* hspi, GPIO_TypeDef* CS_PORT, uint16_t CS_PIN) {
	adxl345->status = ADXL345_OK;
	adxl345->error_code = ADXL345_NO_ERROR;
	adxl345->hspi = hspi;

	adxl345->gains[0] = 0.00376390;
	adxl345->gains[1] = 0.00376009;
	adxl345->gains[2] = 0.00349265;
	adxl345->CS_PIN = CS_PIN;
	adxl345->CS_PORT = CS_PORT;
	adxl345->I2C = false;

	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

void ADXL345powerOn(ADXL345_SPI_I2C* adxl345) {

	//ADXL345 TURN ON
	writeTo(adxl345, ADXL345_POWER_CTL, 0);		// Wakeup
	writeTo(adxl345, ADXL345_POWER_CTL, 16);	// Auto_Sleep
	writeTo(adxl345, ADXL345_POWER_CTL, 8);		// Measure
}


/*********************** READING ACCELERATION ***********************/
/*    Reads Acceleration into Three Variables:  x, y and z          */

void ADXL345readAccel_xyz(ADXL345_SPI_I2C* adxl345, int *xyz){
	ADXL345readAccel_x_y_z(adxl345, xyz, xyz + 1, xyz + 2);
}

void ADXL345readAccel_x_y_z(ADXL345_SPI_I2C* adxl345, int *x, int *y, int *z) {
	readFrom(adxl345, ADXL345_DATAX0, ADXL345_TO_READ, adxl345->_buff);	// Read Accel Data from ADXL345

	// Each Axis @ All g Ranges: 10 Bit Resolution (2 Bytes)
	*x = (int16_t)((((int)adxl345->_buff[1]) << 8) | adxl345->_buff[0]);
	*y = (int16_t)((((int)adxl345->_buff[3]) << 8) | adxl345->_buff[2]);
	*z = (int16_t)((((int)adxl345->_buff[5]) << 8) | adxl345->_buff[4]);

}

void ADXL345get_Gxyz(ADXL345_SPI_I2C* adxl345, float *xyz){
	int i;
	int xyz_int[3];
	ADXL345readAccel_xyz(adxl345, xyz_int);
	for(i=0; i<3; i++){
		xyz[i] = xyz_int[i] * adxl345->gains[i];
	}
}

/***************** WRITES VALUE TO ADDRESS REGISTER *****************/
static void writeTo(ADXL345_SPI_I2C* adxl345, uint8_t address, uint8_t val) {
	if(adxl345->I2C) {
		writeToI2C(adxl345, address, val);
	}
	else {
		writeToSPI(adxl345, address, val);
	}
}

/************************ READING NUM BYTES *************************/
/*    Reads Num Bytes. Starts from Address Reg to _buff Array        */
static void readFrom(ADXL345_SPI_I2C* adxl345, uint8_t address, int num, uint8_t _buff[]) {
	if(adxl345->I2C) {
		readFromI2C(adxl345, address, num, _buff);	// If I2C Communication
	}
	else {
		readFromSPI(adxl345, address, num, _buff);	// If SPI Communication
	}
}

/*************************** WRITE TO I2C ***************************/
/*      Start; Send Register Address; Send Value To Write; End      */
static void writeToI2C(ADXL345_SPI_I2C* adxl345, uint8_t _address, uint8_t _val) {
	uint8_t pData[2] = {_address, _val};
	HAL_I2C_Master_Transmit(adxl345->hi2c, adxl345->ADXL345_DEVICE, pData, 2, 10);
}

/*************************** READ FROM I2C **************************/
/*                Start; Send Address To Read; End                  */
static void readFromI2C(ADXL345_SPI_I2C* adxl345, uint8_t address, int num, uint8_t _buff[]) {
	// Send data
	uint8_t pData[1] = {address};
	HAL_I2C_Master_Transmit(adxl345->hi2c, adxl345->ADXL345_DEVICE, pData, 1, 10);

	// Get data
	if(HAL_I2C_Master_Receive(adxl345->hi2c, adxl345->ADXL345_DEVICE, _buff, (uint16_t) num, 10) != HAL_OK){
		adxl345->status = ADXL345_ERROR;
		adxl345->error_code = ADXL345_READ_ERROR;
	}else{
		adxl345->status = ADXL345_OK;
		adxl345->error_code = ADXL345_OK;
	}

}

/************************** WRITE FROM SPI **************************/
/*         Point to Destination; Write Value; Turn Off              */
static void writeToSPI(ADXL345_SPI_I2C* adxl345, uint8_t __reg_address, uint8_t __val) {
  HAL_GPIO_WritePin(adxl345->CS_PORT, adxl345->CS_PIN, GPIO_PIN_RESET);
  uint8_t pData[2];
  pData[0] = __reg_address | 0x40; // multibyte write enabled
  pData[1] = __val;
  HAL_SPI_Transmit(adxl345->hspi, pData, 2, 10);
  HAL_GPIO_WritePin(adxl345->CS_PORT, adxl345->CS_PIN, GPIO_PIN_SET);
}

/*************************** READ FROM SPI **************************/
/*                                                                  */
static void readFromSPI(ADXL345_SPI_I2C* adxl345, uint8_t __reg_address, int num, uint8_t _buff[]) {
  // Read: Most Sig Bit of Reg Address Set
  uint8_t _address = 0x80 | __reg_address;
  // If Multi-Byte Read: Bit 6 Set
  if(num > 1) {
  	_address = _address | 0x40;
  }
  // Write
  HAL_GPIO_WritePin(adxl345->CS_PORT, adxl345->CS_PIN, GPIO_PIN_RESET);
  HAL_SPI_Transmit(adxl345->hspi, &_address, 1, 10);
  HAL_SPI_Receive(adxl345->hspi, _buff, (uint16_t) num, 10);
  HAL_GPIO_WritePin(adxl345->CS_PORT, adxl345->CS_PIN, GPIO_PIN_SET);
}

/*************************** RANGE SETTING **************************/
/*          ACCEPTABLE VALUES: 2g, 4g, 8g, 16g ~ GET & SET          */
void ADXL345getRangeSetting(ADXL345_SPI_I2C* adxl345, uint8_t* rangeSetting) {
	uint8_t _b;
	readFrom(adxl345, ADXL345_DATA_FORMAT, 1, &_b);
	*rangeSetting = _b & 0b00000011;
}

void ADXL345setRangeSetting(ADXL345_SPI_I2C* adxl345, int val) {
	uint8_t _s;
	uint8_t _b;

	switch (val) {
		case 2:
			_s = 0b00000000;
			break;
		case 4:
			_s = 0b00000001;
			break;
		case 8:
			_s = 0b00000010;
			break;
		case 16:
			_s = 0b00000011;
			break;
		default:
			_s = 0b00000000;
	}
	readFrom(adxl345, ADXL345_DATA_FORMAT, 1, &_b);
	_s |= (_b & 0b11101100);
	writeTo(adxl345, ADXL345_DATA_FORMAT, _s);
}

/*************************** SELF_TEST BIT **************************/
/*                            ~ GET & SET                           */
bool ADXL345getSelfTestBit(ADXL345_SPI_I2C* adxl345) {
	return getRegisterBit(adxl345, ADXL345_DATA_FORMAT, 7);
}

// If Set (1) Self-Test Applied. Electrostatic Force exerted on the sensor
//  causing a shift in the output data.
// If Set (0) Self-Test Disabled.
void ADXL345setSelfTestBit(ADXL345_SPI_I2C* adxl345, bool selfTestBit) {
	setRegisterBit(adxl345, ADXL345_DATA_FORMAT, 7, selfTestBit);
}

/*************************** SPI BIT STATE **************************/
/*                           ~ GET & SET                            */
bool ADXL345getSpiBit(ADXL345_SPI_I2C* adxl345) {
	return getRegisterBit(adxl345, ADXL345_DATA_FORMAT, 6);
}

// If Set (1) Puts Device in 3-wire Mode
// If Set (0) Puts Device in 4-wire SPI Mode
void ADXL345setSpiBit(ADXL345_SPI_I2C* adxl345, bool spiBit) {
	setRegisterBit(adxl345, ADXL345_DATA_FORMAT, 6, spiBit);
}

/*********************** INT_INVERT BIT STATE ***********************/
/*                           ~ GET & SET                            */
bool ADXL345getInterruptLevelBit(ADXL345_SPI_I2C* adxl345) {
	return getRegisterBit(adxl345, ADXL345_DATA_FORMAT, 5);
}

// If Set (0) Sets the Interrupts to Active HIGH
// If Set (1) Sets the Interrupts to Active LOW
void ADXL345setInterruptLevelBit(ADXL345_SPI_I2C* adxl345, bool interruptLevelBit) {
	setRegisterBit(adxl345, ADXL345_DATA_FORMAT, 5, interruptLevelBit);
}

/************************* FULL_RES BIT STATE ***********************/
/*                           ~ GET & SET                            */
bool ADXL345getFullResBit(ADXL345_SPI_I2C* adxl345) {
	return getRegisterBit(adxl345, ADXL345_DATA_FORMAT, 3);
}

// If Set (1) Device is in Full Resolution Mode: Output Resolution Increase with G Range
//  Set by the Range Bits to Maintain a 4mg/LSB Scale Factor
// If Set (0) Device is in 10-bit Mode: Range Bits Determine Maximum G Range
//  And Scale Factor
void ADXL345setFullResBit(ADXL345_SPI_I2C* adxl345, bool fullResBit) {
	setRegisterBit(adxl345, ADXL345_DATA_FORMAT, 3, fullResBit);
}

/*************************** JUSTIFY BIT STATE **************************/
/*                           ~ GET & SET                            */
bool ADXL345getJustifyBit(ADXL345_SPI_I2C* adxl345) {
	return getRegisterBit(adxl345, ADXL345_DATA_FORMAT, 2);
}

// If Set (1) Selects the Left Justified Mode
// If Set (0) Selects Right Justified Mode with Sign Extension
void ADXL345setJustifyBit(ADXL345_SPI_I2C* adxl345, bool justifyBit) {
	setRegisterBit(adxl345, ADXL345_DATA_FORMAT, 2, justifyBit);
}

/*********************** THRESH_TAP BYTE VALUE **********************/
/*                          ~ SET & GET                             */
// Should Set Between 0 and 255
// Scale Factor is 62.5 mg/LSB
// A Value of 0 May Result in Undesirable Behavior
void ADXL345setTapThreshold(ADXL345_SPI_I2C* adxl345, int tapThreshold) {
	tapThreshold = constrain(adxl345, tapThreshold, 0, 255);
	uint8_t _b = (uint8_t) tapThreshold;
	writeTo(adxl345, ADXL345_THRESH_TAP, _b);
}

static int constrain(ADXL345_SPI_I2C* adxl345, int tapThreshold, int lower, int upper){
	if(tapThreshold > upper)
		return upper;
	else if(tapThreshold < lower)
		return lower;
	else
		return tapThreshold;
}

// Return Value Between 0 and 255
// Scale Factor is 62.5 mg/LSB
int ADXL345getTapThreshold(ADXL345_SPI_I2C* adxl345) {
	uint8_t _b;
	readFrom(adxl345, ADXL345_THRESH_TAP, 1, &_b);
	return (int) _b;
}

/****************** GAIN FOR EACH AXIS IN Gs / COUNT *****************/
/*                           ~ SET & GET                            */
void ADXL345setAxisGains(ADXL345_SPI_I2C* adxl345, float *_gains){
	int i;
	for(i = 0; i < 3; i++){
		adxl345->gains[i] = _gains[i];
	}
}
void ADXL345getAxisGains(ADXL345_SPI_I2C* adxl345, float *_gains){
	int i;
	for(i = 0; i < 3; i++){
		_gains[i] = adxl345->gains[i];
	}
}

/********************* OFSX, OFSY and OFSZ BYTES ********************/
/*                           ~ SET & GET                            */
// OFSX, OFSY and OFSZ: User Offset Adjustments in Twos Complement Format
// Scale Factor of 15.6mg/LSB
void ADXL345setAxisOffset(ADXL345_SPI_I2C* adxl345, int x, int y, int z) {
	writeTo(adxl345, ADXL345_OFSX, (uint8_t) x);
	writeTo(adxl345, ADXL345_OFSY, (uint8_t) y);
	writeTo(adxl345, ADXL345_OFSZ, (uint8_t) z);
}

void ADXL345getAxisOffset(ADXL345_SPI_I2C* adxl345, int* x, int* y, int*z) {
	uint8_t _b;
	readFrom(adxl345, ADXL345_OFSX, 1, &_b);
	*x = (int) _b;
	readFrom(adxl345, ADXL345_OFSY, 1, &_b);
	*y = (int) _b;
	readFrom(adxl345, ADXL345_OFSZ, 1, &_b);
	*z = (int) _b;
}

/****************************** DUR BYTE ****************************/
/*                           ~ SET & GET                            */
// DUR Byte: Contains an Unsigned Time Value Representing the Max Time
//  that an Event must be Above the THRESH_TAP Threshold to qualify
//  as a Tap Event
// The scale factor is 625µs/LSB
// Value of 0 Disables the Tap/Double Tap Funcitons. Max value is 255.
void ADXL345setTapDuration(ADXL345_SPI_I2C* adxl345, int tapDuration) {
	tapDuration = constrain(adxl345, tapDuration,0,255);
	uint8_t _b = (uint8_t) tapDuration;
	writeTo(adxl345, ADXL345_DUR, _b);
}

int ADXL345getTapDuration(ADXL345_SPI_I2C* adxl345) {
	uint8_t _b;
	readFrom(adxl345, ADXL345_DUR, 1, &_b);
	return (int) _b;
}

/************************** LATENT REGISTER *************************/
/*                           ~ SET & GET                            */
// Contains Unsigned Time Value Representing the Wait Time from the Detection
//  of a Tap Event to the Start of the Time Window (defined by the Window
//  Register) during which a possible Second Tap Even can be Detected.
// Scale Factor is 1.25ms/LSB.
// A Value of 0 Disables the Double Tap Function.
// It Accepts a Maximum Value of 255.
void ADXL345setDoubleTapLatency(ADXL345_SPI_I2C* adxl345, int doubleTapLatency) {
	uint8_t _b = (uint8_t) doubleTapLatency;
	writeTo(adxl345, ADXL345_LATENT, _b);
}

int ADXL345getDoubleTapLatency(ADXL345_SPI_I2C* adxl345) {
	uint8_t _b;
	readFrom(adxl345, ADXL345_LATENT, 1, &_b);
	return (int) _b;
}

/************************** WINDOW REGISTER *************************/
/*                           ~ SET & GET                            */
// Contains an Unsigned Time Value Representing the Amount of Time
//  After the Expiration of the Latency Time (determined by Latent register)
//  During which a Second Valid Tape can Begin.
// Scale Factor is 1.25ms/LSB.
// Value of 0 Disables the Double Tap Function.
// It Accepts a Maximum Value of 255.
void ADXL345setDoubleTapWindow(ADXL345_SPI_I2C* adxl345, int doubleTapWindow) {
	doubleTapWindow = constrain(adxl345, doubleTapWindow, 0, 255);
	uint8_t _b = (uint8_t) doubleTapWindow;
	writeTo(adxl345, ADXL345_WINDOW, _b);
}

int ADXL345getDoubleTapWindow(ADXL345_SPI_I2C* adxl345) {
	uint8_t _b;
	readFrom(adxl345, ADXL345_WINDOW, 1, &_b);
	return (int) _b;
}

/*********************** THRESH_ACT REGISTER ************************/
/*                          ~ SET & GET                             */
// Holds the Threshold Value for Detecting Activity.
// Data Format is Unsigned, so the Magnitude of the Activity Event is Compared
//  with the Value is Compared with the Value in the THRESH_ACT Register.
// The Scale Factor is 62.5mg/LSB.
// Value of 0 may Result in Undesirable Behavior if the Activity Interrupt Enabled.
// It Accepts a Maximum Value of 255.
void ADXL345setActivityThreshold(ADXL345_SPI_I2C* adxl345, int activityThreshold) {
	activityThreshold = constrain(adxl345, activityThreshold, 0, 255);
	uint8_t _b = (uint8_t) activityThreshold;
	writeTo(adxl345, ADXL345_THRESH_ACT, _b);
}

// Gets the THRESH_ACT byte
int ADXL345getActivityThreshold(ADXL345_SPI_I2C* adxl345) {
	uint8_t _b;
	readFrom(adxl345, ADXL345_THRESH_ACT, 1, &_b);
	return (int) _b;
}

/********************** THRESH_INACT REGISTER ***********************/
/*                          ~ SET & GET                             */
// Holds the Threshold Value for Detecting Inactivity.
// The Data Format is Unsigned, so the Magnitude of the INactivity Event is
//  Compared with the value in the THRESH_INACT Register.
// Scale Factor is 62.5mg/LSB.
// Value of 0 May Result in Undesirable Behavior if the Inactivity Interrupt Enabled.
// It Accepts a Maximum Value of 255.
void ADXL345setInactivityThreshold(ADXL345_SPI_I2C* adxl345, int inactivityThreshold) {
	inactivityThreshold = constrain(adxl345, inactivityThreshold, 0, 255);
	uint8_t _b = (uint8_t) inactivityThreshold;
	writeTo(adxl345, ADXL345_THRESH_INACT, _b);
}

int ADXL345getInactivityThreshold(ADXL345_SPI_I2C* adxl345) {
	uint8_t _b;
	readFrom(adxl345, ADXL345_THRESH_INACT, 1, &_b);
	return (int) _b;
}

/*********************** TIME_INACT RESIGER *************************/
/*                          ~ SET & GET                             */
// Contains an Unsigned Time Value Representing the Amount of Time that
//  Acceleration must be Less Than the Value in the THRESH_INACT Register
//  for Inactivity to be Declared.
// Uses Filtered Output Data* unlike other Interrupt Functions
// Scale Factor is 1sec/LSB.
// Value Must Be Between 0 and 255.
void ADXL345setTimeInactivity(ADXL345_SPI_I2C* adxl345, int timeInactivity) {
	timeInactivity = constrain(adxl345, timeInactivity, 0, 255);
	uint8_t _b = (uint8_t) timeInactivity;
	writeTo(adxl345, ADXL345_TIME_INACT, _b);
}

int ADXL345getTimeInactivity(ADXL345_SPI_I2C* adxl345) {
	uint8_t _b;
	readFrom(adxl345, ADXL345_TIME_INACT, 1, &_b);
	return (int) _b;
}

/*********************** THRESH_FF Register *************************/
/*                          ~ SET & GET                             */
// Holds the Threshold Value, in Unsigned Format, for Free-Fall Detection
// The Acceleration on all Axes is Compared with the Value in THRES_FF to
//  Determine if a Free-Fall Event Occurred.
// Scale Factor is 62.5mg/LSB.
// Value of 0 May Result in Undesirable Behavior if the Free-Fall interrupt Enabled.
// Accepts a Maximum Value of 255.
void ADXL345setFreeFallThreshold(ADXL345_SPI_I2C* adxl345, int freeFallThreshold) {
	freeFallThreshold = constrain(adxl345, freeFallThreshold, 0, 255);
	uint8_t _b = (uint8_t) freeFallThreshold;
	writeTo(adxl345, ADXL345_THRESH_FF, _b);
}

int ADXL345getFreeFallThreshold(ADXL345_SPI_I2C* adxl345) {
	uint8_t _b;
	readFrom(adxl345, ADXL345_THRESH_FF, 1, &_b);
	return (int) _b;
}

/************************ TIME_FF Register **************************/
/*                          ~ SET & GET                             */
// Stores an Unsigned Time Value Representing the Minimum Time that the Value
//  of all Axes must be Less Than THRES_FF to Generate a Free-Fall Interrupt.
// Scale Factor is 5ms/LSB.
// Value of 0 May Result in Undesirable Behavior if the Free-Fall Interrupt Enabled.
// Accepts a Maximum Value of 255.
void ADXL345setFreeFallDuration(ADXL345_SPI_I2C* adxl345, int freeFallDuration) {
	freeFallDuration = constrain(adxl345, freeFallDuration, 0, 255);
	uint8_t _b = (uint8_t) freeFallDuration;
	writeTo(adxl345, ADXL345_TIME_FF, _b);
}

int ADXL345getFreeFallDuration(ADXL345_SPI_I2C* adxl345) {
	uint8_t _b;
	readFrom(adxl345, ADXL345_TIME_FF, 1, &_b);
	return (int) _b;
}

/************************** ACTIVITY BITS ***************************/
/*                                                                  */
bool ADXL345isActivityXEnabled(ADXL345_SPI_I2C* adxl345) {
	return getRegisterBit(adxl345, ADXL345_ACT_INACT_CTL, 6);
}
bool ADXL345isActivityYEnabled(ADXL345_SPI_I2C* adxl345) {
	return getRegisterBit(adxl345, ADXL345_ACT_INACT_CTL, 5);
}
bool ADXL345isActivityZEnabled(ADXL345_SPI_I2C* adxl345) {
	return getRegisterBit(adxl345, ADXL345_ACT_INACT_CTL, 4);
}
bool ADXL345isInactivityXEnabled(ADXL345_SPI_I2C* adxl345) {
	return getRegisterBit(adxl345, ADXL345_ACT_INACT_CTL, 2);
}
bool ADXL345isInactivityYEnabled(ADXL345_SPI_I2C* adxl345) {
	return getRegisterBit(adxl345, ADXL345_ACT_INACT_CTL, 1);
}
bool ADXL345isInactivityZEnabled(ADXL345_SPI_I2C* adxl345) {
	return getRegisterBit(adxl345, ADXL345_ACT_INACT_CTL, 0);
}

void ADXL345setActivityX(ADXL345_SPI_I2C* adxl345, bool state) {
	setRegisterBit(adxl345, ADXL345_ACT_INACT_CTL, 6, state);
}
void ADXL345setActivityY(ADXL345_SPI_I2C* adxl345, bool state) {
	setRegisterBit(adxl345, ADXL345_ACT_INACT_CTL, 5, state);
}
void ADXL345setActivityZ(ADXL345_SPI_I2C* adxl345, bool state) {
	setRegisterBit(adxl345, ADXL345_ACT_INACT_CTL, 4, state);
}
void ADXL345setActivityXYZ(ADXL345_SPI_I2C* adxl345, bool stateX, bool stateY, bool stateZ) {
	ADXL345setActivityX(adxl345, stateX);
	ADXL345setActivityY(adxl345, stateY);
	ADXL345setActivityZ(adxl345, stateZ);
}
void ADXL345setInactivityX(ADXL345_SPI_I2C* adxl345, bool state) {
	setRegisterBit(adxl345, ADXL345_ACT_INACT_CTL, 2, state);
}
void ADXL345setInactivityY(ADXL345_SPI_I2C* adxl345, bool state) {
	setRegisterBit(adxl345, ADXL345_ACT_INACT_CTL, 1, state);
}
void ADXL345setInactivityZ(ADXL345_SPI_I2C* adxl345, bool state) {
	setRegisterBit(adxl345, ADXL345_ACT_INACT_CTL, 0, state);
}
void ADXL345setInactivityXYZ(ADXL345_SPI_I2C* adxl345, bool stateX, bool stateY, bool stateZ) {
	ADXL345setInactivityX(adxl345, stateX);
	ADXL345setInactivityY(adxl345, stateY);
	ADXL345setInactivityZ(adxl345, stateZ);
}

bool ADXL345isActivityAc(ADXL345_SPI_I2C* adxl345) {
	return getRegisterBit(adxl345, ADXL345_ACT_INACT_CTL, 7);
}
bool ADXL345isInactivityAc(ADXL345_SPI_I2C* adxl345){
	return getRegisterBit(adxl345, ADXL345_ACT_INACT_CTL, 3);
}

void ADXL345setActivityAc(ADXL345_SPI_I2C* adxl345, bool state) {
	setRegisterBit(adxl345, ADXL345_ACT_INACT_CTL, 7, state);
}
void ADXL345setInactivityAc(ADXL345_SPI_I2C* adxl345, bool state) {
	setRegisterBit(adxl345, ADXL345_ACT_INACT_CTL, 3, state);
}

/************************* SUPPRESS BITS ****************************/
/*                                                                  */
bool ADXL345getSuppressBit(ADXL345_SPI_I2C* adxl345){
	return getRegisterBit(adxl345, ADXL345_TAP_AXES, 3);
}
void ADXL345setSuppressBit(ADXL345_SPI_I2C* adxl345, bool state) {
	setRegisterBit(adxl345, ADXL345_TAP_AXES, 3, state);
}

/**************************** TAP BITS ******************************/
/*                                                                  */
bool ADXL345isTapDetectionOnX(ADXL345_SPI_I2C* adxl345){
	return getRegisterBit(adxl345, ADXL345_TAP_AXES, 2);
}
void ADXL345setTapDetectionOnX(ADXL345_SPI_I2C* adxl345, bool state) {
	setRegisterBit(adxl345, ADXL345_TAP_AXES, 2, state);
}
bool ADXL345isTapDetectionOnY(ADXL345_SPI_I2C* adxl345){
	return getRegisterBit(adxl345, ADXL345_TAP_AXES, 1);
}
void ADXL345setTapDetectionOnY(ADXL345_SPI_I2C* adxl345, bool state) {
	setRegisterBit(adxl345, ADXL345_TAP_AXES, 1, state);
}
bool ADXL345isTapDetectionOnZ(ADXL345_SPI_I2C* adxl345){
	return getRegisterBit(adxl345, ADXL345_TAP_AXES, 0);
}
void ADXL345setTapDetectionOnZ(ADXL345_SPI_I2C* adxl345, bool state) {
	setRegisterBit(adxl345, ADXL345_TAP_AXES, 0, state);
}

void ADXL345setTapDetectionOnXYZ(ADXL345_SPI_I2C* adxl345, bool stateX, bool stateY, bool stateZ) {
	ADXL345setTapDetectionOnX(adxl345, stateX);
	ADXL345setTapDetectionOnY(adxl345, stateY);
	ADXL345setTapDetectionOnZ(adxl345, stateZ);
}

bool ADXL345isActivitySourceOnX(ADXL345_SPI_I2C* adxl345){
	return getRegisterBit(adxl345, ADXL345_ACT_TAP_STATUS, 6);
}
bool ADXL345isActivitySourceOnY(ADXL345_SPI_I2C* adxl345){
	return getRegisterBit(adxl345, ADXL345_ACT_TAP_STATUS, 5);
}
bool ADXL345isActivitySourceOnZ(ADXL345_SPI_I2C* adxl345){
	return getRegisterBit(adxl345, ADXL345_ACT_TAP_STATUS, 4);
}

bool ADXL345isTapSourceOnX(ADXL345_SPI_I2C* adxl345){
	return getRegisterBit(adxl345, ADXL345_ACT_TAP_STATUS, 2);
}
bool ADXL345isTapSourceOnY(ADXL345_SPI_I2C* adxl345){
	return getRegisterBit(adxl345, ADXL345_ACT_TAP_STATUS, 1);
}
bool ADXL345isTapSourceOnZ(ADXL345_SPI_I2C* adxl345){
	return getRegisterBit(adxl345, ADXL345_ACT_TAP_STATUS, 0);
}

/*************************** ASLEEP BIT *****************************/
/*                                                                  */
bool ADXL345isAsleep(ADXL345_SPI_I2C* adxl345){
	return getRegisterBit(adxl345, ADXL345_ACT_TAP_STATUS, 3);
}

/************************** LOW POWER BIT ***************************/
/*                                                                  */
bool ADXL345isLowPower(ADXL345_SPI_I2C* adxl345){
	return getRegisterBit(adxl345, ADXL345_BW_RATE, 4);
}
void ADXL345setLowPower(ADXL345_SPI_I2C* adxl345, bool state) {
	setRegisterBit(adxl345, ADXL345_BW_RATE, 4, state);
}

/*************************** RATE BITS ******************************/
/*                                                                  */
double ADXL345getRate(ADXL345_SPI_I2C* adxl345){
	uint8_t _b;
	readFrom(adxl345, ADXL345_BW_RATE, 1, &_b);
	_b &= 0b00001111;
	return (pow(2,((int) _b)-6)) * 6.25;
}

void ADXL345setRate(ADXL345_SPI_I2C* adxl345, float rate){
	uint8_t _b,_s;
	int v = (int) (rate / 6.25);
	int r = 0;
	while (v >>= 1)
	{
		r++;
	}
	if (r <= 9) {
		readFrom(adxl345, ADXL345_BW_RATE, 1, &_b);
		_s = (uint8_t) (r + 6) | (_b & 0b11110000);
		writeTo(adxl345, ADXL345_BW_RATE, _s);
	}
}

/*************************** BANDWIDTH ******************************/
/*                          ~ SET & GET                             */
void ADXL345set_bw(ADXL345_SPI_I2C* adxl345, uint8_t bw_code){
	if((bw_code < ADXL345_BW_0_05) || (bw_code > ADXL345_BW_1600)){
		adxl345->status = false;
		adxl345->error_code = ADXL345_BAD_ARG;
	}
	else{
		writeTo(adxl345, ADXL345_BW_RATE, bw_code);
	}
}

uint8_t ADXL345get_bw_code(ADXL345_SPI_I2C* adxl345){
	uint8_t bw_code;
	readFrom(adxl345, ADXL345_BW_RATE, 1, &bw_code);
	return bw_code;
}




/************************* TRIGGER CHECK  ***************************/
/*                                                                  */
// Check if Action was Triggered in Interrupts
// Example triggered(interrupts, ADXL345_SINGLE_TAP);
bool ADXL345triggered(ADXL345_SPI_I2C* adxl345, uint8_t interrupts, int mask){
	return ((interrupts >> mask) & 1);
}

/*
 ADXL345_DATA_READY
 ADXL345_SINGLE_TAP
 ADXL345_DOUBLE_TAP
 ADXL345_ACTIVITY
 ADXL345_INACTIVITY
 ADXL345_FREE_FALL
 ADXL345_WATERMARK
 ADXL345_OVERRUNY
 */


uint8_t ADXL345getInterruptSource(ADXL345_SPI_I2C* adxl345) {
	uint8_t _b;
	readFrom(adxl345, ADXL345_INT_SOURCE, 1, &_b);
	return _b;
}

bool ADXL345getInterruptSource_interrupt(ADXL345_SPI_I2C* adxl345, uint8_t interruptBit) {
	return getRegisterBit(adxl345, ADXL345_INT_SOURCE,interruptBit);
}

bool ADXL345getInterruptMapping(ADXL345_SPI_I2C* adxl345, uint8_t interruptBit) {
	return getRegisterBit(adxl345, ADXL345_INT_MAP,interruptBit);
}

/*********************** INTERRUPT MAPPING **************************/
/*         Set the Mapping of an Interrupt to pin1 or pin2          */
// eg: setInterruptMapping(ADXL345_INT_DOUBLE_TAP_BIT,ADXL345_INT2_PIN);
void ADXL345setInterruptMapping(ADXL345_SPI_I2C* adxl345, uint8_t interruptBit, bool interruptPin) {
	setRegisterBit(adxl345, ADXL345_INT_MAP, interruptBit, interruptPin);
}

void ADXL345setImportantInterruptMapping(ADXL345_SPI_I2C* adxl34, int single_tap, int double_tap, int free_fall, int activity, int inactivity) {
	if(single_tap == 1) {
		ADXL345setInterruptMapping(adxl34, ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );}
	else if(single_tap == 2) {
		ADXL345setInterruptMapping(adxl34, ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT2_PIN );}

	if(double_tap == 1) {
		ADXL345setInterruptMapping(adxl34, ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );}
	else if(double_tap == 2) {
		ADXL345setInterruptMapping(adxl34, ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT2_PIN );}

	if(free_fall == 1) {
		ADXL345setInterruptMapping(adxl34, ADXL345_INT_FREE_FALL_BIT,   ADXL345_INT1_PIN );}
	else if(free_fall == 2) {
		ADXL345setInterruptMapping(adxl34, ADXL345_INT_FREE_FALL_BIT,   ADXL345_INT2_PIN );}

	if(activity == 1) {
		ADXL345setInterruptMapping(adxl34, ADXL345_INT_ACTIVITY_BIT,   ADXL345_INT1_PIN );}
	else if(activity == 2) {
		ADXL345setInterruptMapping(adxl34, ADXL345_INT_ACTIVITY_BIT,   ADXL345_INT2_PIN );}

	if(inactivity == 1) {
		ADXL345setInterruptMapping(adxl34, ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );}
	else if(inactivity == 2) {
		ADXL345setInterruptMapping(adxl34, ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT2_PIN );}
}

bool ADXL345isInterruptEnabled(ADXL345_SPI_I2C* adxl34, uint8_t interruptBit) {
	return getRegisterBit(adxl34, ADXL345_INT_ENABLE,interruptBit);
}

void ADXL345setInterrupt(ADXL345_SPI_I2C* adxl34, uint8_t interruptBit, bool state) {
	setRegisterBit(adxl34, ADXL345_INT_ENABLE, interruptBit, state);
}

void ADXL345singleTapINT(ADXL345_SPI_I2C* adxl34, bool status) {
	if(status) {
		ADXL345setInterrupt(adxl34, ADXL345_INT_SINGLE_TAP_BIT, 1);
	}
	else {
		ADXL345setInterrupt(adxl34, ADXL345_INT_SINGLE_TAP_BIT, 0);
	}
}
void ADXL345doubleTapINT(ADXL345_SPI_I2C* adxl34, bool status) {
	if(status) {
		ADXL345setInterrupt(adxl34, ADXL345_INT_DOUBLE_TAP_BIT, 1);
	}
	else {
		ADXL345setInterrupt(adxl34, ADXL345_INT_DOUBLE_TAP_BIT, 0);
	}
}
void ADXL345FreeFallINT(ADXL345_SPI_I2C* adxl34, bool status) {
	if(status) {
		ADXL345setInterrupt(adxl34, ADXL345_INT_FREE_FALL_BIT,  1);
	}
	else {
		ADXL345setInterrupt(adxl34, ADXL345_INT_FREE_FALL_BIT,  0);
	}
}
void ADXL345ActivityINT(ADXL345_SPI_I2C* adxl34, bool status) {
	if(status) {
		ADXL345setInterrupt(adxl34, ADXL345_INT_ACTIVITY_BIT,   1);
	}
	else {
		ADXL345setInterrupt(adxl34, ADXL345_INT_ACTIVITY_BIT,   0);
	}
}
void ADXL345InactivityINT(ADXL345_SPI_I2C* adxl34, bool status) {
	if(status) {
		ADXL345setInterrupt(adxl34, ADXL345_INT_INACTIVITY_BIT, 1);
	}
	else {
		ADXL345setInterrupt(adxl34, ADXL345_INT_INACTIVITY_BIT, 0);
	}
}

static void setRegisterBit(ADXL345_SPI_I2C* adxl34, uint8_t regAdress, int bitPos, bool state) {
	uint8_t _b;
	readFrom(adxl34, regAdress, 1, &_b);
	if (state) {
		_b |= (1 << bitPos);  // Forces nth Bit of _b to 1. Other Bits Unchanged.
	}
	else {
		_b &= ~(1 << bitPos); // Forces nth Bit of _b to 0. Other Bits Unchanged.
	}
	writeTo(adxl34, regAdress, _b);
}

static bool getRegisterBit(ADXL345_SPI_I2C* adxl34, uint8_t regAdress, int bitPos) {
	uint8_t _b;
	readFrom(adxl34, regAdress, 1, &_b);
	return ((_b >> bitPos) & 1);
}


