# Liquid Crystal with I2C 

This library is made for the PCF8574/PCF8574A expander IC with I2C connection. With this library, you can write to liquid crystals
displays such as 16x2 LCD or 20x4 LCD. Notice that this example is using GPIOA PIN5 as a check-led if the I2C connected the expander IC. Change function init_priv(LCD* lcd) in LiquidCrystal_I2C.c if you want to change that pin.

This library has been tested with Nucleo-F446RE.

Program example:
```
  // Include
  #include "LiquidCrystal_I2C/LiquidCrystal_I2C.h"
  
  // LCD defines
  #define ADDRESS 0x4E // Write to LCD
  #define COLS 20
  #define ROWS 4
  
  // Structs
  LiquidCrystal_LCD lcd0;
  I2C_HandleTypeDef hi2c1;

  // LCD 20x4
  LiquidCrystal_I2C(&lcd0, &hi2c1, ADDRESS, COLS, ROWS);
  init(&lcd0);
  clear(&lcd0);
  home(&lcd0);
  char* str = "Hej August!";
  printToLCD(&lcd0, str, 11);
  blink_on(&lcd0);
  setCursor(&lcd0, 8, 2);
  char* str2 = "Hej ITH!";
  printToLCD(&lcd0, str2, 8);
```

Hint: If you don't know, the C function sprintf can be used to turn a float value to a string. If you want to display float values on a STM32, then you need to add (Atollic TrueSTUDIO is used): -u _printf_float

![a](https://raw.githubusercontent.com/DanielMartensson/STM32-Libraries/master/LiquidCrystal%20I2C/Selecci%C3%B3n_020.png)
