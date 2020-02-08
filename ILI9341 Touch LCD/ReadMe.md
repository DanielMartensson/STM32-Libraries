# ILI9341 LCD with touch

This is a simple LCD with touch feature. The communication protocol is SPI. The touch is not perfect for precision and speed, but at least you can display an image, press some large buttons and write a emoij. This LCD works perfect for simple use where you don't want physical buttons and instead want every button on your embedded system onto the LCD screen.

I recommend this LCD due to its price.

Program example:

```
  // Includes
  #include "LCD_ILI9341/LCD_ILI9341.h"
  #include "stdio.h"

  // Handles 
  SPI_HandleTypeDef hspi2;
  ILI9341_SPI spi;

  // Initialize the spi object
  ILI9341_Init(&spi, &hspi2, LCD_CS_GPIO_Port, LCD_CS_Pin, LCD_DC_GPIO_Port, LCD_DC_Pin, LCD_RST_GPIO_Port, LCD_RST_Pin, TS_CS_GPIO_Port, TS_CS_Pin);

  // Use the LCD
  ILI9341_setRotation(&spi, 2);
  ILI9341_Fill(&spi, COLOR_NAVY);
  ILI9341_printText(&spi, "hello", 100, 100, COLOR_GREEN, 1, 5);
  ILI9341_drawLine(&spi, 50, 30, 100, 200, COLOR_GREEN);
  ILI9341_drawCircle(&spi, 50, 50, 10, COLOR_RED);

  // Calibrate the touch
  TSC2046_Calibrate(&spi);

  // Try the features
  while (1) {
	TSC2046_GetTouchData(&spi);
	if (spi.myTsData.isPressed == 1) {
		// Draw a point
		ILI9341_fillCircle(&spi, spi.myTsData.X, spi.myTsData.Y, 1, COLOR_RED);
		
		// Display coordinates
		char text[20];
		sprintf(text, "x=%d, y=%d", spi.myTsData.X, spi.myTsData.Y);
		ILI9341_printText(&spi, text, 10, 10, COLOR_GREEN, 1, 2);
		
		// First clean the creen with COLOR_NAVY. Then create a rectangle if we are in a specific area. 
		if (spi.myTsData.X >= 40 && spi.myTsData.X <= 240 && spi.myTsData.Y >= 0 && spi.myTsData.Y <= 50) {
			ILI9341_Fill(&spi, COLOR_NAVY);
			ILI9341_Fill_Rect(&spi, 40, 0, 200, 50, COLOR_BLUE);
		}
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // LED ON when we touch
	} else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // LED OFF when we don't touch
	}
  }
```
