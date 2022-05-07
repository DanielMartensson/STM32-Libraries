/*
 * mcp4728.h
 *
 *  Created on: May 1, 2022
 *      Author: Daniel Mårtensson
 */

#ifndef SRC_MCP4728_MCP4728_H_
#define SRC_MCP4728_MCP4728_H_

#include "main.h"
#include <stdint.h>

#define MCP4728_CHANNEL_A 0x0
#define MCP4728_CHANNEL_B 0x1
#define MCP4728_CHANNEL_C 0x2
#define MCP4728_CHANNEL_D 0x3
#define MCP4728_VREF_VDD 0x0
#define MCP4728_VREF_INTERNAL 0x1
#define MCP4728_POWER_DOWN_NORMAL 0x0
#define MCP4728_POWER_DOWN_1K 0x1
#define MCP4728_POWER_DOWN_100K 0x2
#define MCP4728_POWER_DOWN_500K 0x3
#define MCP4728_GAIN_1 0x0
#define MCP4728_GAIN_2 0x1

void mcp4728_init(I2C_HandleTypeDef *hi2c, uint8_t address);
void mcp4728_set_channel_value(uint8_t channel, uint16_t value);
void mcp4728_set_channel_power_down(uint8_t channel, uint8_t power_down);
void mcp4728_set_channel_gain(uint8_t channel, uint8_t gain);
void mcp4728_set_channel_vref(uint8_t channel, uint8_t vref);
void mcp4728_multi_write_command();
void mcp4728_single_write_command(uint8_t channel);

#endif /* SRC_MCP4728_MCP4728_H_ */
