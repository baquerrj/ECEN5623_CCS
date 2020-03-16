/*
 * taskTwo.c
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
#include "taskTwo.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "rtes_10_2.h"

#define TASKTWOSTACKSIZE        128         // Stack size in words

extern xSemaphoreHandle g_pUARTSemaphore;
extern xSemaphoreHandle g_pTaskSemaphore;
extern void calculateFibonacciNumbers( uint32_t terms );

static void taskTwo( void *pvParameters )
{
   int32_t processingTicks = 0;
   portTickType wakeTick = 0;
   portTickType doneTick = 0;

   while ( 1 )
   {
      xSemaphoreTake( g_pTaskSemaphore, portMAX_DELAY );
      wakeTick = xTaskGetTickCount();
      UARTPRINTF( "\n***** TASK TWO *****\n" );
      calculateFibonacciNumbers( 57 );
      doneTick = xTaskGetTickCount();
      processingTicks = doneTick - wakeTick;
      UARTPRINTF( "TASK TWO: done after %d ticks, %d ms\n", processingTicks, processingTicks / portTICK_PERIOD_MS );
      xSemaphoreGive( g_pTaskSemaphore );
      taskYIELD();
   }
   vTaskDelete( NULL );
}

uint32_t TaskTwoInit( void )
{
   if ( xTaskCreate( taskTwo,
         (const portCHAR *)"TaskTwo",
         TASKTWOSTACKSIZE,
         NULL,
         tskIDLE_PRIORITY + PRIORITY_TASK_TWO,
         NULL) != pdTRUE )
   {
      return 1;
   }

   return 0;

}
