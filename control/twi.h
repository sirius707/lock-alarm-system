 /******************************************************************************
 *
 * Module: TWI
 *
 * File Name: twi.h
 *
 * Description: header file for I2C
 *
 * Author: Mostafa Nabil
 *
 *******************************************************************************/


#ifndef TWI_H_
#define TWI_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                      			 macros                                    *
 *******************************************************************************/

/* twi status codes */
#define TWI_START         	0x08
#define TWI_REPSTART     	0x10
#define TWI_M_WRITE_REQ_ACK 0x18
#define TWI_M_READ_REQ_ACK  0x40
#define TWI_M_TRANSMIT_ACK  0x28
#define TWI_M_RCV_ACK   	0x50
#define TWI_M_RCV_NACK  	0x58

/*******************************************************************************
 *                           Data Types                                        *
 *******************************************************************************/
/*twi configuration struct*/
typedef struct
{
	uint8 baud_rate;
	uint8 address;
}TWI_CONFIG;

/*******************************************************************************
 *                      	      prototypes                                   *
 *******************************************************************************/
void TWI_init(const TWI_CONFIG const *config);
void TWI_start(void);
void TWI_stop(void);
void TWI_write(uint8 data);
uint8 TWI_readWithACK(void); //read with ack
uint8 TWI_readWithNACK(void); //read with nack
uint8 TWI_getStatus(void);


#endif /* TWI_H_ */
