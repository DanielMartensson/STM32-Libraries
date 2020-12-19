/*
 * ReadWrite.h
 *
 *  Created on: Dec 10, 2020
 *      Author: Daniel Mårtensson
 */

#ifndef SRC_FLASHMEMORY_F3_READWRITE_H_
#define SRC_FLASHMEMORY_F3_READWRITE_H_

#include "main.h"

//Typedefs
//1. data size
typedef enum{
	DATA_TYPE_U64=0,
	DATA_TYPE_U16,
	DATA_TYPE_U32,
	DATA_TYPE_FLOAT
}DataTypeDef;

// Converter for float <--> uint8_t
typedef union{
    float float_value;
    uint8_t uint8_values[sizeof(float)];
}Convert;

// Set memory address
void FlashSetAddrs(uint32_t address);
// Write Flash
void FlashWriteN(uint32_t idx, void *wrBuf, uint32_t Nsize, DataTypeDef dataType);
// Read Flash
void FlashReadN(uint32_t idx, void *rdBuf, uint32_t Nsize, DataTypeDef dataType);

#endif /* SRC_FLASHMEMORY_F3_READWRITE_H_ */
