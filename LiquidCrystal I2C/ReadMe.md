# Liquid Crystal with I2C 

This library is made for the PCF8574/PCF8574A expander IC with I2C connection. With this library, you can write to liquid crystals
displays such as 16x2 LCD or 20x4 LCD. 

Program example:
```
  // Include
  #include "LiquidCrystal_I2C/LiquidCrystal_I2C.h"
  
  // LCD defines
  #define ADDRESS 0x4E // Write to LCD
  #define COLS 20
  #define ROWS 4
  
  // Struct
  LCD lcd0;

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
