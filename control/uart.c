 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: source file for the uart driver
 *
 * Author: Mostafa Nabil
 *
 *******************************************************************************/

#include "uart.h"


#define BAUD_PRESCALE(X)  ((F_CPU /((X)* 16UL)) - 1)
#define BAUD_PRESCALE_DOUBLE(X) (((F_CPU / (X * 8UL))) - 1)

/*******************************************************************************
 *                      Callback function pointers							   *
 *******************************************************************************/

volatile void (*UART_recvCompleteCallback)(uint8 recieved_data);
volatile void (*UART_transCompleteCallback)();


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void UART_init(const UART_CONF_STRUCT * const uart_config)
{
	/*enable uart reception and transmission*/
	UCSRB = (1 << TXEN) | (1 << RXEN);

	/*double speed*/
	SET_BIT(UCSRA, U2X);

	/* UCSZ1:0 = 11 For 8-bit data mode*/
	SET_BIT(UCSRC, URSEL);
	SET_BIT(UCSRC, UCSZ0);
	SET_BIT(UCSRC, UCSZ1);

	if(uart_config){


		/*Enable two stop bits*/
		if(uart_config->two_stop_bits){
			SET_BIT(UCSRC, URSEL);
			SET_BIT(UCSRC, USBS);
		}

		/*set number of bits, if 0 default to 8 bits*/
		if(uart_config->num_of_bits){
			UCSRC |= (uart_config->num_of_bits & 0x03) << UCSZ0;

			if(uart_config->num_of_bits == NINE_BIT){
				SET_BIT(UCSRB, UCSZ2);
			}
		}

		/*adjust parity accordingly if parity isn't disabled*/
		if(uart_config->parity){
					UCSRC |= (uart_config->parity & 0x03) << UPM0;
		}

		/*set baud rate if the specified baud rate is greater than 0*/
		if(uart_config->baud_rate > 0){
					CLEAR_BIT(UCSRC, URSEL);
					UBRRH = BAUD_PRESCALE_DOUBLE(uart_config->baud_rate) >> 8;
					UBRRL = BAUD_PRESCALE_DOUBLE(uart_config->baud_rate);
					return;
		}
	}

	/*default settings if a config struct isn't provided*/
	CLEAR_BIT(UCSRC, URSEL);
	UBRRH = BAUD_PRESCALE_DOUBLE(DEFAULT_BAUD_RATE) >> 8;
	UBRRL = BAUD_PRESCALE_DOUBLE(DEFAULT_BAUD_RATE);
}


void UART_sendByte(uint8 data)
{
	while(BIT_IS_CLEAR(UCSRA, UDRE));
	UDR = data;
}


uint8 UART_recvByte(void)
{
	while(BIT_IS_CLEAR(UCSRA, RXC));
	return UDR;
}

void UART_sendString(const char *str, char terminator)
{
	uint8 i = 0;
	while(str[i] != '\0')
	{
		UART_sendByte(str[i]);
		i++;
	}
	UART_sendByte(terminator);
}


void UART_recvString(char *str, char terminator)
{
	uint8 i = 0;
		str[i] = UART_recvByte();
		while(str[i] != terminator)
		{
			i++;
			str[i] = UART_recvByte();
		}
		str[i] = '\0';
}



