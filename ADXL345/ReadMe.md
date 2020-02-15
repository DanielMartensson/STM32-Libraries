# ADXL345 Accelerometer 10-Bit
This is an accelerometer, named ADXL345. With this you can measure the acceleration i x, y and 
z-axis with resolution 10-bit. ADXL345 have both I2C and SPI communication.

Program example:

```
// Include
  #include "ADXL345/ADXL345.h"

  // Structure
  ADXL345_SPI_I2C adxl;

  ADXL345_I2C(&adxl, &hi2c1, 0x53);			        // Declare object with address - I2C
	ADXL345_SPI(&adxl, &hspi3, GPIOA, GPIO_PIN_8);// Declare object with CS - SPI

	ADXL345powerOn(&adxl);                        // Power on the ADXL345
	
	ADXL345setRangeSetting(&adxl, 16);            // Give the range settings
	                                              // Accepted values are 2g, 4g, 8g or 16g
	                                              // Higher Values = Wider Measurement Range
	                                              // Lower Values = Greater Sensitivity

	ADXL345setSpiBit(&adxl, 0);                   // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
	                                              // Default: Set to 1
	                                              // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library

	ADXL345setActivityXYZ(&adxl, 1, 0, 0);        // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
	ADXL345setActivityThreshold(&adxl, 75);       // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)

	ADXL345setInactivityXYZ(&adxl, 1, 0, 0);      // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
	ADXL345setInactivityThreshold(&adxl, 75);     // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
	ADXL345setTimeInactivity(&adxl, 10);          // How many seconds of no activity is inactive?

	ADXL345setTapDetectionOnXYZ(&adxl, 0, 0, 1);  // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)

	// Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
	ADXL345setTapThreshold(&adxl, 50);            // 62.5 mg per increment
	ADXL345setTapDuration(&adxl, 15);             // 625 μs per increment
	ADXL345setDoubleTapLatency(&adxl, 80);        // 1.25 ms per increment
	ADXL345setDoubleTapWindow(&adxl, 200);        // 1.25 ms per increment

	  // Set values for what is considered FREE FALL (0-255)
	ADXL345setFreeFallThreshold(&adxl, 7);        // (5 - 9) recommended - 62.5mg per increment
	ADXL345setFreeFallDuration(&adxl, 30);        // (20 - 70) recommended - 5ms per increment

	ADXL345InactivityINT(&adxl, 1);
	ADXL345ActivityINT(&adxl, 1);
	ADXL345FreeFallINT(&adxl, 1);
	ADXL345doubleTapINT(&adxl, 1);
	ADXL345singleTapINT(&adxl, 1);
  
  while(1){
    // Read the accelerometer values and store them in variables declared above x,y,z
    int x,y,z;
		ADXL345readAccel_x_y_z(&adxl, &x, &y, &z);    
    
  }
