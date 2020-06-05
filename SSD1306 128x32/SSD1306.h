/*
 * SSD1306.h
 *
 *  Created on: 4 juin 2020
 *      Author: Daniel Mårtensson
 */

#ifndef SRC_SSD1306_128X32_SSD1306_H_
#define SRC_SSD1306_128X32_SSD1306_H_

#include "stdbool.h"
#include "main.h"

void SSD1306_init(I2C_HandleTypeDef *i2c);
void SSD1306_invert_display(bool invert);
void SSD1306_start_scroll_right(uint8_t start, uint8_t stop);
void SSD1306_start_scroll_left(uint8_t start, uint8_t stop);
void SSD1306_start_scroll_diagonal_right(uint8_t start, uint8_t stop);
void SSD1306_start_scroll_diagonal_left(uint8_t start, uint8_t stop);
void SSD1306_stop_scroll();
void SSD1306_dim(uint8_t dim);
void SSD1306_draw_line(uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1);
void SSD1306_write_text(char* str, uint8_t x, uint8_t y);
void SSD1306_draw_char(char c, uint8_t x, uint8_t y);
void SSD1306_write_bitmap(uint8_t x, uint8_t y, uint8_t* bitmap, uint8_t rows, uint8_t columns);

#endif /* SRC_SSD1306_128X32_SSD1306_H_ */
