/******************************************************************************
 *
 * Module: Timer1
 *
 * File Name: timer.c
 *
 * Description: Source file for the Timer1 driver
 *
 * Author: Mostafa Nabil
 *
 *******************************************************************************/

#include "timer.h"

/*******************************************************************************
 *                     callback function pointers                              *
 *******************************************************************************/
static volatile void (*g_Timer1NormalCallBackPtr)(void) = NULL;
static volatile void (*g_Timer1CTCCallBackPtr)(void) = NULL;


/*******************************************************************************
 *                      ISRs				                                   *
 *******************************************************************************/

ISR(TIMER1_OVF_vect){
	if(g_Timer1NormalCallBackPtr != NULL)
	{
/* Call the Call Back function  when there are unread data in the receive buffer */
		(*g_Timer1NormalCallBackPtr)();
	}
}

ISR(TIMER1_COMPA_vect){
	if(g_Timer1CTCCallBackPtr != NULL)
	{
		/* Call the Call Back function  when the transmit buffer is ready to receive new data */
		(*g_Timer1CTCCallBackPtr)();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


/*
 * Function responsible for initializing Timer1.
 */
void Timer1_init(TIMER1_CONFIG *timer1_config){

	TCNT1 = timer1_config->init_value;
	OCR1A = timer1_config->compare_value;

	if(timer1_config->mode == CTC){
		TIMSK |= (1<<OCIE1A); /* Enable Timer1 Compare A Interrupt */
		TCCR1A = (1<<FOC1A);
		TCCR1B = (1<<WGM12);
	}else{
		TIMSK |= (1<<TOIE1);
	}

	TCCR1B |= (timer1_config->clock);



}
void Timer1_start(TIMER1_PRESCALER clock){
	TCNT1=0;
	TCCR1B&=0XF8;
	TCCR1B|=clock;
}
void Timer1_setCallBackNormal(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_Timer1NormalCallBackPtr = a_ptr;
}
void Timer1_setCallBackCompare(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_Timer1CTCCallBackPtr = a_ptr;
}


void Timer1_Stop(void){
	TCCR1B &= ~(0X07);
}

void Timer1_reset(void){
	TCNT1 = 0;
}


uint16 Timer1_value(void){
	return TCNT1;
}


