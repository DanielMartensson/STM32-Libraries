# HX711 Load Cell 24-bit Amplifier 
This is a load cell amplifier with 24-bit resolution. I have rewrite Arduino C++ to STM32 C code from [bogde](https://github.com/bogde/HX711).
This library works great with STM32. 

Program example:

```
// Include
#include "main.h"
#include "HX711/HX711.h"

// Structure
HX711 hx;

int main(void)
{
  // PD_SCK is digital output
  // DOUT is digital input with a pull-up resistor activated
  HX711_begin(&hx, HX711_PD_SCK_GPIO_Port, HX711_PD_SCK_Pin, HX711_DOUT_GPIO_Port, HX711_DOUT_Pin, 128);
  HX711_set_scale(&hx, 2280.f); // Set scale
  HX711_tare(&hx, 0); // Reset scale to 0

    while (1)
    {
         float value = HX711_get_units(&hx, 10);
         char text[20];
         sprintf(text, "%f", value); // Don't forget to enable -u _printf_float flag in the settings
  }
}
```
