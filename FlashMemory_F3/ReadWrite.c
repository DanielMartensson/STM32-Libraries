/*
 * ReadWrite.c
 *
 *  Created on: Dec 10, 2020
 *      Author: Daniel Mårtensson
 */

#include "../FlashMemory_F3/ReadWrite.h"

//Private variables
//1. sector start address
static uint32_t memory_address;

// Set memory address
void FlashSetAddrs(uint32_t address) {
	memory_address = address;
}

// Write Flash
void FlashWriteN(uint32_t idx, void *wrBuf, uint32_t Nsize, DataTypeDef dataType) {
	uint32_t flashAddress = memory_address + idx;

	// Converter
	Convert convert;
	uint32_t wrBuf2[1];

	//Unlock Flash
	HAL_FLASH_Unlock();
	//Write to Flash
	switch (dataType) {
	case DATA_TYPE_U64:
		for (uint32_t i = 0; i < Nsize; i++) {
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flashAddress, ((uint64_t*) wrBuf)[i]);
			flashAddress += 8;
		}
		break;

	case DATA_TYPE_U16:
		for (uint32_t i = 0; i < Nsize; i++) {
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, flashAddress, ((uint16_t*) wrBuf)[i]);
			flashAddress += 2;
		}
		break;

	case DATA_TYPE_U32:
		for (uint32_t i = 0; i < Nsize; i++) {
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashAddress, ((uint32_t*) wrBuf)[i]);
			flashAddress += 4;
		}
		break;

	case DATA_TYPE_FLOAT:
		for (uint32_t i = 0; i < Nsize; i++) {
			// Convert float -> uint8_t
			convert.float_value = ((float*) wrBuf)[i];
			// Turn it to uint32_t
			wrBuf2[0] = convert.uint8_values[0] << 24 | convert.uint8_values[1] << 16 | convert.uint8_values[2] << 8 || convert.uint8_values[3];
			// Write the uint32_t byte
			FlashWriteN(idx + 4*i, wrBuf2, 1, DATA_TYPE_U32);
		}
		break;
	}
	//Lock the Flash space
	HAL_FLASH_Lock();
}
//4. Read Flash
void FlashReadN(uint32_t idx, void *rdBuf, uint32_t Nsize, DataTypeDef dataType) {
	uint32_t flashAddress = memory_address + idx;

	// Converter
	Convert convert;
	uint32_t rdBuf2[1];

	switch (dataType) {
	case DATA_TYPE_U64:
		for (uint32_t i = 0; i < Nsize; i++) {
			*((uint64_t*) rdBuf + i) = *(uint64_t*) flashAddress;
			flashAddress += 8;
		}
		break;

	case DATA_TYPE_U16:
		for (uint32_t i = 0; i < Nsize; i++) {
			*((uint16_t*) rdBuf + i) = *(uint16_t*) flashAddress;
			flashAddress += 2;
		}
		break;

	case DATA_TYPE_U32:
		for (uint32_t i = 0; i < Nsize; i++) {
			*((uint32_t*) rdBuf + i) = *(uint32_t*) flashAddress;
			flashAddress += 4;
		}
		break;

	case DATA_TYPE_FLOAT:
		for (uint32_t i = 0; i < Nsize; i++) {
			// Read the uint32_t byte
			FlashReadN(idx + 4 * i, rdBuf2, 1, DATA_TYPE_U32);
			// Convert uint32_t -> float
			convert.uint8_values[0] = rdBuf2[0] >> 24;
			convert.uint8_values[1] = rdBuf2[1] >> 16;
			convert.uint8_values[2] = rdBuf2[2] >> 8;
			convert.uint8_values[3] = rdBuf2[3];
			*((float*) rdBuf + i) = convert.float_value;
		}
		break;
	}
}
