/*
 * task4.c
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
#include "task4.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "sequencer.h"

#define TASKFOURSTACKSIZE        128         // Stack size in words

extern xSemaphoreHandle g_pUARTSemaphore;
extern xSemaphoreHandle pSemaphoreS4;

extern bool abortS4;

static TaskHandle_t taskHandle;
static void taskFour( void *pvParameters )
{
   portTickType wakeTick = 0;
   portTickType doneTick = 0;
   portTickType wcet = 0;
   uint32_t releases = 0;
   const char * taskName = ( const char* ) pcTaskGetName( taskHandle );

   TASKLOGTIME( taskName, releases, getTimeFromTimer() );
   while ( !abortS4 )
   {
      if ( pdPASS == xSemaphoreTake( pSemaphoreS4, portMAX_DELAY ) )
      {
         wakeTick = getTimeFromTimer();
         releases++;
         TASKLOGTIME( taskName, releases, wakeTick );
         doneTick = getTimeFromTimer();
         wcet = getTimeDifference( wakeTick, doneTick );

//         xSemaphoreGive( pSemaphoreS4 );
      }
   }
   TASKLOGEND( taskName, releases, wcet );
   vTaskDelete( NULL );
}

uint32_t TaskFourInit( void )
{
   taskHandle = NULL;
   if ( xTaskCreate( taskFour, ( const portCHAR * ) "S4",
   TASKFOURSTACKSIZE,
                     NULL,
                     PRIORITY_TASK_FOUR,
                     &taskHandle ) != pdTRUE )
   {
      return 1;
   }

   return 0;

}
