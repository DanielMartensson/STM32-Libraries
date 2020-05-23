/*
 * Functions.h
 *
 *  Created on: May 22, 2020
 *      Author: Daniel Mårtensson
 */

#ifndef SRC_LCDTOOLS_LCDTOOLS_H_
#define SRC_LCDTOOLS_LCDTOOLS_H_

#include "../LCDCore/LCD_ILI9341.h"
#include "stdbool.h"
#include <stdio.h> // For sprintf
#include <string.h> // For memset

void create_main_frame(ILI9341_SPI* spi, bool closedloop_on, bool only_change_loop_icon);
void create_num_pad_frame(ILI9341_SPI* spi, bool decimalbutton_show, bool minusbutton_show);
void change_plot_values(ILI9341_SPI *spi, uint16_t new_input, uint16_t new_output, bool only_move_array);
int16_t num_pad_logic(ILI9341_SPI *spi, bool decimalbutton_show, bool minusbutton_show, float *number_value);
void show_question_dialog(ILI9341_SPI *spi);
uint8_t question_dialog_logic(ILI9341_SPI *spi);

#endif /* SRC_LCDTOOLS_LCDTOOLS_H_ */
