/******************************************************************************
 *
 * Module: TWI
 *
 * File Name: eeprom.c
 *
 * Description: source  file for eeprom
 *
 * Author: Mostafa Nabil
 *
 *******************************************************************************/

#include "twi.h"
#include "eeprom.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void EEPROM_init()
{
	TWI_CONFIG twi_config;
	twi_config.address = 0x01;
	twi_config.baud_rate = 0x02;

	TWI_init(&twi_config);
}

uint8 EEPROM_writeByte(uint16 address, uint8 data)
{
	TWI_start();
	TWI_write(0xA0 | ((address & 0x0700) >> 7));
	TWI_write(address & 0x00FF);

	TWI_write(data);

	if(TWI_getStatus() != TWI_M_TRANSMIT_ACK) return FAIL;

	TWI_stop();
	return SUCCESS;
}


uint8 EEPROM_readByte(uint16 address, uint8 *data)
{
	TWI_start();
	TWI_write(0xA0 | ((address & 0x0700) >> 7));
	TWI_write(address & 0x00FF);

	TWI_start();
	TWI_write(0xA0 | ((address & 0x0700) >> 7) | 1);

	*data = TWI_readWithNACK();
	if(TWI_getStatus() != TWI_M_RCV_NACK) return FAIL;

	TWI_stop();
	return SUCCESS;
}
