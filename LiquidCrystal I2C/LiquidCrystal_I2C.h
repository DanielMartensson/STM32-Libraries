//Daniel Mårtensson
#ifndef LiquidCrystal_I2C_h
#define LiquidCrystal_I2C_h

#include "main.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

// Booleans
#define TRUE 1
#define FALSE 0

typedef struct {
	uint8_t _Addr;
	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;
	uint8_t _numlines;
	uint8_t _oled; // Initial false
	uint8_t _cols;
	uint8_t _rows;
	uint8_t _backlightval;
	uint8_t _charsize;  // E.g charsize = LCD_5x8DOTS
	I2C_HandleTypeDef* hi2c;
} LiquidCrystal_LCD;

void LiquidCrystal_I2C(LiquidCrystal_LCD* lcd, I2C_HandleTypeDef* hi2c, uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows);
void clear(LiquidCrystal_LCD* lcd);
void home(LiquidCrystal_LCD* lcd);
void noDisplay(LiquidCrystal_LCD* lcd);
void display(LiquidCrystal_LCD* lcd);
void noBlink(LiquidCrystal_LCD* lcd);
void blink(LiquidCrystal_LCD* lcd);
void noCursor(LiquidCrystal_LCD* lcd);
void cursor(LiquidCrystal_LCD* lcd);
void scrollDisplayLeft(LiquidCrystal_LCD* lcd);
void scrollDisplayRight(LiquidCrystal_LCD* lcd);
void printLeft(LiquidCrystal_LCD* lcd);
void printRight(LiquidCrystal_LCD* lcd);
void leftToRight(LiquidCrystal_LCD* lcd);
void rightToLeft(LiquidCrystal_LCD* lcd);
void shiftIncrement(LiquidCrystal_LCD* lcd);
void shiftDecrement(LiquidCrystal_LCD* lcd);
void noBacklight(LiquidCrystal_LCD* lcd);
void backlight(LiquidCrystal_LCD* lcd);
void autoscroll(LiquidCrystal_LCD* lcd);
void noAutoscroll(LiquidCrystal_LCD* lcd);
void createChar(LiquidCrystal_LCD* lcd, uint8_t location, uint8_t charmap[]); // Example: uint8_t charmap[8] = {0b00100,0b01110,0b01110,0b01110,0b11111,0b00000,0b00100,0b00000};
void setCursor(LiquidCrystal_LCD* lcd, uint8_t, uint8_t);
void write(LiquidCrystal_LCD* lcd, uint8_t);
void command(LiquidCrystal_LCD* lcd, uint8_t);
void init(LiquidCrystal_LCD* lcd);
void oled_init(LiquidCrystal_LCD* lcd);
void printToLCD(LiquidCrystal_LCD* lcd, char* string, uint8_t length);

/*
 * compatibility API function aliases
 */
void blink_on(LiquidCrystal_LCD* lcd);												// alias for blink()
void blink_off(LiquidCrystal_LCD* lcd);       										// alias for noBlink()
void cursor_on(LiquidCrystal_LCD* lcd);      	 										// alias for cursor()
void cursor_off(LiquidCrystal_LCD* lcd);      										// alias for noCursor()
void setBacklight(LiquidCrystal_LCD* lcd, uint8_t ON);								// alias for backlight() and nobacklight()
void load_custom_character(LiquidCrystal_LCD* lcd, uint8_t char_num, uint8_t *rows);  // alias for createChar()

#endif
