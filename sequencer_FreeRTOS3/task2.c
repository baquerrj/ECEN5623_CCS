/*
 * task2.c
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
#include "task2.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "sequencer.h"

#define TASKTWOSTACKSIZE        128         // Stack size in words

extern xSemaphoreHandle g_pUARTSemaphore;
extern xSemaphoreHandle pSemaphoreS2;

extern bool abortS2;

static TaskHandle_t taskHandle;

static void taskTwo( void *pvParameters )
{
   portTickType wakeTick = 0;
   portTickType doneTick = 0;
   portTickType wcet = 0;
   uint32_t releases = 0;
   const char * taskName = ( const char* ) pcTaskGetName( taskHandle );

   TASKLOGTIME( taskName, releases, getTimeFromTimer() );
   while ( !abortS2 )
   {
      if ( pdPASS == xSemaphoreTake( pSemaphoreS2, portMAX_DELAY ) )
      {
         wakeTick = getTimeFromTimer();
         releases++;
         TASKLOGTIME( taskName, releases, wakeTick );
         doneTick = getTimeFromTimer();
         wcet = getTimeDifference( wakeTick, doneTick );
      }
   }
   logTaskWcet( taskName, releases, wcet );
//   TASKLOGEND( taskName, releases, wcet );
   vTaskDelete( NULL );
}

uint32_t TaskTwoInit( void )
{
   taskHandle = NULL;
   if ( xTaskCreate( taskTwo, ( const portCHAR * ) "S2",
   TASKTWOSTACKSIZE,
                     NULL,
                     PRIORITY_TASK_TWO,
                     &taskHandle ) != pdTRUE )
   {
      return 1;
   }

   return 0;

}
