/*
 * Tareas.h
 *
 *  Created on: Mar 3, 2020
 *      Author: David Ruiz
 */

#ifndef TAREAS_H_
#define TAREAS_H_

#include "MK66F18.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"


void task_Seconds(void *data);

void task_Minutes(void *data);

void task_Hours(void *data);

void task_Alarm(void *data);

void task_Print(void *data);


#endif /* TAREAS_H_ */
