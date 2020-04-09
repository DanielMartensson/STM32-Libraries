# nRF24L01 / nRF24L01+

This is a very popular and robust radio and transmitter and reciever. This can be used with STM32 boards and it works fine.
I don't know what to say any more, rather than it can send data by using SPI channel. 
I have rewritten this from ![MYaqoobEmbedded](https://github.com/MYaqoobEmbedded/STM32-Tutorials/tree/master/Tutorial%2024%20-%20NRF24L01%20Radio%20Transceiver)
so it can be used with same SPI line. I have't change anything in his logic, only made it possible for using structs as objects.

Program example:

```
  // Includes
  #include "nRF24L01/nRF24L01.h"
  
  // Handles 
  SPI_HandleTypeDef hspi2;
  UART_HandleTypeDef huart2;
  NRF24L01_SPI spi_rx;
  NRF24L01_SPI spi_tx;
  
  // Global variables
  uint64_t pipeAddress = 0x11223344AA;
  char myRxData[50];
  char myAckPayload[32] = "Ack by STM32 Daniel!";
  char myTxData[32] = "Hello World To You!";
  char AckPayload[32];
  
  /* Do setup for RX */
	NRF24_begin(&spi_rx, GPIOA, CSN_RX_Pin, CE_RX_Pin, &hspi2); // Notice that CSN_RX and CE_RX need to have the same port
	nrf24_DebugUART_Init(&spi_rx, &huart2);

	// Print radio settings for RX
	printRadioSettings(&spi_rx);
	NRF24_setAutoAck(&spi_rx, true);
	NRF24_setChannel(&spi_rx, 52);
	NRF24_setPayloadSize(&spi_rx, 32);
	NRF24_openReadingPipe(&spi_rx, 1, pipeAddress);
	NRF24_enableDynamicPayloads(&spi_rx);
	NRF24_enableAckPayload(&spi_rx);
	NRF24_startListening(&spi_rx);

	/* Do setup for TX */
	NRF24_begin(&spi_tx, GPIOA, CSN_TX_Pin, CE_TX_Pin, &hspi2);
	nrf24_DebugUART_Init(&spi_tx, &huart2);

	// Print radio settings for TX
	printRadioSettings(&spi_tx);

	/* Do setup for TX */
	NRF24_stopListening(&spi_tx);
	NRF24_openWritingPipe(&spi_tx, pipeAddress);
	NRF24_setAutoAck(&spi_tx, true);
	NRF24_setChannel(&spi_tx, 52);
	NRF24_setPayloadSize(&spi_tx, 32);
	NRF24_enableDynamicPayloads(&spi_tx);
	NRF24_enableAckPayload(&spi_tx);
  
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		//**** TRANSMIT ****//
		if(NRF24_write(&spi_tx, myTxData, 32)){
			NRF24_read(&spi_tx, AckPayload, 32);
			HAL_UART_Transmit(&huart2, (uint8_t *)"Transmitted Successfully\r\n", strlen("Transmitted Successfully\r\n"), 10);

			char myDataack[80];
			sprintf(myDataack, "AckPayload:  %s \r\n", AckPayload);
			HAL_UART_Transmit(&huart2, (uint8_t *)myDataack, strlen(myDataack), 10);
		}

		//**** RECEIVE ****//
		if(NRF24_available(&spi_rx)){
			NRF24_read(&spi_rx, myRxData, 32);
			NRF24_writeAckPayload(&spi_rx, 1, myAckPayload, 32);
			myRxData[32] = '\r';
			myRxData[32+1] = '\n';
			HAL_UART_Transmit(&huart2, (uint8_t *)myRxData, 32+2, 10);
		}

		HAL_Delay(1000); // 1 second

	}
  
```

