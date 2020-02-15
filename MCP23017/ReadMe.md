# MCP23017 IO-Expander 16-bit
This is a very powerfull IO-expander that can handle interrupts, inputs and digital outputs. This library follows the
same command for the Arduino library. Use this if you have few I/O-pins left or you don't want to waste pins on a LCD 
or something. 

Program example:

```
  // Include
  #include "MCP23017/MCP23017.h"

  // Structs
  I2C_HandleTypeDef hi2c1;
  MCP23017_I2C mcp_spi;

  // Init
  MCP23017begin(&mcp_spi, &hi2c1, 0x00); // Look at the functions comment. There is an address table there for you.
  MCP23017pinMode(&mcp_spi, 0, MCP23017_OUTPUT);
  
  // Set to high
  MCP23017digitalWrite(&mcp_spi, 0, MCP23017_HIGH); // GPA0 = Pin ID 0
```

Physial Pin #| Pin Name | Pin ID
----|------|------------------------------
21 | GPA0 | 0
22 | GPA1 | 1
23 | GPA2 | 2
24 | GPA3 | 3
25 | GPA4 | 4
26 | GPA5 | 5
27 | GPA6 | 6
28 | GPA7 | 7
1 | GPB0 |  8
2 | GPB1 |  9
3 | GPB2 | 10
4 | GPB3 | 11
5 | GPB4 | 12
6 | GPB5 | 13
7 | GPB6 | 14
8 | GPB7 | 15
