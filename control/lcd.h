 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.h
 *
 * Description: Header file for the LCD driver
 *
 * Author: Mostafa Nabil
 *
 *******************************************************************************/

#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"
#include <avr/io.h>
#include <stdio.h>

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/*LCD PINS*/
#define LCD_DATA_PORT_DIR DDRC
#define LCD_DATA_PORT	  PORTC
#define LCD_CTRL_PORT_DIR DDRD
#define LCD_CTRL_PORT 	  PORTD

#define LCD_RS PD4
#define LCD_RW PD5
#define LCD_EN PD6

/* LCD Commands */
#define CLEAR_COMMAND 0x01
#define CURSOR_OFF 0x0C
#define CURSOR_ON 0x0E
#define SET_CURSOR_LOCATION 0x80
#define FOUR_BITS_DATA_MODE 0x02
#define TWO_LINE_LCD_8_BIT 0x38
#define TWO_LINE_LCD_4_BIT 0x28

#define LCD_BITS_MODE 8

#if (LCD_BITS_MODE == 4)
#define UPPER_NIBBLE
#endif

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void LCD_init();
void LCD_sendCmd(uint8_t cmd);
void LCD_putCharacter(const unsigned char ch);
void LCD_putString(const char *str);
void LCD_goToRowColumn(uint8_t row, uint8_t col);
void LCD_clearScreen();
void LCD_intToStr(const uint16 n);

#endif /* LCD_H_ */
