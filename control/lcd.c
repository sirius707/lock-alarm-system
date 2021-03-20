 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.c
 *
 * Description: source file for the LCD driver
 *
 * Author: Mostafa Nabil
 *
 *******************************************************************************/

#include "common_macros.h"
#include <util/delay.h>
#include "lcd.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void LCD_init()
{
	/*make control pins output port*/
	SET_BIT(LCD_CTRL_PORT_DIR, LCD_RS);
	SET_BIT(LCD_CTRL_PORT_DIR, LCD_EN);
	SET_BIT(LCD_CTRL_PORT_DIR, LCD_RW);

	/*make data port output port*/
	#if(LCD_BITS_MODE == 4)
		#ifdef UPPER_NIBBLE
			LCD_DATA_PORT_DIR |= 0xF0;
		#else
			LCD_DATA_PORT_DIR =  0x0F;
		#endif
		LCD_sendCmd(FOUR_BITS_DATA_MODE);
		LCD_sendCmd(TWO_LINE_LCD_4_BIT);
	#else
			LCD_DATA_PORT_DIR = 0xFF;
			LCD_sendCmd(TWO_LINE_LCD_8_BIT);
	#endif

	LCD_sendCmd(CURSOR_OFF);/*turn off cursor*/
	LCD_sendCmd(CLEAR_COMMAND);/*clear LCD*/


}

void LCD_sendCmd(uint8_t cmd)
{
	CLEAR_BIT(LCD_CTRL_PORT, LCD_RS);
	CLEAR_BIT(LCD_CTRL_PORT, LCD_RW);

	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT, LCD_EN);
	_delay_ms(1);
	#if (LCD_BITS_MODE == 4)

		/*send first nibble*/
		#ifdef UPPER_NIBBLE

			LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F)| (cmd & 0xF0);
			CLEAR_BIT(LCD_CTRL_PORT, LCD_EN);
			_delay_ms(1);
			SET_BIT(LCD_CTRL_PORT, LCD_EN);
			_delay_ms(1);
			LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F)| ((cmd & 0x0F) << 4);
			_delay_ms(1);
			CLEAR_BIT(LCD_CTRL_PORT, LCD_EN);

		#else
			LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0)| (cmd & 0x0F);
			CLEAR_BIT(LCD_CTRL_PORT, LCD_EN);
			_delay_ms(1);
			SET_BIT(LCD_CTRL_PORT, LCD_EN);
			_delay_ms(1);
			LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0)| ((cmd & 0xF0) >> 4);
			_delay_ms(1);
			CLEAR_BIT(LCD_CTRL_PORT, LCD_EN);
		#endif

		_delay_ms(1);
	#else
	_delay_ms(1);
	LCD_DATA_PORT = cmd;
	_delay_ms(1);
	#endif
	CLEAR_BIT(LCD_CTRL_PORT, LCD_EN);
	_delay_ms(1);
}

void LCD_putCharacter(const unsigned char ch)
{
	SET_BIT(LCD_CTRL_PORT, LCD_RS);
	CLEAR_BIT(LCD_CTRL_PORT, LCD_RW);

	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT, LCD_EN);
	_delay_ms(1);
	#if (LCD_BITS_MODE == 4)

		/*send first nibble*/
		#ifdef UPPER_NIBBLE

			LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F)| (ch & 0xF0);
			CLEAR_BIT(LCD_CTRL_PORT, LCD_EN);
			_delay_ms(1);
			SET_BIT(LCD_CTRL_PORT, LCD_EN);
			_delay_ms(1);
			LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F)| ((ch & 0x0F) << 4);
			_delay_ms(1);
			CLEAR_BIT(LCD_CTRL_PORT, LCD_EN);

		#else
			LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0)| (ch & 0x0F);
			CLEAR_BIT(LCD_CTRL_PORT, LCD_EN);
			_delay_ms(1);
			SET_BIT(LCD_CTRL_PORT, LCD_EN);
			_delay_ms(1);
			LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0)| ((ch & 0xF0) >> 4);
			_delay_ms(1);
			CLEAR_BIT(LCD_CTRL_PORT, LCD_EN);
		#endif

		_delay_ms(1);
	#else
	_delay_ms(1);
	LCD_DATA_PORT = ch;
	_delay_ms(1);
	#endif
	CLEAR_BIT(LCD_CTRL_PORT, LCD_EN);
	_delay_ms(1);
}

void _LCD_putCharacter(const unsigned char ch)
{
	SET_BIT(LCD_CTRL_PORT, LCD_RS);
	CLEAR_BIT(LCD_CTRL_PORT, LCD_RW);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT, LCD_EN);
	_delay_ms(1);
	LCD_DATA_PORT = ch;
	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT, LCD_EN);
	_delay_ms(1);
}

void LCD_putString(const char *str)
{
	while(*str != '\0'){
		LCD_putCharacter(*str);
		str++;
	}
}

void LCD_clearScreen()
{
	LCD_sendCmd(CLEAR_COMMAND);
}

void LCD_goToRowColumn(uint8_t row, uint8_t col)
{
	uint8 Address;

		/* first of all calculate the required address */
		switch(row)
		{
			case 0:
					Address=col;
					break;
			case 1:
					Address=col+0x40;
					break;
			case 2:
					Address=col+0x10;
					break;
			case 3:
					Address=col+0x50;
					break;
		}
		/* to write to a specific address in the LCD
		 * we need to apply the corresponding command 0b10000000+Address */
		LCD_sendCmd(Address | SET_CURSOR_LOCATION);
}

void LCD_intToStr(const uint16 n)
{
	char str[5] = {'\0'};

	uint8 i = 0;
	for(i = 0; i < 5; i++)str[i] = ' ';
	sprintf(str, "%04u", n);/*read 10 bits only*/
	LCD_putString(str);
}
