 /******************************************************************************
 *
 * Module: TWI
 *
 * File Name: twi.c
 *
 * Description: source file for I2C
 *
 * Author: Mostafa Nabil
 *
 *******************************************************************************/

#include "twi.h"


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


void TWI_init(const TWI_CONFIG const *config)
{
	TWSR = 0x00;/*no pre scaler*/
    TWBR = config->baud_rate;
	TWAR = (config->address) << 1;/*address*/
    TWCR = (1<<TWEN); /*enable twi */
}

void TWI_start(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);/*clear twint, entable start, enable twi*/
    while(BIT_IS_CLEAR(TWCR,TWINT));/*wait until start bit is sent*/
}

void TWI_stop(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);/*clear twint, entable stop, enable twi*/
}

void TWI_write(uint8 data)
{
    TWDR = data;/*store data in twi data register*/
    TWCR = (1 << TWINT) | (1 << TWEN);/*clear twint, enable twi*/
    while(BIT_IS_CLEAR(TWCR,TWINT));/*wait until data is sent*/
}

uint8 TWI_readWithACK(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);/*clear twint, enable twi, enable ack*/
    while(BIT_IS_CLEAR(TWCR,TWINT));/*wait until data is recieved*/

    return TWDR;
}

uint8 TWI_readWithNACK(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN);/*clear twint, enable twi*/
    while(BIT_IS_CLEAR(TWCR,TWINT));/*wait until data is recieved*/

    return TWDR;
}

uint8 TWI_getStatus(void)
{
    return (TWSR & 0xF8);
}
