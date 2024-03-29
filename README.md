# STM32-Libraries
Collection of rewritten Arduino C++ libraries to STM32 C libraies by my self. Some libraries have i written from scratch.

## Liquid Crystal LCD Display with I2C Expander IC
Rewritten Arduino C++ library from ![johnrickman](https://github.com/johnrickman/LiquidCrystal_I2C) to STM32 C library. Added some more features as well. Works for both 16x2 LCD and 20x4 LCD etc.

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/LiquidCrystal%20I2C/Selecci%C3%B3n_017.png)

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/LiquidCrystal%20I2C/Selecci%C3%B3n_018.png)

## MAX31865 RTD Temperature sensor reader
Rewritten STM32 C library from ![4ilo](https://github.com/4ilo/MAX31865-Stm32HAL) so it will fit multiple PT100 sensors at multiple  MAX31865 IC:s. Also I have increase the speed of the library as well. This library is using hardware SPI, not software SPI as ![4ilo](https://github.com/4ilo/MAX31865-Stm32HAL) does.

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/MAX31865/Selecci%C3%B3n_021.png)

## ILI9341 LCD with touch
Rewritten STM32 C library from ![MYaqoobEmbedded](https://github.com/MYaqoobEmbedded/STM32-Tutorials/tree/master/Tutorial%2026%20-%20SPI%20LCD%20%2B%20Resistive%20touch). MYaqoobEmbedded is a very professional STM32 coder but I have rewritted some code of his library. My library is 90% based on MYaqoobEmbedded's library, but I'm using only one SPI peripheral, and also this library is writted with more as a "object oriented" C library as my other libraries are as well. That means you can have multiple ILI9341 LCD's on the same SPI peripheral. All you need to do is declare more structures for the ILI9341 LCD. View the code example in the folder and you will understand.

This library contains
- Windows 3.11 Question dialog
- Icons
- Start up screen
- Plot with 2 graphs
- Number pad
- Frames etc.

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

# SSD1306 128x32 OLED LCD

My own C code for STM32. This is a simple OLED LCD that have two colors only. Use this LCD if you want to show something simple like a number or display a tiny plot. Notice that this LCD can only have one address. So only one SSD1306 128x32 per project. Connection I2C. I prefer this LCD for debugging.

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/SSD1306%20128x32/S%C3%A9lection_008.png)

# Micro SD SPI 
This code is from [Controllers Tech](https://www.youtube.com/watch?v=spVIZO-jbxE) and I have made a very quick setup how to get started, most for my own purposes because I'm going to use this library in the near future. With this code you can read, write, delete and change text files with a STM32 microcontroller. 

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/Micro%20SD%20SPI/S%C3%A9lection_015.png)

# PCA9685 PWM Servo driver
This is a library for controlling 5V servos with the PCA9685 IC chip. This IC chip has 16 channels with 12-bit PWM. Works great
for small servos. A good thing with this library is that this library share the same functions as [Adafruit's PCA9685 library](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library)

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/PCA9685%20PWM%20Servo/S%C3%A9lection_016.png)

# HX711 Load Cell 24-bit Amplifier 
This is a load cell amplifier with 24-bit resolution. I have rewrite Arduino C++ to STM32 C code from [bogde](https://github.com/bogde/HX711).
This library works great with STM32. 

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/HX711%20Load%20Cell%20Amplifier/S%C3%A9lection_018.png)

# Writing and reading to the memory on the fly
If your CPU missing EEPROM. Don't worry, you can still write and read to the memory. This folder `FlashMemory` gives you a library where you can save `float, uint8_t, uint16_t, uint32_t` values and read them later, during the CPU is running.

 ![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/FlashMemory_F4/memory.png)
 
 # Encoder
 With this, you can measure velocity, direction and also count the position for an encoder.
 
 ![a](https://github.com/DanielMartensson/STM32-Libraries/blob/master/Encoder/Sélection_063.png)
 
 # Sonceboz CAN bus controller
 
 Have a look at my repository CControl here at "Model reference adaptive control" topic
 https://github.com/DanielMartensson/CControl
 
# VL6180X & VL6180 distance + ALS sensor
This library is for ST's own VL6180X and VL6180 distance + ALS sensor. Very good sensor for the low price. I recommend it.
This code is rewritten Arduino C++ code from [Adafruit](https://github.com/adafruit/Adafruit_VL6180X/) to STM32 C code.

![a](https://github.com/DanielMartensson/STM32-Libraries/blob/master/VL6180X/VL6180X.PNG)
