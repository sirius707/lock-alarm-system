/*
 * main.c
 *
 *  Created on: 20 Feb 2021
 *      Author: Mostafa
 */

#define F_CPU 8000000

#include "hmi.h"
#include <stdlib.h>

int main()
{
	CONTROL_COMMAND control_cmd = NEW_PASS;

	HMI_init();
	sei();

	while(1){
		control_cmd = HMI_recieveCmd();
		HMI_execCmd(control_cmd);
	}
}
