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
extern portTickType g_wakeTick;

static void calculateFibonacciNumbers( uint32_t terms )
{
   uint32_t n1 = 0;
   uint32_t n2 = 1;
   uint32_t i = 0;
   uint32_t nextTerm = 0;
   for ( i = 1; i < terms; ++i )
   {
      UARTPRINTF( "%d, ", n1 );
      nextTerm = n1 + n2;
      n1 = n2;
      n2 = nextTerm;
   }
   UARTPRINTF( "\n" );
}

static void taskTwo( void *pvParameters )
{
   int32_t ticksBetweenWaking = 0;
   int32_t processingTicks = 0;
   portTickType wakeTick = 0;
   portTickType lastWakeTick = 0;
   portTickType doneTick = 0;

   while ( 1 )
   {
      //UARTPRINTF( "Waiting for task semaphore\n" );
      xSemaphoreTake( g_pTaskSemaphore, portMAX_DELAY );
      wakeTick = xTaskGetTickCount();
      ticksBetweenWaking = wakeTick - lastWakeTick;
      calculateFibonacciNumbers( 35 );
      //UARTPRINTF( "Task Two Woke Up after %d ticks, %d ms\n", ticksBetweenWaking, ticksBetweenWaking / portTICK_PERIOD_MS );
      doneTick = xTaskGetTickCount();
      processingTicks = doneTick - wakeTick;
      UARTPRINTF( "Task Two Done after %d ticks, %d ms\n", processingTicks, processingTicks / portTICK_PERIOD_MS );
      lastWakeTick = wakeTick;
      xSemaphoreGive( g_pTaskSemaphore );
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
