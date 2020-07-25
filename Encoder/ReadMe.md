# Encoder
With this, you can measure velocity, direction and also count the position for an encoder.

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

  // Init encoder with 1000 milli seconds as intervall and also encoder have 30 pulses for each rotation
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
	Encoder_init(&encoder, &htim5, 30, 1000);
  
  
  while(1){
    Encoder_count(&encoder);
    float speed = Encoder_getSpeed(&encoder);
    float difference = Encoder_getDifference(&encoder);
    HAL_Delay(1000);
  }
  
}
```
