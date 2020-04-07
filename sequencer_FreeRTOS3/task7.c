/*
 * task7.c
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
#include "task7.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "sequencer.h"

#define TASKSEVENSTACKSIZE        128         // Stack size in words

extern xSemaphoreHandle g_pUARTSemaphore;
extern xSemaphoreHandle pSemaphoreS7;

extern bool abortS7;

static TaskHandle_t taskHandle;
static void taskSeven( void *pvParameters )
{
   portTickType wakeTick = 0;
   portTickType doneTick = 0;
   portTickType wcet = 0;
   uint32_t releases = 0;
   const char * taskName = ( const char* ) pcTaskGetName( taskHandle );

   TASKLOGTIME( taskName, releases, getTimeFromTimer() );
   while ( !abortS7 )
   {
      if ( pdPASS == xSemaphoreTake( pSemaphoreS7, portMAX_DELAY ) )
      {
         wakeTick = getTimeFromTimer();
         releases++;
         TASKLOGTIME( taskName, releases, wakeTick );
         doneTick = getTimeFromTimer();
         wcet = getTimeDifference( wakeTick, doneTick );
//         xSemaphoreGive( pSemaphoreS7 );
      }
   }
   TASKLOGEND( taskName, releases, wcet );
   vTaskDelete( NULL );
}

uint32_t TaskSevenInit( void )
{
   taskHandle = NULL;
   if ( xTaskCreate( taskSeven, ( const portCHAR * ) "S7",
   TASKSEVENSTACKSIZE,
                     NULL,
                     PRIORITY_TASK_SEVEN,
                     &taskHandle ) != pdTRUE )
   {
      return 1;
   }

   return 0;

}
