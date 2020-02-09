# STM32-Libraries
Collection of rewritten Arduino C++ libraries to STM32 C libraies. 

## Liquid Crystal LCD Display with I2C Expander IC
Rewritten Arduino C++ library from ![johnrickman](https://github.com/johnrickman/LiquidCrystal_I2C) to STM32 C library. Added some more features as well. Works for both 16x2 LCD and 20x4 LCD etc.

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/LiquidCrystal%20I2C/Selecci%C3%B3n_017.png)
![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/LiquidCrystal%20I2C/Selecci%C3%B3n_018.png)

## MAX31865 RTD Temperature sensor reader
Rewritten STM32 C library from ![4ilo](https://github.com/4ilo/MAX31865-Stm32HAL) so it will fit multiple PT100 sensors at multiple  MAX31865 IC:s. Also I have increase the speed of the library as well. This library is using hardware SPI, not software SPI as ![4ilo](https://github.com/4ilo/MAX31865-Stm32HAL) does.

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/MAX31865/Selecci%C3%B3n_021.png)

## ILI9341 LCD with touch
Rewritten STM32 C library from ![MYaqoobEmbedded](https://github.com/MYaqoobEmbedded/STM32-Tutorials/tree/master/Tutorial%2026%20-%20SPI%20LCD%20%2B%20Resistive%20touch). MYaqoobEmbedded is a very professional STM32 coder but I have rewritted some code of his library. My library is 90% based on MYaqoobEmbedded's library, but I'm using only one SPI peripheral, and also this library is writted with more as a "object oriented" C library as my other libraries are as well. That means you can have multiple ILI9341 LCD's on the same SPI peripheral. All you need to do is declare more structures for the ILI9341 LCD. View the program example in the folder and you will understand.

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/ILI9341%20Touch%20LCD/S%C3%A9lection_013.png)

## ADS1015 and ADS1115
Rewritten Arduino C++ library from ![Adafruit](https://github.com/adafruit/Adafruit_ADS1X15) to STM32 C library. This library shares exactly the same functions as the Adafruit's library. This ADC is very powerfull and have 4 channels for measurement. The resolution is 16-bit which is gives a very accurate measurement. 

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/ADS1015_ADS1115/Adafruit-1085-30091131-02.jpg)
