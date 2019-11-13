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
} LCD;

void LiquidCrystal_I2C(LCD* lcd, I2C_HandleTypeDef* hi2c, uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows);
void clear(LCD* lcd);
void home(LCD* lcd);
void noDisplay(LCD* lcd);
void display(LCD* lcd);
void noBlink(LCD* lcd);
void blink(LCD* lcd);
void noCursor(LCD* lcd);
void cursor(LCD* lcd);
void scrollDisplayLeft(LCD* lcd);
void scrollDisplayRight(LCD* lcd);
void printLeft(LCD* lcd);
void printRight(LCD* lcd);
void leftToRight(LCD* lcd);
void rightToLeft(LCD* lcd);
void shiftIncrement(LCD* lcd);
void shiftDecrement(LCD* lcd);
void noBacklight(LCD* lcd);
void backlight(LCD* lcd);
void autoscroll(LCD* lcd);
void noAutoscroll(LCD* lcd);
void createChar(LCD* lcd, uint8_t location, uint8_t charmap[]); // Example: uint8_t charmap[8] = {0b00100,0b01110,0b01110,0b01110,0b11111,0b00000,0b00100,0b00000};
void setCursor(LCD* lcd, uint8_t, uint8_t);
void write(LCD* lcd, uint8_t);
void command(LCD* lcd, uint8_t);
void init(LCD* lcd);
void oled_init(LCD* lcd);
void printToLCD(LCD* lcd, char* string, uint8_t length);

/*
 * compatibility API function aliases
 */
void blink_on(LCD* lcd);												// alias for blink()
void blink_off(LCD* lcd);       										// alias for noBlink()
void cursor_on(LCD* lcd);      	 										// alias for cursor()
void cursor_off(LCD* lcd);      										// alias for noCursor()
void setBacklight(LCD* lcd, uint8_t ON);								// alias for backlight() and nobacklight()
void load_custom_character(LCD* lcd, uint8_t char_num, uint8_t *rows);  // alias for createChar()



#endif
