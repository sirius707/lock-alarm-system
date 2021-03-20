/******************************************************************************
 *
 * Module: TWI
 *
 * File Name: eeprom.h
 *
 * Description: header file for eeprom
 *
 * Author: Mostafa Nabil
 *
 *******************************************************************************/

#ifndef EEPROM_H_
#define EEPROM_H_

#include "std_types.h"
#include "common_macros.h"


/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

#define SUCCESS 1
#define FAIL 0

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void EEPROM_init();
uint8 EEPROM_writeByte(uint16 address, uint8 data);
uint8 EEPROM_readByte(uint16 address, uint8 *data);


#endif /* EEPROM_H_ */
