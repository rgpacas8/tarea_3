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
	time_msg_t msg;
	msg.time_type = seconds_type;


	static uint32_t seconds = 0;
	TickType_t last_wake_time = xTaskGetTickCount();

	parameters_task_t parameters_task = *((parameters_task_t*) data);
	EventBits_t event_all_flags = 0;
	EventBits_t event_minu_flag = 0;

	while (1)
	{
		if ((event_minu_flag == TRUE)
						&& (seconds == parameters_task.alarm.second)) {
			xEventGroupSetBits(parameters_task.event_HH_MM_SS, EVENT_SECOND);
		}

		seconds++;
		if (seconds >= SEGUNDOS) {
			seconds = 0;
			xSemaphoreGive(parameters_task.minutes_semaphore); // Libera semaforo de minutos
		}

		event_all_flags = xEventGroupGetBits(parameters_task.event_HH_MM_SS);
		event_minu_flag = ((event_all_flags) & (EVENT_MINUTE)) >> 1;

		msg.value = seconds;
		xQueueSend(parameters_task.time_queue, &msg, portMAX_DELAY);
		vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(1000));
	}
}

void task_Minutes(void *data)
{

	    time_msg_t msg;
	    msg.time_type = minutes_type;
		static uint32_t minutes = 0;
		parameters_task_t parameters_task = *((parameters_task_t*) data);
		EventBits_t event_all_flags = 0;
		EventBits_t event_hora_flag = 0;

	while (1)
	{
		if ((event_hora_flag == TRUE)
						&& (minutes == parameters_task.alarm.minute)) {
			xEventGroupSetBits(parameters_task.event_HH_MM_SS, EVENT_MINUTE);
		}

		xSemaphoreTake(parameters_task.minutes_semaphore,portMAX_DELAY);//Esperando para tomar semaforo de minutos
		minutes++;
		if (minutes >= MINUTOS) {
			xSemaphoreGive(parameters_task.hours_semaphore); // Libera semaforo de horas
			minutes = 0;
		}

		event_all_flags = xEventGroupGetBits(parameters_task.event_HH_MM_SS);
		event_hora_flag = ((event_all_flags) & (EVENT_HOUR)) >> 2;

		msg.value = minutes;
		xQueueSend(parameters_task.time_queue, &msg, portMAX_DELAY);

	}//end while
}

void task_Hours(void *data)
{
	time_msg_t msg;
	msg.time_type = hours_type;
	static uint32_t hours = 0;
	parameters_task_t parameters_task = *((parameters_task_t*) data);

	while(1)
	{
		if (hours == parameters_task.alarm.hour) {
			xEventGroupSetBits(parameters_task.event_HH_MM_SS, EVENT_HOUR);
		}

		xSemaphoreTake(parameters_task.hours_semaphore, portMAX_DELAY); //Esperando
		hours++;
		if (hours >= HORAS) {
			hours = 0;
		}

		msg.value = hours;
		xQueueSend(parameters_task.time_queue, &msg, portMAX_DELAY);
	}
}

void task_Alarm(void *data)
{
	parameters_task_t parameters_task = *((parameters_task_t*)data);

	while (1) {
	 /*
	  * xEventGroupWaitBits(xEventGroup,
	  *		uxBitsToWaitFor, 			xClearOnExit, 	xWaitForAllBits,   xTicksToWait);
	  */
		xEventGroupWaitBits(parameters_task.event_HH_MM_SS,
				EVENT_HOUR | EVENT_MINUTE | EVENT_SECOND, pdFALSE, pdTRUE, portMAX_DELAY);

		xEventGroupClearBits(parameters_task.event_HH_MM_SS, EVENT_HOUR);
		xEventGroupClearBits(parameters_task.event_HH_MM_SS, EVENT_MINUTE);
		xEventGroupClearBits(parameters_task.event_HH_MM_SS, EVENT_SECOND);

		xSemaphoreTake(parameters_task.mutex_UART, portMAX_DELAY);
		PRINTF("--- --- --- --- ALARM --- --- --- ---\n ");
		xSemaphoreGive(parameters_task.mutex_UART);
	}
}

void task_Print(void *data)
{
	time_msg_t received_msg;
	static uint8_t horas = 0, minutos = 0, segundos = 0;

	parameters_task_t parameters_task = *((parameters_task_t*) data);

	while (1) {
		xQueueReceive(parameters_task.time_queue, &received_msg,
		portMAX_DELAY);

		xSemaphoreTake(parameters_task.mutex_UART, portMAX_DELAY);
		switch (received_msg.time_type)
		{
			case seconds_type:
				segundos = received_msg.value;
				break;
			case minutes_type:
				minutos = received_msg.value;
				break;
			case hours_type:
				horas = received_msg.value;
				break;
			default:
				PRINTF("\rError\n");
				break;
		}
		PRINTF("Reloj: %d : %d : %d\n ", horas, minutos, segundos);
		xSemaphoreGive(parameters_task.mutex_UART);
	}
}
