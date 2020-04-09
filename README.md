# STM32-Libraries
Collection of rewritten Arduino C++ libraries to STM32 C libraies, and some made by my self.

## TODO
I hope that you like this library and want to share it with others. Please star this library 
and fork it. This is my schedule.

* TMC2209 Stepper motor driver 
* AT-09 Bluetooth 4.0 transmitter and receiver
* HX711 ADC load cell sensor 
* SSD1306 128x32 LCD I2C 
* PCA9685 16-Bit PWM servo driver 
* MCP2515 CAN-bus transmitter and receiver
* SD card reader library 
* MPU-9250 9DOF gyro, accelerometer and magnetometer
* ArduCam OV5642 camera module 
* ESP8266 ESP-01 WiFi HTTP server

## Liquid Crystal LCD Display with I2C Expander IC
Rewritten Arduino C++ library from ![johnrickman](https://github.com/johnrickman/LiquidCrystal_I2C) to STM32 C library. Added some more features as well. Works for both 16x2 LCD and 20x4 LCD etc.

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/LiquidCrystal%20I2C/Selecci%C3%B3n_017.png)
![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/LiquidCrystal%20I2C/Selecci%C3%B3n_018.png)

## MAX31865 RTD Temperature sensor reader
Rewritten STM32 C library from ![4ilo](https://github.com/4ilo/MAX31865-Stm32HAL) so it will fit multiple PT100 sensors at multiple  MAX31865 IC:s. Also I have increase the speed of the library as well. This library is using hardware SPI, not software SPI as ![4ilo](https://github.com/4ilo/MAX31865-Stm32HAL) does.

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/MAX31865/Selecci%C3%B3n_021.png)

## ILI9341 LCD with touch
Rewritten STM32 C library from ![MYaqoobEmbedded](https://github.com/MYaqoobEmbedded/STM32-Tutorials/tree/master/Tutorial%2026%20-%20SPI%20LCD%20%2B%20Resistive%20touch). MYaqoobEmbedded is a very professional STM32 coder but I have rewritted some code of his library. My library is 90% based on MYaqoobEmbedded's library, but I'm using only one SPI peripheral, and also this library is writted with more as a "object oriented" C library as my other libraries are as well. That means you can have multiple ILI9341 LCD's on the same SPI peripheral. All you need to do is declare more structures for the ILI9341 LCD. View the code example in the folder and you will understand.

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/ILI9341%20Touch%20LCD/S%C3%A9lection_013.png)

## ADS1015 and ADS1115
Rewritten Arduino C++ library from ![Adafruit](https://github.com/adafruit/Adafruit_ADS1X15) to STM32 C library. This library shares exactly the same functions as the Adafruit's library. This ADC is very powerfull and have 4 channels for measurement. The resolution is 16-bit which is gives a very accurate measurement. 

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/ADS1015_ADS1115/Adafruit-1085-30091131-02.jpg)

## MCP3008 8-Channel 10-Bit ADC
Written STM32 C library. This is a 10-Bit ADC with 8 channels. Use this if you want to have a potentiometer or something analog like a joystick because this ADC is very stable. This ADC does not fit measurements. Please use another ADC with higher bit resolution e.g ADS1115.

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/MCP3008/IC-MCP-3008-5-1-1-800x800.jpg)

## ADXL345 Accelerometer 10-bit
Rewritten Arduino C++ library from ![SparkFun](https://github.com/sparkfun/SparkFun_ADXL345_Arduino_Library) to STM32 C library. This library share exactly the same functions as the SparkFuns's library. This accelerometer is very cheap but still good. It can measure the acceleration in x, y and z-axis with resolution of 10-bit.

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/ADXL345/ADXL345.jpg)

## MCP23017 IO-Expander 16-bit
Rewritten Arduino C++ library from ![Adafruit](https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library) to STM32 C library. This library share exactly the same functions as the Adafruits's library. This IO-expander can be used if you don't want to waste IO-pins for e.g LCD or other fun stuff or you just have few IO-pins and want more. This can handle interrupts, read and digital outputs.

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/MCP23017/S%C3%A9lection_025.png)

## DS3231 RTC with 2 alarms
Written STM32 C library by my self. This is a well known RTC with two alarm functions for booth weeks and dates.
This RTC is one of the better RTC's out there, but still cheap. This RTC has an output pin that goes low when an alarm appears or both alarms appears. Else the output is high when no alarms appear. That's a way to make sure that the RTC is still alive.

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/DS3231/S%C3%A9lection_041.png)

# nRF24L01 / nRF24L01+ Radio Transmitter and Receiver

This is a very popular and robust radio and transmitter and reciever. This can be used with STM32 boards and it works fine.
I don't know what to say any more, rather than it can send data by using SPI channel. 
I have rewritten this from ![MYaqoobEmbedded](https://github.com/MYaqoobEmbedded/STM32-Tutorials/tree/master/Tutorial%2024%20-%20NRF24L01%20Radio%20Transceiver)
so it can be used with same SPI line. I have't change anything in his logic, only made it possible for using structs as objects. I have also made some simplifications in this library like having only two files.

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/nRF24L01%20Radio%20RX-TX/S%C3%A9lection_060.png)
