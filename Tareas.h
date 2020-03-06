/*
 * Tareas.h
 *
 *  Created on: Mar 3, 2020
 *      Author: David Ruiz
 */

#ifndef TAREAS_H_
#define TAREAS_H_

/* Other include files here. */
#include "MK66F18.h"
#include "FreeRTOS.h"

#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "event_groups.h"

/*! This enumerated constant are used to select the TASK to be used*/
typedef enum {
	seconds_type, minutes_type, hours_type
} time_types_t;

typedef struct {
	time_types_t time_type;	//ID
	uint8_t value;//DATO de tiempo
} time_msg_t;


/* Other definitions and declarations here. */
#define EVENT_HOUR	    (1 << 2)
#define EVENT_MINUTE    (1 << 1)
#define EVENT_SECOND    (1 << 0)

#define DEBUG_ON	 	//** For debugging purposes */
//#define DEBUG_OFF		//** For normal execution */

#ifdef DEBUG_ON
	#define HORAS		3
	#define MINUTOS		3
	#define SEGUNDOS	3
#endif

#ifdef DEBUG_OFF
	#define HORAS		24
	#define MINUTOS		60
	#define SEGUNDOS	60

#endif

typedef enum
{
	FALSE = 0, TRUE = 1
} boolean_t;

typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
} alarmClock_t;

typedef struct {
	SemaphoreHandle_t minutes_semaphore;	/*Semaforos*/
	SemaphoreHandle_t hours_semaphore;

	QueueHandle_t time_queue;				/*Queue de mailbox*/
	EventGroupHandle_t event_HH_MM_SS;		/*Event Group*/
	SemaphoreHandle_t mutex_UART;			/*Mutex para UART*/

	alarmClock_t alarm;						/*Alarma*/
} parameters_task_t;



/*! All the task declarations of the application are here*/
void task_Seconds(void *data);

void task_Minutes(void *data);

void task_Hours(void *data);

void task_Alarm(void *data);

void task_Print(void *data);



#endif /* TAREAS_H_ */
