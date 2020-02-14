# MCP3008 8-Channel 10-Bit ADC
This is a 10-Bit ADC with 8 channels. Use this if you want to have a potentiometer or something analog like a joystick. 
This ADC does not fit measurements. Please use another ADC with higher bit resolution e.g ADS1115.

```
  // Include
  #include "MCP3008/MCP3008.h"
  
  // Structs
  MCP3008_SPI spi;
  SPI_HandleTypeDef hspi2;
  
  /*
   * SPI settings:
   * CPHA = 1 Edge
   * Prescaler = 2
   * First bit = MBS first
   * CPOL = Low
   */
  
  // Init the 10-Bit ADC
  MCP3008_init(&spi0, &hspi2, GPIOB, GPIO_PIN_14);
  
  // Read ADC's
  uint16_t adc0 = MCP3008_Read_Channel(&spi, 0); // Channel 0
  uint16_t adc1 = MCP3008_Read_Channel(&spi, 1); // Channel 1
  uint16_t adc2 = MCP3008_Read_Channel(&spi, 2); // Channel 2
  uint16_t adc3 = MCP3008_Read_Channel(&spi, 3); // Channel 3
  uint16_t adc4 = MCP3008_Read_Channel(&spi, 4); // Channel 4
  uint16_t adc5 = MCP3008_Read_Channel(&spi, 5); // Channel 5
  uint16_t adc6 = MCP3008_Read_Channel(&spi, 6); // Channel 6

```
