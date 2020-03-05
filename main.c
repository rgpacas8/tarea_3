/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 */

/*
 * @file:			AlarmaFreeRTOS.c
 * @brief:   		Tarea3_ALARMA_in_FreeRTOS_OS_for_FRDM-K66F
 *
 * @company:			  ITESO
 * @Engineer Team:	 D.F.R. / R.G.P.
 * @contact:		ie717807@iteso.mx
 * @contact:		ie706818@iteso.mx
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"

/* TODO: insert other include files here. */
#include "FreeRTOS.h"
#include "Tareas.h"

/* TODO: insert other definitions and declarations here. */
#define SET_HOUR   0
#define SET_MINUTE 1
#define SET_SECOND 15

#define EVENT_HOUR	    (1 << 2)
#define EVENT_MINUTE    (1 << 1)
#define EVENT_SECOND    (1 << 0)

int main(void) {
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();

	PRINTF("Hello World\n");

	/* Crear funciones de FreeRTOS: 2 semaforos, 1 queue, 1 mutex, 1 event group
	 * y 1 variable de struct alarm para Setear una hora de Alarma */
	static parameters_task_t parameters_task;

	parameters_task.minutes_semaphore = xSemaphoreCreateBinary();
	parameters_task.hours_semaphore   = xSemaphoreCreateBinary();

	parameters_task.time_queue = xQueueCreate(5, sizeof(time_msg_t));
	parameters_task.mutex_UART = xSemaphoreCreateMutex();
	parameters_task.event_HH_MM_SS = xEventGroupCreate();

	// Se setea una hora de alarma con los defines de tiempo declarados antes //
	parameters_task.alarm.hour   = 	SET_HOUR;
	parameters_task.alarm.minute = 	SET_MINUTE;
	parameters_task.alarm.second = 	SET_SECOND;

	/* xTaskCreate uses:
	 * 		- Pointer to the function that implements the task.
	 * 		- Text name for the task.  This is to facilitate debugging only.
	 * 		- Stack depth - most small microcontrollers will use much less stack than this.
	 * 		- Task parameter, Pasamos una cadena de texto para DEBUG
	 *
	 * 		- Priority
	 * 		- We are not using the task handle.
	 * 		*/

	xTaskCreate(task_Seconds, "task_Seconds", 200, (void*) &parameters_task,
			configMAX_PRIORITIES, NULL);

	xTaskCreate(task_Minutes, "task_Minutes", 200, (void*) &parameters_task,
			configMAX_PRIORITIES, NULL);

//	xTaskCreate(task_Hours, "task_Hours", 	  200, (void*) &parameters_task,
//			configMAX_PRIORITIES, NULL);

//	xTaskCreate(task_Alarm, "task_Alarm", 	  200, (void*) &parameters_task,
//			configMAX_PRIORITIES, NULL);

//	xTaskCreate(task_Print, "task_Print", 	  200, (void*) &parameters_task,
//			configMAX_PRIORITIES, NULL);


	/* Start the scheduler so the created tasks start executing. */
	vTaskStartScheduler();


	/* Infinite loop is fake or debug tool in possible mistakes. */
	while (1) {

		// If your code arrive here, that is so bad :(

	}

	return 0;
}
