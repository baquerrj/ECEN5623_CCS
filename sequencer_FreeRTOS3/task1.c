/*
 * task1.c
 *
 *  Created on: Mar 15, 2020
 *      Author: baquerrj
 */

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "drivers/buttons.h"
#include "driverlib/timer.h"
#include "utils/uartstdio.h"
#include "task1.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "sequencer.h"

#define TASKONESTACKSIZE        128         // Stack size in words

extern xSemaphoreHandle g_pUARTSemaphore;
extern xSemaphoreHandle pSemaphoreS1;

extern bool abortS1;
extern uint32_t count;

static TaskHandle_t taskHandle;
static void taskOne( void *pvParameters )
{
#ifdef ISR_TIMING
   while ( !abortS1 )
   {
      if ( g_wakeTick == g_lastWakeTick )
      {
         UARTPRINTF( "Iteration [%u]: tick [%u]\n", count, g_wakeTick );
         g_lastWakeTick = g_wakeTick;
      }
   }
#else
   portTickType wakeTick = 0;
   portTickType doneTick = 0;
   portTickType wcet = 0;
   uint32_t releases = 0;
   const char * taskName = ( const char* ) pcTaskGetName( taskHandle );

   TASKLOGTIME( taskName, releases, getTimeFromTimer() );
   while ( !abortS1 )
   {
      if ( pdPASS == xSemaphoreTake( pSemaphoreS1, portMAX_DELAY ) )
      {
         wakeTick = getTimeFromTimer();
         releases++;
         TASKLOGTIME( taskName, releases, wakeTick );
         doneTick = getTimeFromTimer();
         wcet = getTimeDifference( wakeTick, doneTick );

//         xSemaphoreGive( pSemaphoreS1 );
      }
   }
#endif
   UARTPRINTF( "\n*** %s: %d times WCET = %d ms ***\n", taskName, releases, wcet );
   vTaskDelete( NULL );
}

uint32_t TaskOneInit( void )
{
   taskHandle = NULL;
   if ( xTaskCreate( taskOne,
         (const portCHAR *)"S1",
         TASKONESTACKSIZE,
         NULL,
         PRIORITY_TASK_ONE,
         &taskHandle ) != pdTRUE )
   {
      return 1;
   }

   return 0;

}
