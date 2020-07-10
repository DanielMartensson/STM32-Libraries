# ILI9341 LCD with touch

This is a simple LCD with touch feature. The communication protocol is SPI. The touch is not perfect for precision and speed, but at least you can display an image, press some large buttons and write a emoij. This LCD works perfect for simple use where you don't want physical buttons and instead want every button on your embedded system onto the LCD screen.

This library contains
- Windows 3.11 Question dialog
- Icons
- Start up screen
- Plot with 2 graphs
- Number pad
- Frames etc.

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
    show_main_frame(&spi, closedloop_on, update_main_frame, identification_on);

    // Create initial frame
    bool update_main_frame = true;
    bool decimalbutton_show = false;
    bool identification_on = false;
    bool minusbutton_show = false;
    bool closedloop_on = false;
    bool only_move_array = false;
    bool clear_plot = false;
    bool stable = true;
    bool hasChangedStability = true;
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
				if (setpoint > 100)
					setpoint = 100;
				update_main_frame = true;
				show_main_frame(&spi, closedloop_on, update_main_frame, identification_on);
			} else if (X >= 8 && X <= 37 && Y >= 55 && Y <= 94) {
				// Set horizon
				decimalbutton_show = false;
				minusbutton_show = false;
				show_num_pad_frame(&spi, decimalbutton_show, minusbutton_show, &horizon, "Set horizon");
				update_main_frame = true;
				show_main_frame(&spi, closedloop_on, update_main_frame, identification_on);
				htim3.Instance->CCR1 = (uint32_t) horizon;
			} else if (X >= 8 && X <= 37 && Y >= 100 && Y <= 139) {
				// Identification on or off
				if(identification_on == true)
					identification_on = false;
				else
					identification_on = true;
				update_main_frame = false;
				osDelay(300); // Prevent double click
				show_main_frame(&spi, closedloop_on, update_main_frame, identification_on);
			} else if (X >= 8 && X <= 37 && Y >= 146 && Y <= 185) {
				// Open loop or closed loop
				if (closedloop_on == true)
					closedloop_on = false;
				else
					closedloop_on = true;
				update_main_frame = false;
				osDelay(300); // Prevent double click
				show_main_frame(&spi, closedloop_on, update_main_frame, identification_on);
			} else if (X >= 8 && X <= 37 && Y >= 193 && Y <= 232) {
				// Question dialog
				uint8_t choice = show_question_save_settings_dialog(&spi); // Return 1 as YES and 0 as NO
				if (choice == 1) {
					// Save the touch calibration and the user settings
					float wrBuf[7] = { spi.myTS_Calibrate.Scale_X, spi.myTS_Calibrate.Scale_Y, spi.myTS_Calibrate.Bias_X, spi.myTS_Calibrate.Bias_Y, setpoint, horizon, forgetting };
					FlashEraseSector(); // Must clear first
					FlashWriteN(0, wrBuf, 7, DATA_TYPE_FLOAT);
				}

				// Restore to normal again
				update_main_frame = true;
				show_main_frame(&spi, closedloop_on, update_main_frame, identification_on);
			}
			// For every time we change back to main screen, print out if our model is table or not
			displayStableOrUnstable();
		} else {
			// Maximum plot can show is value 203
			uint16_t new_output_uint = 2 * ((uint16_t) new_output_float);
			uint16_t new_input_uint = 2 * ((uint16_t) new_input_float);
			show_plot_frame(&spi, new_input_float, new_output_float, new_input_uint, new_output_uint, only_move_array, clear_plot);
		}
	}
}
```
