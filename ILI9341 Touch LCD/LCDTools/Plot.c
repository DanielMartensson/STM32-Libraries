/*
 * Plot.c
 *
 *  Created on: 22 mai 2020
 *      Author: Daniel Mårtensson
 */

#include "LCDTools.h"

static void move(uint16_t i, uint16_t inputs[], uint16_t outputs[], uint16_t new_input, uint16_t new_output);

void change_plot_values(ILI9341_SPI *spi, uint16_t new_input, uint16_t new_output, bool only_move_array) {
	// We plot from x1 = 51 to x2 = 314, which is 263 pixels
	static uint16_t inputs[264];
	static uint16_t outputs[264];
	// Add more static arrays here

	if (only_move_array == false) {
		// Update the text
		char text[20];
		sprintf(text, "Input:%d", new_input);
		ILI9341_printText(spi, text, 55, 10, COLOR_RED, COLOR_NAVY, 2);
		sprintf(text, "Output:%d", new_output);
		ILI9341_printText(spi, text, 180, 10, COLOR_CYAN, COLOR_NAVY, 2);
		// Add more printText functions here

		// Change
		for (int16_t i = 51; i < 315; i++) {
			// Clear
			ILI9341_DrawPixel(spi, i, -inputs[i - 51] + 234, COLOR_NAVY);
			ILI9341_DrawPixel(spi, i, -outputs[i - 51] + 234, COLOR_NAVY);
			// Add more clear pixels here

			// Move
			move(i, inputs, outputs, new_input, new_output);
			// Add more move arrays here

			// Fill
			ILI9341_DrawPixel(spi, i, -inputs[i - 51] + 234, COLOR_RED);
			ILI9341_DrawPixel(spi, i, -outputs[i - 51] + 234, COLOR_CYAN);
			// Add more fill pixles here
		}
	} else {
		for (int16_t i = 51; i < 315; i++) {
			move(i, inputs, outputs, new_input, new_output);
			// Add more move arrays here
		}
	}
}

static void move(uint16_t i, uint16_t inputs[], uint16_t outputs[], uint16_t new_input, uint16_t new_output) {
	if (i < 314) {
		// We can only move values that have a next element
		inputs[i - 51] = inputs[i - 50];
		outputs[i - 51] = outputs[i - 50];
	} else {
		// Limits
		if (new_input < 203) {
			inputs[i - 51] = new_input;
		} else {
			inputs[i - 51] = 203;
		}
		if (new_output < 203) {
			outputs[i - 51] = new_output;
		} else {
			outputs[i - 51] = 203;
		}
	}
}
