# ILI9341 LCD with touch

This is a simple LCD with touch feature. The communication protocol is SPI. The touch is not perfect for precision and speed, but at least you can display an image, press some large buttons and write a emoij. This LCD works perfect for simple use where you don't want physical buttons and instead want every button on your embedded system onto the LCD screen.

I recommend this LCD due to its price.

Program example:

```
  #include "main.h"
  #include "LCD_ILI9341/LCDCore/LCD_ILI9341.h"
  #include "LCD_ILI9341/LCDTools/LCDTools.h"
  #include "stdbool.h"
  #include <stdlib.h>

  SPI_HandleTypeDef hspi2;

  ILI9341_SPI spi;

  int main(void)
  {
    // Init LCD
    ILI9341_Init(&spi, &hspi2, LCD_CS_GPIO_Port, LCD_CS_Pin, LCD_DC_GPIO_Port, LCD_DC_Pin, LCD_RST_GPIO_Port, LCD_RST_Pin, TS_CS_GPIO_Port, TS_CS_Pin);

    // Calibrate the touch if blue Nucleo board button is pressed
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) {
  	  TSC2046_Calibrate(&spi);
    }

    // Create initial frame
    bool only_change_loop_icon = false;
    bool decimalbutton_show = false;
    bool minusbutton_show = false;
    bool closedloop_on = false;
    bool only_move_array = false;
    float number_value = 0.0;
    int16_t status;
    create_main_frame(&spi, closedloop_on, only_change_loop_icon);
    
	while (1) {
		// Read the LCD touch
		TSC2046_GetTouchData(&spi);
		uint8_t isPressed = spi.myTsData.isPressed;
		uint16_t X = spi.myTsData.X;
		uint16_t Y = spi.myTsData.Y;
		if (isPressed == true) {
			// Check which button we are pressing on
			if (X >= 8 && X <= 37 && Y >= 8 && Y <= 37) {
				// Set point
				decimalbutton_show = true;
				minusbutton_show = true;
				create_num_pad_frame(&spi, decimalbutton_show, minusbutton_show);
				status = num_pad_logic(&spi, decimalbutton_show, minusbutton_show, &number_value);
				only_change_loop_icon = false;
				create_main_frame(&spi, closedloop_on, only_change_loop_icon);
			} else if (X >= 8 && X <= 37 && Y >= 55 && Y <= 94) {
				// Set horizon
				decimalbutton_show = false;
				minusbutton_show = false;
				create_num_pad_frame(&spi, decimalbutton_show, minusbutton_show);
				status = num_pad_logic(&spi, decimalbutton_show, minusbutton_show, &number_value);
				only_change_loop_icon = false;
				create_main_frame(&spi, closedloop_on, only_change_loop_icon);
			} else if (X >= 8 && X <= 37 && Y >= 100 && Y <= 139) {
				// Model order
				decimalbutton_show = false;
				minusbutton_show = false;
				create_num_pad_frame(&spi, decimalbutton_show, minusbutton_show);
				status = num_pad_logic(&spi, decimalbutton_show, minusbutton_show, &number_value);
				only_change_loop_icon = false;
				create_main_frame(&spi, closedloop_on, only_change_loop_icon);
			} else if (X >= 8 && X <= 37 && Y >= 146 && Y <= 185) {
				// Open loop or closed loop
				if (closedloop_on == true)
					closedloop_on = false;
				else
					closedloop_on = true;
				only_change_loop_icon = true;
				create_main_frame(&spi, closedloop_on, only_change_loop_icon);
			} else if (X >= 8 && X <= 37 && Y >= 193 && Y <= 232) {
				// Question dialog
				show_question_dialog(&spi);
				uint8_t choice = question_dialog_logic(&spi); // Return 1 as YES and 0 as NO
				only_change_loop_icon = false;
				create_main_frame(&spi, closedloop_on, only_change_loop_icon);
			}
		}else{
			// Plot
			uint16_t new_input = (uint16_t) rand() % 205;
			uint16_t new_output = (uint16_t) rand() % 205;
			change_plot_values(&spi, new_input, new_output, only_move_array);
		}
	}
}
```
