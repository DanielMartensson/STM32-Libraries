This is a simple library for writing and reading to the flash memory for STM32 F3-series. This is NOT an EEPROM library, but it works as the same. 

Program example:

```
// Include
#include "FlashMemory_F3/ReadWrite.h"

// Set the sector and the address - They can be found in the data sheet of your processor. Look at chapter 3.2.1 at document RM0313 for STM32F37XXX processors
FlasheSetAddrs(0x0803F800); // We take the last sector for the main memory and begin to write at the first address

// Write
float wrBuf[7] = {value1, value2, pressure_value, speed, setpoint, horizon, forgetting};
FlashWriteN(0, wrBuf, 7, DATA_TYPE_FLOAT); // Begin to write at index 0 and write 7 float values

// Read 
uint16_t TIM_PRESCALES[4] = {0};
uint32_t Nsize = sizeof(TIM_PRESCALES)/sizeof(TIM_PRESCALES[0]);
FlashReadN(0, TIM_PRESCALES, Nsize, DATA_TYPE_U16);
```
