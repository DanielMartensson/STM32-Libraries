#include "mcp4728.h"

static I2C_HandleTypeDef *_hi2c;
static uint8_t _address;
#define Timeout 10

#define GERNAL_CALL_COMMAND 0x0
#define GENERAL_CALL_RESET 0x6
#define GENERAL_CALL_WAKE_UP 0x9
#define SINGLE_WRITE_COMMAND 0xB


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
}

void mcp4728_single_write_command(uint8_t channel, uint8_t Vref, uint8_t power_down, uint8_t gain, uint16_t value){
	HAL_GPIO_WritePin(LDAC_GPIO_Port, LDAC_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	const uint8_t UDAC = 0x0;
	uint8_t pData[3] = {(SINGLE_WRITE_COMMAND << 3) | (channel << 1) | UDAC, (Vref << 8) | (power_down << 5) | (gain << 4) | (value >> 8), value};
	HAL_I2C_Master_Transmit(_hi2c, _address << 1, pData, sizeof(pData), Timeout);
	HAL_Delay(10);
	HAL_GPIO_WritePin(LDAC_GPIO_Port, LDAC_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(LDAC_GPIO_Port, LDAC_Pin, GPIO_PIN_SET);
}
