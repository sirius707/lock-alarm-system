 /******************************************************************************
 *
 * Module: Timer1
 *
 * File Name: timer.h
 *
 * Description: Header file for the Timer1 driver
 *
 * Author: Mostafa Nabil
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include <stdlib.h>
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                           Data Types                                        *
 *******************************************************************************/

typedef enum
{
	NO_PRESCALER = 1,FCPU_8 = 2,FCPU_64 = 3,FCPU_256 = 4,FCPU_1024 = 5
}TIMER1_PRESCALER;

typedef enum
{
	NORMAL,CTC = 8
}TIMER1_MODE;

/*timer1 configuration struct*/
typedef struct
{
	TIMER1_MODE mode;
	TIMER1_PRESCALER clock;
	uint16 init_value;
	uint16 compare_value;
}TIMER1_CONFIG;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void Timer1_init(TIMER1_CONFIG *timer1_config);
void Timer1_start(TIMER1_PRESCALER clock);
void Timer1_setCallBackNormal(void(*a_ptr)(void));
void Timer1_setCallBackCompare(void(*a_ptr)(void));
uint16 Timer1_value(void);
void Timer1_Stop(void);
void Timer1_reset(void);

#endif /* TIMER_H_ */
