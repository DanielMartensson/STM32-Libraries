# Encoder
With this, you can measure velocity, direction and also count the position for an encoder.

Settings:

Don't forget to use pull-down resistors at `GPIO Settings->GPIO Pull-up/Pull-down`

![a](https://github.com/DanielMartensson/STM32-Libraries/blob/master/Encoder/S%C3%A9lection_064.png)

Program example:

```
// Includes
#include "main.h"
#include "Encoder/Encoder.h"

// Structures
TIM_HandleTypeDef htim5;
Encoder encoder;

int main(){
  /* Regular STM32 stuffs */

  // Init encoder that have 30 pulses for each rotation
  HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
  Encoder_init(&encoder, &htim5, 30);
  
  while(1){
    Encoder_count(&encoder);
    float speed = Encoder_getSpeed(&encoder); // Unit: RPM
    float difference = Encoder_getDifference(&encoder);
    HAL_Delay(1);
  }
  
}
```
