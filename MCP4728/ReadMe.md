  MCP4728 is a 12-bit DAC. It's working. Notice that you need to use LDAC pin too. Connect it to your STM32 GPIO and name it `LDAC` only or connect it to `GND`.
  Don't forget pull-up resistors for the I2C SDA and SCL. You can activate internal pull-ups in the STM32 processor for I2C.
  
  ```c
  
  /* Include */
  #include "MCP4728/mcp4728.h"
  
  /* Handler */
  I2C_HandleTypeDef hi2c1;
  
  /* Write to MCP4728 */
  mcp4728_init(&hi2c1, 0x4); // Manufacturer I2C settings: A4 (A2 = 1, A1 = 0, A0 = 0)

  /* Set values */
  mcp4728_set_channel_value(MCP4728_CHANNEL_A, 4095);
  mcp4728_set_channel_value(MCP4728_CHANNEL_B, 1000);
  mcp4728_set_channel_value(MCP4728_CHANNEL_C, 2000);
  mcp4728_set_channel_value(MCP4728_CHANNEL_D, 3000);

  /* Set gain */
  mcp4728_set_channel_gain(MCP4728_CHANNEL_A, MCP4728_GAIN_1);
  mcp4728_set_channel_gain(MCP4728_CHANNEL_B, MCP4728_GAIN_1);
  mcp4728_set_channel_gain(MCP4728_CHANNEL_C, MCP4728_GAIN_1);
  mcp4728_set_channel_gain(MCP4728_CHANNEL_D, MCP4728_GAIN_1);

  /* Set power down */
  mcp4728_set_channel_power_down(MCP4728_CHANNEL_A, MCP4728_POWER_DOWN_NORMAL);
  mcp4728_set_channel_power_down(MCP4728_CHANNEL_B, MCP4728_POWER_DOWN_NORMAL);
  mcp4728_set_channel_power_down(MCP4728_CHANNEL_C, MCP4728_POWER_DOWN_NORMAL);
  mcp4728_set_channel_power_down(MCP4728_CHANNEL_D, MCP4728_POWER_DOWN_NORMAL);

  /* Set voltage reference */
  mcp4728_set_channel_vref(MCP4728_CHANNEL_A, MCP4728_VREF_VDD);
  mcp4728_set_channel_vref(MCP4728_CHANNEL_B, MCP4728_VREF_VDD);
  mcp4728_set_channel_vref(MCP4728_CHANNEL_C, MCP4728_VREF_VDD);
  mcp4728_set_channel_vref(MCP4728_CHANNEL_D, MCP4728_VREF_VDD);

  /* Multi write without EEPROM */
  mcp4728_multi_write_command();

  /* Single write with EEPROM */
  mcp4728_set_channel_value(MCP4728_CHANNEL_A, 1000);
  mcp4728_single_write_command(MCP4728_CHANNEL_A);
  ```

