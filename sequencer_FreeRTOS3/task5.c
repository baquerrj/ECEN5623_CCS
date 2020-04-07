/*
 * taskFive.c
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
#include "task5.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "sequencer.h"

#define TASKFIVESTACKSIZE        128         // Stack size in words

extern xSemaphoreHandle g_pUARTSemaphore;
extern xSemaphoreHandle pSemaphoreS5;

extern bool abortS5;

static TaskHandle_t taskHandle;
static void taskFive( void *pvParameters )
{
   portTickType wakeTick = 0;
   portTickType doneTick = 0;
   portTickType wcet = 0;
   uint32_t releases = 0;
   const char * taskName = ( const char* ) pcTaskGetName( taskHandle );

   TASKLOGTIME( taskName, releases, getTimeFromTimer() );
   while ( !abortS5 )
   {
      if ( pdPASS == xSemaphoreTake( pSemaphoreS5, portMAX_DELAY ) )
      {
         wakeTick = getTimeFromTimer();
         releases++;
         TASKLOGTIME( taskName, releases, wakeTick );
         doneTick = getTimeFromTimer();
         wcet = getTimeDifference( wakeTick, doneTick );

//         xSemaphoreGive( pSemaphoreS5 );
      }
   }
   TASKLOGEND( taskName, releases, wcet );
   vTaskDelete( NULL );
}

uint32_t TaskFiveInit( void )
{
   taskHandle = NULL;
   if ( xTaskCreate( taskFive, ( const portCHAR * ) "S5",
   TASKFIVESTACKSIZE,
                     NULL,
                     PRIORITY_TASK_FIVE,
                     &taskHandle ) != pdTRUE )
   {
      return 1;
   }

   return 0;

}
