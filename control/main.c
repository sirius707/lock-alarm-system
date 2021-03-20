#define F_CPU 8000000

#include "uart.h"
#include <string.h>
#include "eeprom.h"
#include "timer.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define PASS_LEN 7
#define MAX_PASS_ATTEMPTS 3
#define EEPROM_DELAY 10 /*delay between eeprom function calls*/

#define LOCK_PERIOD 15U /*duration for opening and locking the door*/
#define WAIT_PERIOD 3U	/*duration for keeping the door open*/
#define ALARM_PERIOD 60U /*ALARM DURATION*/

/*******************************************************************************
 *                      data types												*
 *******************************************************************************/
/*commands recieved from control MC through UART*/
typedef enum{NEW_PASS, ENTER_PASS, OPENING, LOCKING, ALARM, OPTIONS, CONFIRM_PASS}CONTROL_COMMAND;

/*state of control MC */
typedef enum{CTRL_OPTIONS, CTRL_OPEN_DOOR, CTRL_CHANGE_PASSWORD}CONTROL_STATE;
CONTROL_STATE control_state = OPTIONS;

/*******************************************************************************
 *                      Global variables									   *
 *******************************************************************************/
volatile uint8 g_tick = 0;/*increments each second, updated by timer ISR*/


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*auxiliry functions, used mainly by state functions*/
void CONTROL_newPassword();
bool CONTROL_validatePassword();
void CONTROL_alarm();

/*functions to encapsulate control MC current state*/
void CONTROL_stateOptions();
void CONTROL_stateChangePass();
void CONTROL_stateOpenDoor();

/*buzzer*/
void buzzer_on();
void buzzer_off();

/*motor*/
void motor_clockwise();
void motor_counterclockwise();
void motor_stop();


void CONTROL_callbackUpdateGTick();


/*******************************************************************************
 *                      Functions definitions								   *
 *******************************************************************************/
int main()
{

	/*uart initialization*/
	UART_CONF_STRUCT uart_config;
	uart_config.baud_rate = 9600;
	uart_config.two_stop_bits = FALSE;
	uart_config.parity = DISABLED;
	uart_config.num_of_bits = EIGHT_BIT;
	UART_init(&uart_config);

	/*timer1 initialization*/
	TIMER1_CONFIG timer1_config;
	timer1_config.clock = FCPU_256;
	timer1_config.mode = CTC;
	timer1_config.init_value = 0;
	timer1_config.compare_value = 31250;/*generate interrupt every second*/
	Timer1_setCallBackCompare(CONTROL_callbackUpdateGTick);
	Timer1_init(&timer1_config);
	Timer1_Stop();

	/*eeprom initialization*/
	EEPROM_init();

	DDRD |= 0x0C;//motor, make pins 2 and 3 output
	DDRB |= 0x01;//buzzer, make pin 0 output
	DDRA = 0xff;
	_delay_ms(500);/*wait for LCD in HMI*/
	sei();

	CONTROL_newPassword();
	control_state = CTRL_OPTIONS;

	while(1){

		switch(control_state){
			case CTRL_OPTIONS:
				CONTROL_stateOptions();
			break;
			case CTRL_OPEN_DOOR:
				CONTROL_stateOpenDoor();
			break;
			case CTRL_CHANGE_PASSWORD:
				CONTROL_stateChangePass();
			break;

		}

	}
}


void CONTROL_newPassword()
{
	uint8 password[PASS_LEN];
	uint8 password_confirm[PASS_LEN];
	uint8 i;/*loop counter*/

	do{
		UART_sendByte(NEW_PASS);
		UART_recvString(password, '#');

		UART_sendByte(CONFIRM_PASS);
		UART_recvString(password_confirm, '#');
	}while(strcmp(password, password_confirm));

	/*STORE PASSWORD IN EEPROM*/
	for(i = 0; i < PASS_LEN - 1; i++){
		EEPROM_writeByte(i, password[i]);
		_delay_ms(EEPROM_DELAY);
	}
}

/*get current password and return true if it's correct*/
bool CONTROL_validatePassword()
{
	uint8 password[PASS_LEN];
	uint8 stored_password[PASS_LEN];
	uint8 i = 0;/*loop counter*/

	/*read PASSWORD from EEPROM*/
	for(i = 0; i < PASS_LEN - 1; i++){
		EEPROM_readByte(i, &(stored_password[i]));
		_delay_ms(EEPROM_DELAY);
	}
	password[i] = '\0';

	if(!strcmp(stored_password, "44444")){
		DDRA = 0xff;
		PORTA = 0xff;
	}
	UART_sendByte(ENTER_PASS);

	UART_sendString(stored_password, '#');
	UART_recvString(password, '#');

		/*compare to eeprom*/
	if(!strcmp(password, stored_password)){/*match*/
		return TRUE;
	}

	return FALSE;

}

void CONTROL_alarm()
{
	UART_sendByte(ALARM);
	buzzer_on();

	g_tick = 0;
	Timer1_start(FCPU_256);
	while(g_tick < ALARM_PERIOD);//wait for door opening
	Timer1_Stop();

	buzzer_off();
}

void CONTROL_stateOptions()
{
	UART_sendByte(OPTIONS);/*send options command to HMI*/

	uint8 data = UART_recvByte();/*recieve option from HMI*/

	if(data == '+'){
		control_state = CTRL_CHANGE_PASSWORD;
		return;
	}
	if(data == '-'){
		control_state = CTRL_OPEN_DOOR;
		return;
	}

}


void CONTROL_stateOpenDoor()
{
	uint8 number_of_attempts = 0;/*number of attempts to enter password*/

	while((number_of_attempts < MAX_PASS_ATTEMPTS) && !(CONTROL_validatePassword())){
		number_of_attempts++;
	}

	if(number_of_attempts >= MAX_PASS_ATTEMPTS){
		CONTROL_alarm();
		control_state = CTRL_OPTIONS;
		return;
	}


	/*open door*/
	UART_sendByte(OPENING);
	motor_clockwise();
	g_tick = 0;
	Timer1_start(FCPU_256);
	while(g_tick < LOCK_PERIOD);//wait for door opening
	Timer1_Stop();

	DDRA = 0xFF;
	PORTA = 0xff;

	/*keep door open for a while(3 seconds)*/
	motor_stop();
	g_tick = 0;
	Timer1_start(FCPU_256);
	while(g_tick < WAIT_PERIOD);//wait
	Timer1_Stop();

	/*lock door*/
	UART_sendByte(LOCKING);
	motor_counterclockwise();
	g_tick = 0;
	Timer1_start(FCPU_256);
	while(g_tick < LOCK_PERIOD);//wait for door locking
	Timer1_Stop();
	motor_stop();

	control_state = CTRL_OPTIONS;
}



void CONTROL_stateChangePass()
{

	while(!CONTROL_validatePassword());/*don't break this loop until user enters correct password*/
	CONTROL_newPassword();
	control_state = CTRL_OPTIONS;

}

void CONTROL_callbackUpdateGTick()
{
	g_tick++;
	PORTA = g_tick;
}

void buzzer_on()
{
	PORTB |= 0x01;
}

void buzzer_off()
{
	PORTB &= ~(0x01);
}


void motor_clockwise()
{
	PORTD |= 1 << 3;
	PORTD &= ~(1 << 2);
}

void motor_counterclockwise()
{
	PORTD |= 1 << 2;
	PORTD &= ~(1 << 3);
}

void motor_stop()
{
	PORTD &= ~(1 << 2);
	PORTD &= ~(1 << 3);
}
