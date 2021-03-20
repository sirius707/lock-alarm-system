 /******************************************************************************
 *
 * Module: keypad
 *
 * File Name: keypad.c
 *
 * Description: Source file for the Keypad driver
 *
 * Author: Mostafa Nabil
 *
 *******************************************************************************/

#include "keypad.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

#if (N_col == 3)

/*function that decodes keypad input into a corresponding number*/
static uint8 KeyPad_4x3_adjust(uint8 button);
#elif (N_col == 4)
/*function that decodes keypad input into a corresponding number*/
static uint8 KeyPad_4x4_adjust(uint8 button);
#endif

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*return value of key pressed*/
uint8 KeyPad_getPressedKey(void)
{
	uint8 col,row;
	while(1)
	{
        /*make first least signficant 4 pins input pins*/
		KEYPAD_PORT_DIR &= 0XF0;

        /*make first most signficant 4 pins output pins*/
		KEYPAD_PORT_DIR|=0XF0;

		/*activate pull up resistor*/
		KEYPAD_PORT_OUT=0XFF;
		for(col=0;col<N_col;col++) /* loop on columns */
		{

			KEYPAD_PORT_OUT|=0XF0;

			/*activate one column per iteration*/
			KEYPAD_PORT_OUT&=~(1<<(col+4));
			for(row=0;row<N_row;row++) /* loop on rows */
			{
				if(BIT_IS_CLEAR(KEYPAD_PORT_IN,row)) /* if the switch is press in this row */
				{
#if (N_col == 3)
					return KeyPad_4x3_adjust((row*N_col)+col+1);
#elif (N_col == 4)
					return KeyPad_4x4_adjust((row*N_col)+col+1);
#endif
				}
			}
		}
	}
}

#if (N_col == 3)

static uint8 KeyPad_4x3_adjust(uint8 button_number)
{
	switch(button_number)
	{
	case 10: return '*'; // ASCII Code of *
	break;
	case 11: return 0;
	break;
	case 12: return '#'; // ASCII Code of #
	break;
	default: return button_number;
	}
}

#elif (N_col == 4)

static uint8 KeyPad_4x4_adjus(uint8 button_number)
{
	switch(button_number)
	{
	case 1: return 7;
	break;
	case 2: return 8;
	break;
	case 3: return 9;
	break;
	case 4: return '%'; // ASCII Code of %
	break;
	case 5: return 4;
	break;
	case 6: return 5;
	break;
	case 7: return 6;
	break;
	case 8: return '*'; /* ASCII Code of '*' */
	break;
	case 9: return 1;
	break;
	case 10: return 2;
	break;
	case 11: return 3;
	break;
	case 12: return '-'; /* ASCII Code of '-' */
	break;
	case 13: return 13;  /* ASCII of Enter */
	break;
	case 14: return 0;
	break;
	case 15: return '='; /* ASCII Code of '=' */
	break;
	case 16: return '+'; /* ASCII Code of '+' */
	break;
	default: return button_number;
	}
}

#endif
