 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: hmi.h
 *
 * Description: Header file for the HMI driver
 *
 * Author: Mostafa Nabil
 *
 *******************************************************************************/

#ifndef HMI_H_
#define HMI_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"


/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

#define LOCK_PERIOD 15U /*duration for opening and locking the door*/
#define WAIT_PERIOD 3U	/*duration for keeping the door open*/
#define ALARM_PERIOD 60U /*ALARM DURATION*/


/*******************************************************************************
 *                      	Data types		                                   *
 *******************************************************************************/
/*commands recieved from control MC through UART*/
typedef enum{NEW_PASS, ENTER_PASS, OPENING, LOCKING, ALARM, OPTIONS, CONFIRM_PASS}CONTROL_COMMAND;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void HMI_init();

uint8 HMI_recieveCmd();/*recieves command from CONTROL MC through UART*/
void HMI_execCmd(CONTROL_COMMAND  control_cmd);/*execute command recieved from UART*/

void HMI_newPass();
void HMI_confirmPass();
void HMI_enterPass();
void HMI_opening();
void HMI_locking();
void HMI_alarm();
void HMI_options();

#endif /* HMI_H_ */
