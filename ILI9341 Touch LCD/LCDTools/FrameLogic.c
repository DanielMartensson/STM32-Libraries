/*
 * FrameTouch.c
 *
 *  Created on: May 23, 2020
 *      Author: Daniel Mårtensson
 */

#include "LCDTools.h"

int16_t num_pad_logic(ILI9341_SPI *spi, bool decimalbutton_show, bool minusbutton_show, float *number_value) {
	// Set rotation
	ILI9341_setRotation(spi, 2);

	// Iinitial holdings
	char text[17];
	bool haveDot = false;
	uint8_t k = 0;

	// Display the current value
	sprintf(text, "%f", *number_value); // Float value to text
	ILI9341_printText(spi, text, 15, 20, COLOR_BLACK, COLOR_WHITE, 3);
	HAL_Delay(2000);
	ILI9341_Fill_Rect(spi, 12, 12, 308, 49, COLOR_WHITE); // Clear
	memset(text, 0, sizeof(text));

	// Select numbers
	while (1) {
		TSC2046_GetTouchData(spi);
		uint8_t isPressed = spi->myTsData.isPressed;
		uint16_t X = spi->myTsData.X;
		uint16_t Y = spi->myTsData.Y;
		if (isPressed == true) {
			if (X >= 23 && X <= 63 && Y >= 70 && Y <= 110) {
				// 0
				if (k > 0 && k < 16) {
					text[k] = '0';
					k++;
				}
			} else if (X >= 81 && X <= 121 && Y >= 70 && Y <= 110) {
				// 1
				if(k < 16){
					text[k] = '1';
					k++;
				}
			} else if (X >= 139 && X <= 179 && Y >= 70 && Y <= 110) {
				// 2
				if(k < 16){
					text[k] = '2';
					k++;
				}
			} else if (X >= 197 && X <= 237 && Y >= 70 && Y <= 110) {
				// 3
				if(k < 16){
					text[k] = '3';
					k++;
				}
			} else if (X >= 255 && X <= 295 && Y >= 70 && Y <= 110) {
				// 4
				text[k] = '4';
				k++;
			} else if (X >= 23 && X <= 63 && Y >= 125 && Y <= 165) {
				// 5
				if(k < 16){
					text[k] = '5';
					k++;
				}
			} else if (X >= 81 && X <= 121 && Y >= 125 && Y <= 165) {
				// 6
				if(k < 16){
					text[k] = '6';
					k++;
				}
			} else if (X >= 139 && X <= 179 && Y >= 125 && Y <= 165) {
				// 7
				if(k < 16){
					text[k] = '7';
					k++;
				}
			} else if (X >= 197 && X <= 237 && Y >= 125 && Y <= 165) {
				// 8
				if(k < 16){
					text[k] = '8';
					k++;
				}
			} else if (X >= 255 && X <= 295 && Y >= 125 && Y <= 165) {
				// 9
				if(k < 16){
					text[k] = '9';
					k++;
				}
			} else if (X >= 23 && X <= 63 && Y >= 180 && Y <= 220) {
				// -
				if (minusbutton_show == true && k == 0) {
					text[k] = '-';
					k++;
				}
			} else if (X >= 81 && X <= 121 && Y >= 180 && Y <= 220) {
				// .
				if (decimalbutton_show == true && k > 0 && haveDot == false && k < 16) {
					text[k] = '.';
					k++;
					haveDot = true;
				}
			} else if (X >= 139 && X <= 179 && Y >= 180 && Y <= 220) {
				// C
				memset(text, 0, sizeof(text));
				k = 0;
				haveDot = false;
				ILI9341_Fill_Rect(spi, 12, 12, 308, 49, COLOR_WHITE);
			} else if (X >= 197 && X <= 237 && Y >= 180 && Y <= 220) {
				// <-
				return -1;
			} else if (X >= 255 && X <= 295 && Y >= 180 && Y <= 220) {
				// OK
				sscanf(text, "%f", number_value); // char array to float
				return 0;
			}
			// Display the selected number text
			ILI9341_printText(spi, text, 15, 20, COLOR_BLACK, COLOR_WHITE, 3);
		}
	}
}

uint8_t question_dialog_logic(ILI9341_SPI *spi){
	// Select numbers
	while (1) {
		TSC2046_GetTouchData(spi);
		uint8_t isPressed = spi->myTsData.isPressed;
		uint16_t X = spi->myTsData.X;
		uint16_t Y = spi->myTsData.Y;
		if (isPressed == true) {
			if (X >= 120 && X <= 155 && Y >= 140 && Y <= 155) {
				ILI9341_drawFastHLine(spi, 123, 136, 37, COLOR_MAGENTA);
				ILI9341_drawFastHLine(spi, 123, 154, 37, COLOR_MAGENTA);
				ILI9341_drawFastVLine(spi, 123, 137, 17, COLOR_MAGENTA);
				ILI9341_drawFastVLine(spi, 160, 137, 17, COLOR_MAGENTA);
				return 1; // Yes
			}else if(X >= 170 && X <= 205 && Y >= 140 && Y <= 155){
				ILI9341_drawFastHLine(spi, 170, 136, 38, COLOR_MAGENTA);
				ILI9341_drawFastHLine(spi, 170, 154, 38, COLOR_MAGENTA);
				ILI9341_drawFastVLine(spi, 170, 137, 17, COLOR_MAGENTA);
				ILI9341_drawFastVLine(spi, 207, 137, 17, COLOR_MAGENTA);
				return 0; // NO
			}
		}
	}
}
