# MAX31865 RTD Temperature reader

This library is made for the MAX31865 ADC with Software SPI connection. When this library, you can connect a 2/3/4 wire RTD temperature sensor
and read its temperature. The first thing you need to do is change the user configuration and user calibration values in MAX31865.h file.

This library has been tested with Nucleo-F446RE.

Program example:
```
  // Include
  #include "MAX31865/MAX31865.h"
  
  // Structs
  MAX31865_Soft_SPI spi0;
  MAX31865_Soft_SPI spi1;
  MAX31865_Soft_SPI spi2;
  
  // Constructor
  MAX31865_init(&spi0, GPIOB, GPIO_PIN_14, GPIOB, GPIO_PIN_13, GPIOC, GPIO_PIN_1, GPIOC, GPIO_PIN_2, WIRE4);
	MAX31865_init(&spi1, GPIOB, GPIO_PIN_15, GPIOB, GPIO_PIN_13, GPIOC, GPIO_PIN_1, GPIOC, GPIO_PIN_2, WIRE4);
	MAX31865_init(&spi2, GPIOB, GPIO_PIN_12, GPIOB, GPIO_PIN_13, GPIOC, GPIO_PIN_1, GPIOC, GPIO_PIN_2, WIRE4);
  
  // Print temperature
  float temperature0 = MAX31865_readTemp(&spi0);
  float temperature1 = MAX31865_readTemp(&spi1);
  float temperature2 = MAX31865_readTemp(&spi2);
```
