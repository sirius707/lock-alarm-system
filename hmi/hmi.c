 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: hmi.c
 *
 * Description: source file for the HMI driver
 *
 * Author: Mostafa Nabil
 *
 *******************************************************************************/

#include "hmi.h"
#include "lcd.h"
#include "uart.h"
#include "keypad.h"
#include "timer.h"
#include <string.h>

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define PASS_LEN 6
#define NUMBER_OF_OPERATIONS 7
#define KEYPAD_DELAY 500


/*******************************************************************************
 *                      Global Variables									   *
 *******************************************************************************/
static volatile uint8 g_tick = 0;
static void (*func_ptrs[NUMBER_OF_OPERATIONS])();


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
static void HMI_callbackUpdateGTick()
{
	g_tick++;
}

void HMI_init()
{
	UART_CONF_STRUCT uart_config;

	uart_config.baud_rate = 9600;
	uart_config.two_stop_bits = FALSE;
	uart_config.parity = DISABLED;
	uart_config.num_of_bits = EIGHT_BIT;

	UART_init(&uart_config);

	LCD_init();

	func_ptrs[NEW_PASS] = HMI_newPass;
	func_ptrs[ENTER_PASS] = HMI_enterPass;
	func_ptrs[OPENING] = HMI_opening;
	func_ptrs[OPTIONS] = HMI_options;
	func_ptrs[ALARM] = HMI_alarm;
	func_ptrs[LOCKING] = HMI_locking;
	func_ptrs[CONFIRM_PASS] = HMI_confirmPass;

	/*timer1 initialization*/
	TIMER1_CONFIG timer1_config;
	timer1_config.clock = FCPU_256;
	timer1_config.mode = CTC;
	timer1_config.init_value = 0;
	timer1_config.compare_value = 31250;/*generate interrupt every second*/
	Timer1_setCallBackCompare(HMI_callbackUpdateGTick);
	Timer1_init(&timer1_config);
	Timer1_Stop();
}

uint8 HMI_recieveCmd()
{
	return UART_recvByte();
}

void HMI_execCmd(CONTROL_COMMAND control_cmd)
{
	(func_ptrs[control_cmd])();
}
void HMI_opening()
{
	LCD_clearScreen();
	LCD_goToRowColumn(0, 0);
	LCD_putString("Door unlocking...");

	g_tick = 0;
	Timer1_start(FCPU_256);
	while(g_tick < LOCK_PERIOD+WAIT_PERIOD){/*display message for period of locking + waiting*/
		LCD_goToRowColumn(1, 0);
		LCD_intToStr(g_tick);
	}
	Timer1_Stop();
}


void HMI_alarm()
{
	LCD_clearScreen();
	LCD_goToRowColumn(0, 0);
	LCD_putString("ERROR!!");

	g_tick = 0;
	Timer1_start(FCPU_256);
	while(g_tick < ALARM_PERIOD){
		LCD_goToRowColumn(1, 0);
		LCD_intToStr(g_tick);
	}
	Timer1_Stop();
}

void HMI_locking()
{
	LCD_clearScreen();
	LCD_goToRowColumn(0, 0);
	LCD_putString("Door locking...  ");


	g_tick = 0;
	Timer1_start(FCPU_256);
	while(g_tick < LOCK_PERIOD){
		LCD_goToRowColumn(1, 0);
		LCD_intToStr(g_tick);
	}
	Timer1_Stop();
}


void HMI_options()
{
	uint8 key;
	LCD_clearScreen();
	LCD_goToRowColumn(0, 0);
	LCD_putString("+ change pass  ");

	LCD_goToRowColumn(1, 0);
	LCD_putString("- open door        ");

	while(1){
		key = KeyPad_getPressedKey();
		if(key == '+' || key == '-')break;
	}

	UART_sendByte(key);

}

void HMI_newPass()
{
	uint8 password[PASS_LEN];
	uint8 i = 0; /*loop counter*/

	_delay_ms(KEYPAD_DELAY);

	LCD_clearScreen();
	LCD_goToRowColumn(0, 0);
	LCD_putString("Enter new pass:");
	LCD_goToRowColumn(1, 0);/*move cursor to next line*/

	do{
		password[i] = KeyPad_getPressedKey();
		if(password[i] >=0 && password[i] <= 9){/*accept digits only*/
			password[i] = '0' + password[i];/*turn digit to corresponding character*/
			i++;
			LCD_putCharacter('*');
		}
		_delay_ms(KEYPAD_DELAY);

	}while(i < PASS_LEN-1);
	password[i] = '\0';

	while(KeyPad_getPressedKey() != 13);/*wait until return is pressed*/

	UART_sendString(password, '#');
}

void HMI_confirmPass()
{
	uint8 password_confirm[PASS_LEN];
	uint8 i = 0; /*loop counter*/

	_delay_ms(KEYPAD_DELAY);

	LCD_clearScreen();
	LCD_goToRowColumn(0, 0);
	LCD_putString("confirm new pass:");
	LCD_goToRowColumn(1, 0);/*move cursor to next line*/

	do{
		password_confirm[i] = KeyPad_getPressedKey();
		if(password_confirm[i] >=0 && password_confirm[i] <= 9){/*accept digits only*/
			password_confirm[i] = '0' + password_confirm[i];/*turn digit to corresponding character*/
			i++;
			LCD_putCharacter('*');
		}
		_delay_ms(KEYPAD_DELAY);

	}while(i < PASS_LEN-1);
	password_confirm[i] = '\0';

	while(KeyPad_getPressedKey() != 13);/*wait until return is pressed*/


	UART_sendString(password_confirm, '#');
}
void HMI_enterPass()
{

	uint8 password[PASS_LEN];
	uint8 i = 0; /*loop counter*/

	_delay_ms(KEYPAD_DELAY);

	LCD_clearScreen();
	LCD_goToRowColumn(0, 0);
	LCD_putString("Enter pass:");

	LCD_goToRowColumn(1, 0);/*move cursor to next line*/
	do{
		password[i] = KeyPad_getPressedKey();
		if(password[i] >=0 && password[i] <= 9){/*accept digits only*/
			password[i] = '0' + password[i];/*turn digit to corresponding character*/
			i++;
			LCD_putCharacter('*');
		}
		_delay_ms(KEYPAD_DELAY);

	}while(i < PASS_LEN-1);
	password[i] = '\0';

	while(KeyPad_getPressedKey() != 13);/*wait until return is pressed*/

	UART_sendString(password, '#');
}


