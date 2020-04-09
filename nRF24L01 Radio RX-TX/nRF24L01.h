/*
 * nRF24L01.h
 *
 *  Created on: Apr 9, 2020
 *      Author: Mohamed Yaqoob, Rewritten by Daniel Mårtensson
 */

#ifndef SRC_NRF24L01_NRF24L01_H_
#define SRC_NRF24L01_NRF24L01_H_

#include "main.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

// Functions
#define MAX_nRF24L01(x, y) (((x) > (y)) ? (x) : (y))
#define MIN_nRF24L01(x, y) (((x) < (y)) ? (x) : (y))
#define BOOL_nRF24L01(x) (((x)>0) ? 1:0)
#define BV_nRF24L01(x) (1<<(x))

/* Memory Map */
#define REG_CONFIG      0x00
#define REG_EN_AA       0x01
#define REG_EN_RXADDR   0x02
#define REG_SETUP_AW    0x03
#define REG_SETUP_RETR  0x04
#define REG_RF_CH       0x05
#define REG_RF_SETUP    0x06
#define REG_STATUS      0x07
#define REG_OBSERVE_TX  0x08
#define REG_CD          0x09
#define REG_RX_ADDR_P0  0x0A
#define REG_RX_ADDR_P1  0x0B
#define REG_RX_ADDR_P2  0x0C
#define REG_RX_ADDR_P3  0x0D
#define REG_RX_ADDR_P4  0x0E
#define REG_RX_ADDR_P5  0x0F
#define REG_TX_ADDR     0x10
#define REG_RX_PW_P0    0x11
#define REG_RX_PW_P1    0x12
#define REG_RX_PW_P2    0x13
#define REG_RX_PW_P3    0x14
#define REG_RX_PW_P4    0x15
#define REG_RX_PW_P5    0x16
#define REG_FIFO_STATUS 0x17
#define REG_DYNPD	    	0x1C
#define REG_FEATURE	    0x1D

/* Bit Mnemonics */
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define BIT_EN_CRC      3
#define BIT_CRCO        2
#define BIT_PWR_UP      1
#define BIT_PRIM_RX     0
#define BIT_ENAA_P5     5
#define BIT_ENAA_P4     4
#define BIT_ENAA_P3     3
#define BIT_ENAA_P2     2
#define BIT_ENAA_P1     1
#define BIT_ENAA_P0     0
#define BIT_ERX_P5      5
#define BIT_ERX_P4      4
#define BIT_ERX_P3      3
#define BIT_ERX_P2      2
#define BIT_ERX_P1      1
#define BIT_ERX_P0      0
#define BIT_AW          0
#define BIT_ARD         4
#define BIT_ARC         0
#define BIT_PLL_LOCK    4
#define BIT_RF_DR       3
#define BIT_RF_PWR      6
#define BIT_RX_DR       6
#define BIT_TX_DS       5
#define BIT_MAX_RT      4
#define BIT_RX_P_NO     1
#define BIT_TX_FULL     0
#define BIT_PLOS_CNT    4
#define BIT_ARC_CNT     0
#define BIT_TX_REUSE    6
#define BIT_FIFO_FULL   5
#define BIT_TX_EMPTY    4
#define BIT_RX_FULL     1
#define BIT_RX_EMPTY    0
#define BIT_DPL_P5	    5
#define BIT_DPL_P4	    4
#define BIT_DPL_P3	    3
#define BIT_DPL_P2	    2
#define BIT_DPL_P1	    1
#define BIT_DPL_P0	    0
#define BIT_EN_DPL	    2
#define BIT_EN_ACK_PAY  1
#define BIT_EN_DYN_ACK  0

/* Instruction Mnemonics */
#define CMD_R_REGISTER    0x00
#define CMD_W_REGISTER    0x20
#define CMD_REGISTER_MASK 0x1F
#define CMD_ACTIVATE      0x50
#define CMD_R_RX_PL_WID   0x60
#define CMD_R_RX_PAYLOAD  0x61
#define CMD_W_TX_PAYLOAD  0xA0
#define CMD_W_ACK_PAYLOAD 0xA8
#define CMD_FLUSH_TX      0xE1
#define CMD_FLUSH_RX      0xE2
#define CMD_REUSE_TX_PL   0xE3
#define CMD_NOP           0xFF

/* Non-P omissions */
#define LNA_HCURR   0

/* P model memory Map */
#define REG_RPD         0x09

/* P model bit Mnemonics */
#define RF_DR_LOW   5
#define RF_DR_HIGH  3
#define RF_PWR_LOW  1
#define RF_PWR_HIGH 2

//**** TypeDefs ****//
//0. Object structure
typedef struct{
	//*** Library variables ***//
	uint64_t pipe0_reading_address;
	bool ack_payload_available; /**< Whether there is an ack payload waiting */
	uint8_t ack_payload_length; /**< Dynamic size of pending ack payload. */
	uint8_t payload_size; /**< Fixed size of payloads */
	bool dynamic_payloads_enabled; /**< Whether dynamic payloads are enabled. */
	bool p_variant; /* False for RF24L01 and true for RF24L01P */
	bool wide_band; /* 2Mbs data rate in use? */

	//*** NRF24L01 pins and handles ***//
	//CE and CSN pins
	GPIO_TypeDef		*nrf24_PORT;
	uint16_t			nrf24_CSN_PIN;
	uint16_t			nrf24_CE_PIN;
	//SPI handle
	SPI_HandleTypeDef* nrf24_hspi;
	//Debugging UART handle
	UART_HandleTypeDef* nrf24_huart;
}NRF24L01_SPI;

//1. Power Amplifier function, NRF24_setPALevel()
typedef enum {
	RF24_PA_m18dB = 0,
	RF24_PA_m12dB,
	RF24_PA_m6dB,
	RF24_PA_0dB,
	RF24_PA_ERROR
}rf24_pa_dbm_e ;

//2. NRF24_setDataRate() input
typedef enum {
	RF24_1MBPS = 0,
	RF24_2MBPS,
	RF24_250KBPS
}rf24_datarate_e;

//3. NRF24_setCRCLength() input
typedef enum {
	RF24_CRC_DISABLED = 0,
	RF24_CRC_8,
	RF24_CRC_16
}rf24_crclength_e;

//4. Pipe address registers
static const uint8_t NRF24_ADDR_REGS[7] = {
		REG_RX_ADDR_P0,
		REG_RX_ADDR_P1,
		REG_RX_ADDR_P2,
		REG_RX_ADDR_P3,
		REG_RX_ADDR_P4,
		REG_RX_ADDR_P5,
		REG_TX_ADDR
};

//5. RX_PW_Px registers addresses
static const uint8_t RF24_RX_PW_PIPE[6] = {
		REG_RX_PW_P0,
		REG_RX_PW_P1,
		REG_RX_PW_P2,
		REG_RX_PW_P3,
		REG_RX_PW_P4,
		REG_RX_PW_P5
};

//********** Private Functions **********//
//Microsecond delay function
//void NRF24_DelayMicroSeconds(uint32_t uSec);
//1. Chip Select function
//void NRF24_csn(NRF24L01_SPI* spi, int mode);
//2. Chip Enable
//void NRF24_ce(NRF24L01_SPI* spi, int level);
//3. Read single byte from a register
//uint8_t NRF24_read_register(NRF24L01_SPI* spi, uint8_t reg);
//4. Read multiple bytes register
//void NRF24_read_registerN(NRF24L01_SPI* spi, uint8_t reg, uint8_t *buf, uint8_t len);
//5. Write single byte register
//void NRF24_write_register(NRF24L01_SPI* spi, uint8_t reg, uint8_t value);
//6. Write multipl bytes register
//void NRF24_write_registerN(NRF24L01_SPI* spi, uint8_t reg, const uint8_t* buf, uint8_t len);

//********** Global Functions **********//
//7. Write transmit payload
void NRF24_write_payload(NRF24L01_SPI* spi, const void* buf, uint8_t len);
//8. Read receive payload
void NRF24_read_payload(NRF24L01_SPI* spi, void* buf, uint8_t len);
//9. Flush Tx buffer
void NRF24_flush_tx(NRF24L01_SPI* spi);
//10. Flush Rx buffer
void NRF24_flush_rx(NRF24L01_SPI* spi);
//11. Get status register value
uint8_t NRF24_get_status(NRF24L01_SPI* spi);
//12. Begin function
void NRF24_begin(NRF24L01_SPI* spi, GPIO_TypeDef* nrf24PORT, uint16_t nrfCSN_Pin, uint16_t nrfCE_Pin, SPI_HandleTypeDef* nrfSPI);
//13. Listen on open pipes for reading (Must call NRF24_openReadingPipe() first)
void NRF24_startListening(NRF24L01_SPI* spi);
//14. Stop listening (essential before any write operation)
void NRF24_stopListening(NRF24L01_SPI* spi);
//15. Write(Transmit data), returns true if successfully sent
bool NRF24_write(NRF24L01_SPI* spi, const void* buf, uint8_t len);
//16. Check for available data to read
bool NRF24_available(NRF24L01_SPI* spi);
//17. Read received data
bool NRF24_read(NRF24L01_SPI* spi, void* buf, uint8_t len);
//18. Open Tx pipe for writing (Cannot perform this while Listenning, has to call NRF24_stopListening)
void NRF24_openWritingPipe(NRF24L01_SPI* spi, uint64_t address);
//19. Open reading pipe
void NRF24_openReadingPipe(NRF24L01_SPI* spi, uint8_t number, uint64_t address);
//20 set transmit retries (rf24_Retries_e) and delay
void NRF24_setRetries(NRF24L01_SPI* spi, uint8_t delay, uint8_t count);
//21. Set RF channel frequency
void NRF24_setChannel(NRF24L01_SPI* spi, uint8_t channel);
//22. Set payload size
void NRF24_setPayloadSize(NRF24L01_SPI* spi, uint8_t size);
//23. Get payload size
uint8_t NRF24_getPayloadSize(NRF24L01_SPI* spi);
//24. Get dynamic payload size, of latest packet received
uint8_t NRF24_getDynamicPayloadSize(NRF24L01_SPI* spi);
//25. Enable payload on Ackknowledge packet
void NRF24_enableAckPayload(NRF24L01_SPI* spi);
//26. Enable dynamic payloads
void NRF24_enableDynamicPayloads(NRF24L01_SPI* spi);
void NRF24_disableDynamicPayloads(NRF24L01_SPI* spi);
//27. Check if module is NRF24L01+ or normal module
bool NRF24_isNRF_Plus(NRF24L01_SPI* spi) ;
//28. Set Auto Ack for all
void NRF24_setAutoAck(NRF24L01_SPI* spi, bool enable);
//29. Set Auto Ack for certain pipe
void NRF24_setAutoAckPipe(NRF24L01_SPI* spi, uint8_t pipe, bool enable ) ;
//30. Set transmit power level
void NRF24_setPALevel(NRF24L01_SPI* spi, rf24_pa_dbm_e level ) ;
//31. Get transmit power level
rf24_pa_dbm_e NRF24_getPALevel(NRF24L01_SPI* spi) ;
//32. Set data rate (250 Kbps, 1Mbps, 2Mbps)
bool NRF24_setDataRate(NRF24L01_SPI* spi, rf24_datarate_e speed);
//33. Get data rate
rf24_datarate_e NRF24_getDataRate(NRF24L01_SPI* spi);
//34. Set crc length (disable, 8-bits or 16-bits)
void NRF24_setCRCLength(NRF24L01_SPI* spi, rf24_crclength_e length);
//35. Get CRC length
rf24_crclength_e NRF24_getCRCLength(NRF24L01_SPI* spi);
//36. Disable CRC
void NRF24_disableCRC(NRF24L01_SPI* spi) ;
//37. power up
void NRF24_powerUp(NRF24L01_SPI* spi) ;
//38. power down
void NRF24_powerDown(NRF24L01_SPI* spi);
//39. Check if data are available and on which pipe (Use this for multiple rx pipes)
bool NRF24_availablePipe(NRF24L01_SPI* spi, uint8_t* pipe_num);
//40. Start write (for IRQ mode)
void NRF24_startWrite(NRF24L01_SPI* spi, const void* buf, uint8_t len);
//41. Write acknowledge payload
void NRF24_writeAckPayload(NRF24L01_SPI* spi, uint8_t pipe, const void* buf, uint8_t len);
//42. Check if an Ack payload is available
bool NRF24_isAckPayloadAvailable(NRF24L01_SPI* spi);
//43. Check interrupt flags
void NRF24_whatHappened(NRF24L01_SPI* spi, bool *tx_ok,bool *tx_fail,bool *rx_ready);
//44. Test if there is a carrier on the previous listenning period (useful to check for intereference)
bool NRF24_testCarrier(NRF24L01_SPI* spi);
//45. Test if a signal carrier exists (=> -64dB), only for NRF24L01+
bool NRF24_testRPD(NRF24L01_SPI* spi) ;
//46. Reset Status
void NRF24_resetStatus(NRF24L01_SPI* spi);
//47. ACTIVATE cmd
void NRF24_ACTIVATE_cmd(NRF24L01_SPI* spi);
//48. Get AckPayload Size
uint8_t NRF24_GetAckPayloadSize(NRF24L01_SPI* spi);

//********** Debug Functions **********//
//1. Print radio settings
void printRadioSettings(NRF24L01_SPI* spi);
//2. Print Status
void printStatusReg(NRF24L01_SPI* spi);
//3. Print Config
void printConfigReg(NRF24L01_SPI* spi);
//4. Init Variables
void nrf24_DebugUART_Init(NRF24L01_SPI* spi, UART_HandleTypeDef* nrf24Uart);
//5. FIFO Status
void printFIFOstatus(NRF24L01_SPI* spi);


#endif /* SRC_NRF24L01_NRF24L01_H_ */
