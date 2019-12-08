// Based on the work by DFRobot and rewritten by Daniel Mårtensson for STM32

#include "LiquidCrystal_I2C.h"

static void init_priv(LiquidCrystal_LCD* lcd);
static void beginLCD(LiquidCrystal_LCD* lcd);
static void sendToLCD(LiquidCrystal_LCD* lcd, uint8_t value, uint8_t mode);
static void write4bits(LiquidCrystal_LCD* lcd, uint8_t value);
static void expanderWrite(LiquidCrystal_LCD* lcd, uint8_t _data);
static void pulseEnable(LiquidCrystal_LCD* lcd, uint8_t _data);

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

void LiquidCrystal_I2C(LiquidCrystal_LCD* lcd, I2C_HandleTypeDef* hi2c, uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) {
	lcd->_Addr = lcd_Addr;
	lcd->_cols = lcd_cols;
	lcd->_rows = lcd_rows;
	lcd->_backlightval = LCD_BACKLIGHT;
	lcd->_oled = FALSE;
	lcd->hi2c = hi2c;
	lcd->_charsize = LCD_5x8DOTS;
}

void oled_init(LiquidCrystal_LCD* lcd) {
	lcd->_oled = TRUE;
	init_priv(lcd);
}

void init(LiquidCrystal_LCD* lcd) {
	init_priv(lcd);
}

/**
 * Check if we are connected to the LCD device via I2C
 */
static void init_priv(LiquidCrystal_LCD* lcd) {
	if (HAL_I2C_IsDeviceReady(lcd->hi2c, lcd->_Addr, 5, 10) == HAL_OK) {
		lcd->_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
		beginLCD(lcd);
	}else{
		// This MUST have GPIO PA5 ready to use - ERROR I2C - Wrong address
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	}
}

static void beginLCD(LiquidCrystal_LCD* lcd) {
	if (lcd->_rows > 1) {
		lcd->_displayfunction |= LCD_2LINE;
	}
	lcd->_numlines = lcd->_rows;

	// for some 1 line displays you can select a 10 pixel high font
	if ((lcd->_charsize != 0) && (lcd->_rows == 1)) {
		lcd->_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	HAL_Delay(50);

	// Now we pull both RS and R/W low to begin commands
	expanderWrite(lcd, lcd->_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
	HAL_Delay(1000);

	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46

	// we start in 8bit mode, try to set 4 bit mode
	write4bits(lcd, 0x03 << 4);
	HAL_Delay(5); // wait min 4.1ms

	// second try
	write4bits(lcd, 0x03 << 4);
	HAL_Delay(5); // wait min 4.1ms

	// third go!
	write4bits(lcd, 0x03 << 4);
	HAL_Delay(1);

	// finally, set to 4-bit interface
	write4bits(lcd, 0x02 << 4);

	// set # lines, font size, etc.
	command(lcd, LCD_FUNCTIONSET | lcd->_displayfunction);

	// turn the display on with no cursor or blinking default
	lcd->_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	display(lcd);

	// clear it off
	clear(lcd);

	// Initialize to default text direction (for roman languages)
	lcd->_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	command(lcd, LCD_ENTRYMODESET | lcd->_displaymode);

	home(lcd);

}

/********** high level commands, for the user! */
void clear(LiquidCrystal_LCD* lcd) {
	command(lcd, LCD_CLEARDISPLAY); // clear display, set cursor position to zero
	HAL_Delay(2);  // this command takes a long time!
	if (lcd->_oled == TRUE)
		setCursor(lcd, 0, 0);
}

void home(LiquidCrystal_LCD* lcd) {
	command(lcd, LCD_RETURNHOME);  // set cursor position to zero
	HAL_Delay(2);  // this command takes a long time!
}

void setCursor(LiquidCrystal_LCD* lcd, uint8_t col, uint8_t row) {
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > lcd->_numlines) {
		row = lcd->_numlines - 1;    // we count rows starting w/0
	}
	command(lcd, LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void noDisplay(LiquidCrystal_LCD* lcd) {
	lcd->_displaycontrol &= ~LCD_DISPLAYON;
	command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}
void display(LiquidCrystal_LCD* lcd) {
	lcd->_displaycontrol |= LCD_DISPLAYON;
	command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}

// Turns the underline cursor on/off
void noCursor(LiquidCrystal_LCD* lcd) {
	lcd->_displaycontrol &= ~LCD_CURSORON;
	command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}
void cursor(LiquidCrystal_LCD* lcd) {
	lcd->_displaycontrol |= LCD_CURSORON;
	command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}

// Turn on and off the blinking cursor
void noBlink(LiquidCrystal_LCD* lcd) {
	lcd->_displaycontrol &= ~LCD_BLINKON;
	command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}
void blink(LiquidCrystal_LCD* lcd) {
	lcd->_displaycontrol |= LCD_BLINKON;
	command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}

// These commands scroll the display without changing the RAM
void scrollDisplayLeft(LiquidCrystal_LCD* lcd) {
	command(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void scrollDisplayRight(LiquidCrystal_LCD* lcd) {
	command(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void leftToRight(LiquidCrystal_LCD* lcd) {
	lcd->_displaymode |= LCD_ENTRYLEFT;
	command(lcd, LCD_ENTRYMODESET | lcd->_displaymode);
}

// This is for text that flows Right to Left
void rightToLeft(LiquidCrystal_LCD* lcd) {
	lcd->_displaymode &= ~LCD_ENTRYLEFT;
	command(lcd, LCD_ENTRYMODESET | lcd->_displaymode);
}

// This will 'right justify' text from the cursor
void autoscroll(LiquidCrystal_LCD* lcd) {
	lcd->_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(lcd, LCD_ENTRYMODESET | lcd->_displaymode);
}

// This will 'left justify' text from the cursor
void noAutoscroll(LiquidCrystal_LCD* lcd) {
	lcd->_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(lcd, LCD_ENTRYMODESET | lcd->_displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void createChar(LiquidCrystal_LCD* lcd, uint8_t location, uint8_t charmap[]) {
	location &= 0x7; // we only have 8 locations 0-7
	command(lcd, LCD_SETCGRAMADDR | (location << 3));
	for (int i = 0; i < 8; i++) {
		write(lcd, charmap[i]);
	}
}


// Turn the (optional) backlight off/on
void noBacklight(LiquidCrystal_LCD* lcd) {
	lcd->_backlightval = LCD_NOBACKLIGHT;
	expanderWrite(lcd, 0);
}

void backlight(LiquidCrystal_LCD* lcd) {
	lcd->_backlightval = LCD_BACKLIGHT;
	expanderWrite(lcd, 0);
}

/*********** mid level commands, for sending data/cmds */

void command(LiquidCrystal_LCD* lcd, uint8_t value) {
	sendToLCD(lcd, value, 0);
}

/************ low level data pushing commands **********/

// write either command or data
static void sendToLCD(LiquidCrystal_LCD* lcd, uint8_t value, uint8_t mode) {
	uint8_t highnib = value & 0xf0;
	uint8_t lownib = (value << 4) & 0xf0;
	write4bits(lcd, (highnib) | mode);
	write4bits(lcd, (lownib) | mode);
}

static void write4bits(LiquidCrystal_LCD* lcd, uint8_t value) {
	expanderWrite(lcd, value);
	pulseEnable(lcd, value);
}

static void expanderWrite(LiquidCrystal_LCD* lcd, uint8_t _data) {
	uint8_t pData = (_data) | (lcd->_backlightval);
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->_Addr, &pData, 1, 10);
}

static void pulseEnable(LiquidCrystal_LCD* lcd, uint8_t _data) {
	expanderWrite(lcd, _data | En);	// En high
	HAL_Delay(1); // enable pulse must be >450ns

	expanderWrite(lcd, _data & ~En); // En low
	HAL_Delay(1); // commands need > 37us to settle
}

// Alias functions

void cursor_on(LiquidCrystal_LCD* lcd) {
	cursor(lcd);
}

void cursor_off(LiquidCrystal_LCD* lcd) {
	noCursor(lcd);
}

void blink_on(LiquidCrystal_LCD* lcd) {
	blink(lcd);
}

void blink_off(LiquidCrystal_LCD* lcd) {
	noBlink(lcd);
}

void load_custom_character(LiquidCrystal_LCD* lcd, uint8_t char_num, uint8_t *rows) {
	createChar(lcd, char_num, rows);
}

void setBacklight(LiquidCrystal_LCD* lcd, uint8_t ON) {
	if (ON == TRUE) {
		backlight(lcd);		// turn backlight on
	} else {
		noBacklight(lcd);		// turn backlight off
	}
}

void write(LiquidCrystal_LCD* lcd, uint8_t value) {
	sendToLCD(lcd, value, Rs);
}

void printToLCD(LiquidCrystal_LCD* lcd, char* string, uint8_t length) {
	for (uint8_t i = 0; i < length; i++) {
		sendToLCD(lcd, (uint8_t) string[i], Rs);
	}
}
