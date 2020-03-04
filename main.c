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
#include "task.h"
#include "queue.h"
#include "Tareas.h"
/* TODO: insert other definitions and declarations here. */
/*
static void task_Seconds( void *pvParameters );
static void task_Minutes( void *pvParameters );
static void task_Hours  ( void *pvParameters );
*/
const char *pcTextForSeconds1 = "Task 1 is running\r\n";
const char *pcTextForMinutes2 = "Task 2 is running\r\n";
const char *pcTextForHours3   = "Task 3 is running\r\n";
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    PRINTF("Hello World\n");

    /* Create the first task at priority 1... */
    /* xTaskCreate uses:
     * 		Pointer to the function that implements the task.
     * 		Text name for the task.  This is to facilitate debugging only.
     * 		Stack depth - most small microcontrollers will use much less stack than this.
     * 		Task parameter, Pasamos una cadena de texto para DEBUG
     * 		Priority
     * 		We are not using the task handle.
     * 		*/

    xTaskCreate( task_Seconds, "task_Seconds", 500, (void*)pcTextForSeconds1, 1, NULL );
    xTaskCreate( task_Minutes, "task_Minutes", 500, (void*)pcTextForMinutes2, 1, NULL );
    xTaskCreate( task_Hours  , "task_Hours"  , 500, (void*)pcTextForHours3, 1, NULL );

    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {

    }
    return 0 ;
}
