/*
 * Tareas.c
 *
 *  Created on: Mar 3, 2020
 *      Author: David Ruiz
 */

/* Other include files here. */
#include "Tareas.h"
#include "fsl_debug_console.h"


const char *pcTextForSeconds1 = "Task 1 is running\r\n";
const char *pcTextForMinutes2 = "Task 2 is running\r\n";
const char *pcTextForHours3   = "Task 3 is running\r\n";



void task_Seconds(void *data)
{
//	const char timer_msg[] = "task Seconds: ";

	time_msg_t msg;
	msg.time_type = seconds_type;
//	msg.value = timer_msg;

	static uint32_t seconds = 0;
	TickType_t last_wake_time = xTaskGetTickCount();

	parameters_task_t parameters_task = *((parameters_task_t*) data);

	while (1)
	{
		seconds++;
		if (seconds >= 10) {
		seconds = 0;
		xSemaphoreGive(parameters_task.minutes_semaphore);// Libera semaforo de minutos
		}

		//xQueueSend(parameters_task.time_queue, &msg, portMAX_DELAY);
		PRINTF("\rTime: %d seconds since reset\n", seconds);

		vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(1000));
	}
}

void task_Minutes(void *data)
{

	    time_msg_t msg;
	    msg.time_type = minutes_type;
//		msg.value = timer_msg;
		static uint32_t minutes = 0;
		parameters_task_t parameters_task = *((parameters_task_t*) data);

	while (1)
	{
		//xSemaphoregive(parameters_task.minutes_semaphore);
		xSemaphoreTake(parameters_task.minutes_semaphore,portMAX_DELAY);

		minutes++;

		if (minutes == 60) {
			//xSemaphoreGive(parameters_task.hours_semaphore); // Libera semaforo de minutos
			minutes = 0;
		}

		PRINTF("\rTime: %d minutes since reset\n", minutes);

	}//end while
}

void task_Hours(void *data)
{

	while(1)
	{

	}
}

void task_Alarm(void *data)
{

	while(1)
	{

	}
}

void task_Print(void *data)
{

	while(1)
	{

	}
}
