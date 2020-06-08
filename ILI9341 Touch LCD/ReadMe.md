# ILI9341 LCD with touch

This is a simple LCD with touch feature. The communication protocol is SPI. The touch is not perfect for precision and speed, but at least you can display an image, press some large buttons and write a emoij. This LCD works perfect for simple use where you don't want physical buttons and instead want every button on your embedded system onto the LCD screen.

This library contains
- Windows 3.11 Question dialog
- Icons
- Plot with 2 graphs
- Number pad
- Frames

I recommend this LCD due to its price.

Program example:

```
  // Include
  #include "main.h"
  #include "LCD_ILI9341/LCDCore/LCD_ILI9341.h"
  #include "LCD_ILI9341/LCDTools/LCDTools.h"
  #include "stdbool.h"
  #include <stdlib.h>

  // Structure
  SPI_HandleTypeDef hspi2;
  ILI9341_SPI spi;

  int main(void)
  {
    // Init LCD
    // Init LCD
    ILI9341_Init(&spi, &hspi2, LCD_CS_GPIO_Port, LCD_CS_Pin, LCD_DC_GPIO_Port, LCD_DC_Pin, LCD_RST_GPIO_Port, LCD_RST_Pin, TS_CS_GPIO_Port, TS_CS_Pin);

    // Show welcome screen
    ILI9341_setRotation(&spi, 2);
    ILI9341_printImage(&spi, 0, 0, 320, 240, windows95_intro, 640*240*sizeof(uint8_t));
    HAL_Delay(5000);

    // Calibrate the touch if blue Nucleo board button is pressed
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) {
  	  TSC2046_Calibrate(&spi);
    }

    // Create initial frame
    show_main_frame(&spi, closedloop_on, only_change_loop_icon, stable);

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
				minusbutton_show = false;
				show_num_pad_frame(&spi, decimalbutton_show, minusbutton_show, &setpoint, "Set setpoint <= 100");
				if(setpoint > 100)
					setpoint = 100;
				only_change_loop_icon = false;
				show_main_frame(&spi, closedloop_on, only_change_loop_icon, stable);
			} else if (X >= 8 && X <= 37 && Y >= 55 && Y <= 94) {
				// Set horizon
				decimalbutton_show = false;
				minusbutton_show = false;
				show_num_pad_frame(&spi, decimalbutton_show, minusbutton_show, &horizon, "Set horizon");
				only_change_loop_icon = false;
				show_main_frame(&spi, closedloop_on, only_change_loop_icon, stable);
				htim3.Instance->CCR1 = (uint32_t) horizon;
			} else if (X >= 8 && X <= 37 && Y >= 100 && Y <= 139) {
				// Forgetting factor
				decimalbutton_show = true;
				minusbutton_show = false;
				show_num_pad_frame(&spi, decimalbutton_show, minusbutton_show, &forgetting, "Set forgetting factor 0 > and <= 1");
				if(forgetting > 1)
					forgetting = 1;
				if(forgetting <= 0)
					forgetting = 0.01;
				only_change_loop_icon = false;
				show_main_frame(&spi, closedloop_on, only_change_loop_icon, stable);
			} else if (X >= 8 && X <= 37 && Y >= 146 && Y <= 185) {
				// Open loop or closed loop
				if (closedloop_on == true)
					closedloop_on = false;
				else
					closedloop_on = true;
				only_change_loop_icon = true;
				osDelay(300); // Prevent double click
				show_main_frame(&spi, closedloop_on, only_change_loop_icon, stable);
			} else if (X >= 8 && X <= 37 && Y >= 193 && Y <= 232) {
				// Question dialog
				uint8_t choice = show_question_save_settings_dialog(&spi); // Return 1 as YES and 0 as NO
				only_change_loop_icon = false;
				show_main_frame(&spi, closedloop_on, only_change_loop_icon, stable);
			}
		} else {
			// Maximum plot can show is value 203
			uint16_t new_input_uint = 2*((uint16_t) new_input_float);
			uint16_t new_output_uint = 2*((uint16_t) new_output_float);
			show_plot_frame(&spi, new_input_float, new_output_float, new_input_uint, new_output_uint, only_move_array, clear_plot);
		}

		// If our model have change its stability - Show the error icon once
		if(hasChangedStability == true){
			only_change_loop_icon = true;
			show_main_frame(&spi, closedloop_on, only_change_loop_icon, stable);
			hasChangedStability = false;
		}
	}
}
```
