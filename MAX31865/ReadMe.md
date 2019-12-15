# MAX31865 RTD Temperature reader

This library is made for the MAX31865 ADC with Software SPI connection. When this library, you can connect a 2/3/4 wire RTD temperature sensor and read its temperature. 

This library has been tested with Nucleo-F446RE.

Program example:
```
  // Include
  #include "MAX31865/MAX31865.h"
  
  // Structs
  MMAX31865_SPI spi0;
  MAX31865_SPI spi1;
  MAX31865_SPI spi2;
  
  // Select WIRE4 for 4 wire PT100 or WIRE3 for 3 wire PT100 or WIRE2 for 2 wire PT100
  // You can use this for PT1000 as well, just change the reference value in MAX31865.h file
  
  // Temperature sensors
  MAX31865_init(&spi0, GPIOB, GPIO_PIN_14, &hspi2, WIRE4); // 14 = CE0
  MAX31865_init(&spi1, GPIOB, GPIO_PIN_15, &hspi2, WIRE4); // 15 = CE1
  MAX31865_init(&spi2, GPIOB, GPIO_PIN_12, &hspi2, WIRE4); // 12 = CE2
  
  // Print temperature
  float temperature0 = MAX31865_temperature(&spi0);
  float temperature1 = MAX31865_temperature(&spi1);
  float temperature2 = MAX31865_temperature(&spi2);
```
