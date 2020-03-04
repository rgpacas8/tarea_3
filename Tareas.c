/*
 * Tareas.c
 *
 *  Created on: Mar 3, 2020
 *      Author: David Ruiz
 */

#include "Tareas.h"

/*! This enumerated constant are used to select the TASK to be used*/
typedef enum {seconds_type, minutes_type, hours_type} time_types_t;

typedef struct
{
	time_types_t time_type;
	uint8_t value;
} time_msg_t;

void task_Seconds(void *data)
{

}

void task_Minutes(void *data)
{

}

void task_Hours(void *data)
{

}

void task_Alarm(void *data)
{

}

void task_Print(void *data)
{

}
