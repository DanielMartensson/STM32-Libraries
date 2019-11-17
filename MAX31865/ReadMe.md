# MAX31865 RTD Temperature reader

This library is made for the MAX31865 ADC with SPI connection. When this library, you can connect a 2/3/4 wire RTD temperature sensor
and read its temperature. The first thing you need to do is change the user configuration and user calibration values in MAX31865.h file.

This library has been tested with Nucleo-F446RE.

Program example:
```
  // Include
  #include "MAX31865/MAX31865.h"
  
  // Structs
  SPI spi0;
  SPI_HandleTypeDef hspi2

  // MAX31865 
  /* Configure:
     V_BIAS enabled - TRUE
     Auto-conversion - TRUE
     1-shot disabled - FALSE
     3-wire disabled (4 or 2 wire enabled) - FALSE
     Fault detection:  automatic delay - MAX31865_FAULT_DETECTION_NONE
     Fault status:  auto-clear - TRUE
     50 Hz filter enabled (not 60 Hz) - TRUE
  */

  MAX31865(&spi0, RTD_PT100, GPIO_PIN_12, GPIOB, &hspi2);
  configure(&spi0, TRUE, TRUE, FALSE, FALSE, MAX31865_FAULT_DETECTION_NONE, TRUE, TRUE);

  // Status code
  uint8_t status =  read_all(&spi0);
  
  // Temperature
  float temp =  temperature(&spi0);
  
  // Raw resistanse
  uint16_t Rraw =  raw_resistance(&spi0);
  
  // Measured reistanse
  float R =  resistance(&spi0);
```
