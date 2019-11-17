# STM32-Libraries
Collection of rewritten Arduino C++ libraries to STM32 C libraies. 

## Liquid Crystal LCD Display with I2C Expander IC
Rewritten Arduino C++ library from ![johnrickman](https://github.com/johnrickman/LiquidCrystal_I2C) to STM32 C library. Added some more features as well. Works for both 16x2 LCD and 20x4 LCD etc.

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/LiquidCrystal%20I2C/Selecci%C3%B3n_017.png)
![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/LiquidCrystal%20I2C/Selecci%C3%B3n_018.png)

Hint: If you don't know, the C function sprintf can be used to turn a float value to a string. If you want to display float values on a STM32, then you need to add (Atollic TrueSTUDIO is used): -u _printf_float

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/LiquidCrystal%20I2C/Selecci%C3%B3n_020.png)


## MAX31865 RTD Temperature sensor reader
Rewritten Arduino C++ library from ![olewolf](https://github.com/olewolf/arduino-max31865) to STM32 C library. Added some more features as well.

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/MAX31865/Selecci%C3%B3n_021.png)
