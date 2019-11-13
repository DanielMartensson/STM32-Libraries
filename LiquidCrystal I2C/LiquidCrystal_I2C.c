// Based on the work by DFRobot and rewritten by Daniel Mårtensson for STM32

#include "LiquidCrystal_I2C.h"

static void init_priv(LCD* lcd);
static void beginLCD(LCD* lcd);
static void sendToLCD(LCD* lcd, uint8_t value, uint8_t mode);
static void write4bits(LCD* lcd, uint8_t value);
static void expanderWrite(LCD* lcd, uint8_t _data);
static void pulseEnable(LCD* lcd, uint8_t _data);

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

void LiquidCrystal_I2C(LCD* lcd, I2C_HandleTypeDef* hi2c, uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) {
	lcd->_Addr = lcd_Addr;
	lcd->_cols = lcd_cols;
	lcd->_rows = lcd_rows;
	lcd->_backlightval = LCD_BACKLIGHT;
	lcd->_oled = FALSE;
	lcd->hi2c = hi2c;
	lcd->_charsize = LCD_5x8DOTS;
}

void oled_init(LCD* lcd) {
	lcd->_oled = TRUE;
	init_priv(lcd);
}

void init(LCD* lcd) {
	init_priv(lcd);
}

/**
 * Check if we are connected to the LCD device via I2C
 */
static void init_priv(LCD* lcd) {
	if (HAL_I2C_IsDeviceReady(lcd->hi2c, lcd->_Addr, 5, 10) == HAL_OK) {
		lcd->_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
		beginLCD(lcd);
	}else{
		// This MUST have GPIO PA5 ready to use - ERROR I2C - Wrong address
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	}
}

static void beginLCD(LCD* lcd) {
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
void clear(LCD* lcd) {
	command(lcd, LCD_CLEARDISPLAY); // clear display, set cursor position to zero
	HAL_Delay(2);  // this command takes a long time!
	if (lcd->_oled == TRUE)
		setCursor(lcd, 0, 0);
}

void home(LCD* lcd) {
	command(lcd, LCD_RETURNHOME);  // set cursor position to zero
	HAL_Delay(2);  // this command takes a long time!
}

void setCursor(LCD* lcd, uint8_t col, uint8_t row) {
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > lcd->_numlines) {
		row = lcd->_numlines - 1;    // we count rows starting w/0
	}
	command(lcd, LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void noDisplay(LCD* lcd) {
	lcd->_displaycontrol &= ~LCD_DISPLAYON;
	command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}
void display(LCD* lcd) {
	lcd->_displaycontrol |= LCD_DISPLAYON;
	command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}

// Turns the underline cursor on/off
void noCursor(LCD* lcd) {
	lcd->_displaycontrol &= ~LCD_CURSORON;
	command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}
void cursor(LCD* lcd) {
	lcd->_displaycontrol |= LCD_CURSORON;
	command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}

// Turn on and off the blinking cursor
void noBlink(LCD* lcd) {
	lcd->_displaycontrol &= ~LCD_BLINKON;
	command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}
void blink(LCD* lcd) {
	lcd->_displaycontrol |= LCD_BLINKON;
	command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}

// These commands scroll the display without changing the RAM
void scrollDisplayLeft(LCD* lcd) {
	command(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void scrollDisplayRight(LCD* lcd) {
	command(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void leftToRight(LCD* lcd) {
	lcd->_displaymode |= LCD_ENTRYLEFT;
	command(lcd, LCD_ENTRYMODESET | lcd->_displaymode);
}

// This is for text that flows Right to Left
void rightToLeft(LCD* lcd) {
	lcd->_displaymode &= ~LCD_ENTRYLEFT;
	command(lcd, LCD_ENTRYMODESET | lcd->_displaymode);
}

// This will 'right justify' text from the cursor
void autoscroll(LCD* lcd) {
	lcd->_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(lcd, LCD_ENTRYMODESET | lcd->_displaymode);
}

// This will 'left justify' text from the cursor
void noAutoscroll(LCD* lcd) {
	lcd->_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(lcd, LCD_ENTRYMODESET | lcd->_displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void createChar(LCD* lcd, uint8_t location, uint8_t charmap[]) {
	location &= 0x7; // we only have 8 locations 0-7
	command(lcd, LCD_SETCGRAMADDR | (location << 3));
	for (int i = 0; i < 8; i++) {
		write(lcd, charmap[i]);
	}
}


// Turn the (optional) backlight off/on
void noBacklight(LCD* lcd) {
	lcd->_backlightval = LCD_NOBACKLIGHT;
	expanderWrite(lcd, 0);
}

void backlight(LCD* lcd) {
	lcd->_backlightval = LCD_BACKLIGHT;
	expanderWrite(lcd, 0);
}

/*********** mid level commands, for sending data/cmds */

void command(LCD* lcd, uint8_t value) {
	sendToLCD(lcd, value, 0);
}

/************ low level data pushing commands **********/

// write either command or data
static void sendToLCD(LCD* lcd, uint8_t value, uint8_t mode) {
	uint8_t highnib = value & 0xf0;
	uint8_t lownib = (value << 4) & 0xf0;
	write4bits(lcd, (highnib) | mode);
	write4bits(lcd, (lownib) | mode);
}

static void write4bits(LCD* lcd, uint8_t value) {
	expanderWrite(lcd, value);
	pulseEnable(lcd, value);
}

static void expanderWrite(LCD* lcd, uint8_t _data) {
	uint8_t pData = (_data) | (lcd->_backlightval);
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->_Addr, &pData, 1, 10);
}

static void pulseEnable(LCD* lcd, uint8_t _data) {
	expanderWrite(lcd, _data | En);	// En high
	HAL_Delay(1); // enable pulse must be >450ns

	expanderWrite(lcd, _data & ~En); // En low
	HAL_Delay(1); // commands need > 37us to settle
}

// Alias functions

void cursor_on(LCD* lcd) {
	cursor(lcd);
}

void cursor_off(LCD* lcd) {
	noCursor(lcd);
}

void blink_on(LCD* lcd) {
	blink(lcd);
}

void blink_off(LCD* lcd) {
	noBlink(lcd);
}

void load_custom_character(LCD* lcd, uint8_t char_num, uint8_t *rows) {
	createChar(lcd, char_num, rows);
}

void setBacklight(LCD* lcd, uint8_t ON) {
	if (ON == TRUE) {
		backlight(lcd);		// turn backlight on
	} else {
		noBacklight(lcd);		// turn backlight off
	}
}

void write(LCD* lcd, uint8_t value) {
	sendToLCD(lcd, value, Rs);
}

void printToLCD(LCD* lcd, char* string, uint8_t length) {
	for (uint8_t i = 0; i < length; i++) {
		sendToLCD(lcd, (uint8_t) string[i], Rs);
	}
}
