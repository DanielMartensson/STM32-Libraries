  MCP4728 is a 12-bit DAC. It's working. Notice that you need to use LDAC pin too. Connect it to your STM32 GPIO and name it `LDAC` only
  
  ```c
  
  /* Include */
  #include "MCP4728/mcp4728.h"
  
  /* Handler */
  I2C_HandleTypeDef hi2c1;
  
  /* Write to MCP4728 */
  mcp4728_init(&hi2c1, 0x4); // A4 for A2 = 1, A1 = 0, A0 = 0
  mcp4728_single_write_command(MCP4728_CHANNEL_A, MCP4728_VREF_VDD, MCP4728_POWER_DOWN_NORMAL, MCP4728_GAIN_1, 1000);
  mcp4728_single_write_command(MCP4728_CHANNEL_B, MCP4728_VREF_VDD, MCP4728_POWER_DOWN_NORMAL, MCP4728_GAIN_1, 2000);
  mcp4728_single_write_command(MCP4728_CHANNEL_C, MCP4728_VREF_VDD, MCP4728_POWER_DOWN_NORMAL, MCP4728_GAIN_1, 3000);
  mcp4728_single_write_command(MCP4728_CHANNEL_D, MCP4728_VREF_VDD, MCP4728_POWER_DOWN_NORMAL, MCP4728_GAIN_1, 4095);
  ```

