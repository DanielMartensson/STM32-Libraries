#include "mcp4728.h"

static I2C_HandleTypeDef *_hi2c;
static uint8_t _address;
#define Timeout 10

#define GERNAL_CALL_COMMAND 0x0
#define GENERAL_CALL_RESET 0x6
#define GENERAL_CALL_WAKE_UP 0x9
#define SINGLE_WRITE_COMMAND 0xB
#define MULTI_WRITE_COMMAND 0x8

static uint16_t channel_value[4] = {0};
static uint8_t channel_power_down[4] = {0};
static uint8_t channel_gain[4] = {0};
static uint8_t channel_vref[4] = {0};

void static transmit_reset(){
	uint8_t pData[1] = {GENERAL_CALL_RESET};
	HAL_I2C_Master_Transmit(_hi2c, GERNAL_CALL_COMMAND, pData, sizeof(pData), Timeout);
}

void static transmit_wake_up(){
	uint8_t pData[1] = {GENERAL_CALL_WAKE_UP};
	HAL_I2C_Master_Transmit(_hi2c, GERNAL_CALL_COMMAND, pData, sizeof(pData), Timeout);
}

void mcp4728_init(I2C_HandleTypeDef *hi2c, uint8_t address){
	_hi2c = hi2c;
	_address = (0xC << 3) | address;
	transmit_reset();
	transmit_wake_up();
	HAL_GPIO_WritePin(LDAC_GPIO_Port, LDAC_Pin, GPIO_PIN_RESET);
}

void mcp4728_set_channel_value(uint8_t channel, uint16_t value){
	if(channel <= MCP4728_CHANNEL_D)
		channel_value[channel] = value;
}

void mcp4728_set_channel_power_down(uint8_t channel, uint8_t power_down){
	if(channel <= MCP4728_CHANNEL_D)
		channel_power_down[channel] = power_down;
}

void mcp4728_set_channel_gain(uint8_t channel, uint8_t gain){
	if(channel <= MCP4728_CHANNEL_D)
		channel_gain[channel] = gain;
}

void mcp4728_set_channel_vref(uint8_t channel, uint8_t vref){
	if(channel <= MCP4728_CHANNEL_D)
		channel_vref[channel] = vref;
}

void mcp4728_multi_write_command(){
	const uint8_t UDAC = 0x0;
	for(uint8_t channel = MCP4728_CHANNEL_A; channel <= MCP4728_CHANNEL_D; channel++){
		uint8_t multi_write_data[3] = {(MULTI_WRITE_COMMAND << 3) | (channel << 1) | UDAC, (channel_vref[channel] << 7) | (channel_power_down[channel] << 5) | (channel_gain[channel] << 4) | (channel_value[channel] >> 8), channel_value[channel]};
		HAL_I2C_Master_Transmit(_hi2c, _address << 1, multi_write_data, sizeof(multi_write_data), Timeout);
	}
}

void mcp4728_single_write_command(uint8_t channel){
	/* This command should only be called with a time delay about 50 ms due to the EEPROM writing */
	if(channel <= MCP4728_CHANNEL_D){
		const uint8_t UDAC = 0x0;
		uint8_t pData[3] = {(SINGLE_WRITE_COMMAND << 3) | (channel << 1) | UDAC, (channel_vref[channel] << 7) | (channel_power_down[channel] << 5) | (channel_gain[channel] << 4) | (channel_value[channel] >> 8), channel_value[channel]};
		HAL_I2C_Master_Transmit(_hi2c, _address << 1, pData, sizeof(pData), Timeout);
	}
}
