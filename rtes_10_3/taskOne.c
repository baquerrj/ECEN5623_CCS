/*
 * taskOne.c
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
#include "taskOne.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "rtes_10_3.h"

#define TASKONESTACKSIZE        128         // Stack size in words

extern xSemaphoreHandle g_pUARTSemaphore;
extern xSemaphoreHandle g_pTaskSemaphore;
extern void calculateFibonacciNumbers( uint32_t terms );

static void taskOne( void *pvParameters )
{
   int32_t processingTicks = 0;
   portTickType wakeTick = 0;
   portTickType doneTick = 0;

   while ( 1 )
   {
      if ( pdPASS == xSemaphoreTake( g_p30MsTaskSemaphore, pdMS_TO_TICKS(30) ) )
      {
         wakeTick = xTaskGetTickCount();
         UARTPRINTF( "\n***** TASK ONE *****\n" );
         calculateFibonacciNumbers( 22 );
         doneTick = xTaskGetTickCount();
         processingTicks = doneTick - wakeTick;
         UARTPRINTF( "TASK ONE: done after %d ticks, %d ms\n", processingTicks, processingTicks / portTICK_PERIOD_MS );
         taskYIELD();
      }
   }
   vTaskDelete( NULL );
}

uint32_t TaskOneInit( void )
{
   if ( xTaskCreate( taskOne,
         (const portCHAR *)"TaskOne",
         TASKONESTACKSIZE,
         NULL,
         tskIDLE_PRIORITY + PRIORITY_TASK_ONE,
         NULL) != pdTRUE )
   {
      return 1;
   }

   return 0;

}
