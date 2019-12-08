/**
 * Olivier Van den Eede (4ilo) 2019
 * This library uses the MAX31865 RTD-to-Digital Converter to measure the temperature using a Pt100 or Pt1000 temperature probe.
 *
 * This library is written for Stm32f4 microcontrollers using the stm32-HAL software library.
 * The usage of software SPI in this library is due to issues with reading data using the SPI-HAL functions.
 *
 * Rewritten by Daniel Mårtensson for multiple use of PT100:s
 */
#ifndef MAX31865_LIB_MAX31865_H
#define MAX31865_LIB_MAX31865_H

#include "main.h"

/********************* MAX31865 registers and config bits *********************/
#define MAX31865_READ                   0x00
#define MAX31865_WRITE                  0x80
#define MAX31865_RTDMSB_REG             0x01
#define MAX31865_CONFIG_REG             0x00
#define MAX31865_CONFIG_BIAS            0x80
#define MAX31865_CONFIG_1SHOT           0x20
#define MAX31865_CONFIG_3WIRE           0x10
#define MAX31865_CONFIG_MODEAUTO        0x40


/********************* Constants *********************/
#define MAX31865_RREF 430                        // Reference resistor
#define MAX31865_FACTOR 32768                    // 2^15 used for data to resistance conversion
#define MAX31865_ALPHA 0.003851                  // PT-100 temperature coefficient


/********************* General defines *********************/
#define MAX31865_ON 1
#define MAX31865_OFF 0

/********************* MAX31865_GPIO pinout struct *********************/
typedef struct {
    GPIO_TypeDef* CE_PORT;
    uint16_t CE_PIN;

    GPIO_TypeDef* CLK_PORT;
    uint16_t CLK_PIN;

    GPIO_TypeDef* MOSI_PORT;
    uint16_t MOSI_PIN;

    GPIO_TypeDef* MISO_PORT;
    uint16_t MISO_PIN;
} MAX31865_Soft_SPI;


/********************* Public functions *********************/
float MAX31865_readTemp(MAX31865_Soft_SPI* soft_spi);
void MAX31865_init(MAX31865_Soft_SPI* soft_spi, GPIO_TypeDef* CE_PORT, uint16_t CE_PIN, GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* MOSI_PORT, uint16_t MOSI_PIN, GPIO_TypeDef* MISO_PORT, uint16_t MISO_PIN, uint8_t wires);
#endif //MAX31865_LIB_MAX31865_H
