 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the uart driver
 *
 * Author: Mostafa Nabil
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"
#include <stdlib.h>


/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define DEFAULT_BAUD_RATE 9600


/*******************************************************************************
 *                           Data Types                                        *
 *******************************************************************************/
typedef enum{ FIVE_BIT = 0x00, SIX_BIT = 0x01, SEVEN_BIT = 0x10, EIGHT_BIT = 0x11, NINE_BIT = 0x07} UART_BITS;
typedef enum{ DISABLED = 0x00, EVEN = 0x10, ODD = 0x01} PARITY_TYPE;

/*uart configuration struct*/
typedef struct UART_CONF_STRUCT{
	uint32 baud_rate;
	bool two_stop_bits;
	UART_BITS num_of_bits;
	PARITY_TYPE parity;
}UART_CONF_STRUCT;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void UART_init(const UART_CONF_STRUCT *const conf_struct);
void UART_sendByte(uint8 data);
uint8 UART_recvByte(void);
void UART_sendString(const char *str,  char terminator);
void UART_recvString(char *str, char terminator);

#endif /* UART_H_ */
