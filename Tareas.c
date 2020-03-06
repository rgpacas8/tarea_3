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

	while (1)
	{
		seconds++;
		if (seconds >= 60) {
		seconds = 0;
		xSemaphoreGive(parameters_task.minutes_semaphore);// Libera semaforo de minutos
		}

		//PRINTF("\rTime: %d seconds since reset\n", seconds);
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

	while (1)
	{
		xSemaphoreTake(parameters_task.minutes_semaphore,portMAX_DELAY);//Esperando
		minutes++;
		if (minutes >= 60) {
			xSemaphoreGive(parameters_task.hours_semaphore); // Libera semaforo de minutos
			minutes = 0;
		}
		//PRINTF("\rTime: %d minutes since reset\n", minutes);
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
		xSemaphoreTake(parameters_task.hours_semaphore, portMAX_DELAY); //Esperando
		hours++;
		if (hours >= 24) {
			hours = 0;
		}
		//PRINTF("\rTime: %d hours since reset\n", hours);
		msg.value = hours;
		xQueueSend(parameters_task.time_queue, &msg, portMAX_DELAY);
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
	time_msg_t received_msg;
	static uint8_t horas = 0, minutos = 0, segundos = 0;

	parameters_task_t parameters_task = *((parameters_task_t*) data);

	while (1) {
		xQueueReceive(parameters_task.time_queue, &received_msg,
				portMAX_DELAY);

	//	xSemaphoreTake(parameters_task.mutex_UART, portMAX_DELAY);
		switch (received_msg.time_type) {
		case seconds_type:
			segundos = received_msg.value;
			//PRINTF("Seconds sent: %d \n",received_msg.value);
			break;
		case minutes_type:
			minutos = received_msg.value;
			//PRINTF("Minutes sent: %d \n",received_msg.value);
			break;
		case hours_type:
			horas = received_msg.value;
			//PRINTF("Hours sent: %d\n ",received_msg.value);
			break;
		default:
			PRINTF("\rError\n");
			break;
		}
		PRINTF("Reloj: %d : %d : %d\n ", horas, minutos, segundos);
	//	xSemaphoreGive(parameters_task.mutex_UART);
	}
}
