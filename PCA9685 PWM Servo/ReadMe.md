# PCA9685 PWM Servo driver
This is a library for controlling 5V servos with the PCA9685 IC chip. This IC chip has 16 channels with 12-bit PWM. Works great
for small servos. A good thing with this library is that this library share the same functions as Adafruit's PCA9685 library!

Program example:
```
// Includes 
#include "main.h"
#include "PCA9685 PWM Servo/PCA9685.h"


// Defines
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define SERVOS 1 // How many servos we have

// Structures
I2C_HandleTypeDef hi2c1;

// Functions
void setServoPulse(uint8_t n, float pulse) {

  float pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
  pulselength /= 4096;  // 12 bits of resolution
  pulse *= 1000000;  // convert input seconds to us
  pulse /= pulselength;
  PCA9685_setPWM(&i2c, n, 0, pulse);
}
uint8_t servonum = 0;

int main(void)
{
  // Configure
  PCA9685_begin(&i2c, &hi2c1, 0x80, 0);
  PCA9685_setOscillatorFrequency(&i2c, 27000000);
  PCA9685_setPWMFreq(&i2c, SERVO_FREQ);  // Analog servos run at ~50 Hz updates

    while (1)
    {
      	// Drive each servo one at a time using setPWM()
    	  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    	    PCA9685_setPWM(&i2c, servonum, 0, pulselen);
    	  }

    	  HAL_Delay(500);
    	  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    		  PCA9685_setPWM(&i2c, servonum, 0, pulselen);
    	  }

    	  HAL_Delay(500);

    	  // Drive each servo one at a time using writeMicroseconds(), it's not precise due to calculation rounding!
    	  // The writeMicroseconds() function is used to mimic the Arduino Servo library writeMicroseconds() behavior.
    	  for (uint16_t microsec = USMIN; microsec < USMAX; microsec++) {
    		  PCA9685_writeMicroseconds(&i2c, servonum, microsec);
    	  }

    	  HAL_Delay(500);
    	  for (uint16_t microsec = USMAX; microsec > USMIN; microsec--) {
    		  PCA9685_writeMicroseconds(&i2c, servonum, microsec);
    	  }

    	  HAL_Delay(500);

    	  servonum++;
    	  if (servonum >= SERVOS)
    		  servonum = 0; // Testing the first SERVOS servo channels
  }
}
```
