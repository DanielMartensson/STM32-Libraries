# ADS1015 and ADS1115 ADC 16 bit

This is a powerful ADC of 16 bit resolution. It has 4 channels and communicating with STM32 through the I2C bus.
I recommend this due to its high resolution and cheap price. This library follows the same as Adafruit library of the same
ADC.

Program example:

```
  // Include
  #include "ADS1015_ADS1115/ADS1015_ADS1115.h"

  // Structures
  I2C_HandleTypeDef hi2c1;
  ADS1xx5_I2C i2c;

  // Declare the structure where we using GND as address. Look at the top of the header file for addresses.
  ADS1115(&i2c, &hi2c1, ADS_ADDR_GND); // Or ADS1015(&i2c, &hi2c1, ADS_ADDR_GND);
  ADSsetGain(&i2c, GAIN_EIGHT);

  while(){
      // Get ADC values
      int16_t adc0, adc1, adc2, adc3;

      adc0 = ADSreadADC_SingleEnded(&i2c, 0);
      adc1 = ADSreadADC_SingleEnded(&i2c, 1);
      adc2 = ADSreadADC_SingleEnded(&i2c, 2);
      adc3 = ADSreadADC_SingleEnded(&i2c, 3);
  }
```
