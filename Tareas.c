/*
 * Tareas.c
 *
 *  Created on: Mar 3, 2020
 *      Author: David Ruiz
 */

#include "Tareas.h"

/*! This enumerated constant are used to select the TASK to be used*/
typedef enum {
	seconds_type, minutes_type, hours_type
} time_types_t;

typedef struct {
	SemaphoreHandle_t minutes_semaphore;
	EventGroupHandle_t event_FreeRtos;
	QueueHandle_t time_queue;
	SemaphoreHandle_t mutex_UART_freertos;

} parameters_task_t;

typedef struct {
	time_types_t time_type;
	uint8_t value;
} time_msg_t;


const char *pcTextForSeconds1 = "Task 1 is running\r\n";
const char *pcTextForMinutes2 = "Task 2 is running\r\n";
const char *pcTextForHours3   = "Task 3 is running\r\n";


void task_Seconds(void *data)
{
	const char timer_msg[] = "task Seconds: ";

	time_msg_t msg;
	msg.time_type = seconds_type;
	msg.value = timer_msg;

	uint32_t seconds = 0;
	parameters_task_t parameters_task = *((parameters_task_t*) data);

	TickType_t last_wake_time = xTaskGetTickCount();

	for(;;)
	{
		seconds++;

		if (seconds == 60){
			xSemaphoreGive(parameters_task.minutes_semaphore);	// Libera semaforo de minutos
			seconds = 0;
		}

		xQueueSend(parameters_task.time_queue, &msg, portMAX_DELAY);

		PRINTF("\rTime: %i seconds since reset\n",seconds);



		vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(1000));
	}
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
