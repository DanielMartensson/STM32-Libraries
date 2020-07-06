This is a simple library for writing and reading to the flash memory. This is NOT an EEPROM library, but it works as the same. 

Program example:

```
// Include
#include "FlashMemory/ReadWrite.h"

// Set the sector and the address - They can be found in the data sheet of your processor. Look at chapter 3.3 at document RM0390 for STM32F446XX processors
FlashSetSectorAddrs(7, 0x08060000); // We take the last sector for the main memory and begin to write at the first address

// Write
float wrBuf[7] = {value1, value2, pressure_value, speed, setpoint, horizon, forgetting};
FlashEraseSector(); // Must clear first
FlashWriteN(0, wrBuf, 7, DATA_TYPE_FLOAT); // Begin to write at index 0 and write 7 float values

// Read 
float rdBuf[7];
FlashReadN(0, rdBuf, 7, DATA_TYPE_FLOAT); // Begin at index 0 and read 7 float values
float value1 = rdBuf[0];
float value2 = rdBuf[1];
float pressure_value = rdBuf[2];
float speed = rdBuf[3];
float setpoint = rdBuf[4];
float horizon = rdBuf[5];
float forgetting = rdBuf[6];
```
