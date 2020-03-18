/*
 * processingTask.c
 *
 *  Created on: Mar 15, 2020
 *      Author: baquerrj
 */

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "drivers/buttons.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "priorities.h"
#include "queue.h"
#include "rtes_10_1.h"
#include "semphr.h"
#include "task.h"
#include "utils/uartstdio.h"
#include "processingTask.h"

#define PROCESSINGTASKSTACKSIZE 128  // Stack size in words

extern xSemaphoreHandle g_pUARTSemaphore;
extern xSemaphoreHandle g_pTaskSemaphore;
extern portTickType g_wakeTick;

static void processingTask( void *pvParameters )
{
   portTickType elapsed = 0;
   portTickType lastWakeTick = 0;
   portTickType currentTick = 0;
   while ( 1 )
   {
      currentTick = xTaskGetTickCount();
      UARTPRINTF( "TASK: waiting on semaphore at %d ticks - %d ms\n", currentTick, currentTick / portTICK_PERIOD_MS );
      xSemaphoreTake( g_pTaskSemaphore, portMAX_DELAY );
      elapsed = g_wakeTick - lastWakeTick;
      UARTPRINTF( "TASK: got semaphore after %d ticks - %d ms\n", elapsed, elapsed / portTICK_PERIOD_MS );
      lastWakeTick = g_wakeTick;
   }
   vTaskDelete( NULL );
}

uint32_t ProcessingTaskInit( void )
{
   if ( xTaskCreate( processingTask,
         (const portCHAR *)"PROCTASK",
         PROCESSINGTASKSTACKSIZE,
         NULL,
         tskIDLE_PRIORITY + PRIORITY_PROCESSING_TASK,
         NULL ) != pdTRUE )
   {
      return 1;
   }

   return 0;
}
