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
extern void calculateFibonacciNumbers( uint32_t terms, uint32_t iterations );
extern unsigned int _40MsIterations;

#if 1
static void taskTwo( void *pvParameters )
{
   int32_t processingTicks = 0;
   portTickType wakeTick = 0;
   portTickType doneTick = 0;
   uint32_t seqIterations = 50;

   while ( 1 )
   {
      xSemaphoreTake( g_pTaskSemaphore, portMAX_DELAY );
      UARTPRINTF( "\n***** TASK T2 *****\n" );
      wakeTick = xTaskGetTickCount();
      calculateFibonacciNumbers( seqIterations, _40MsIterations );
      doneTick = xTaskGetTickCount();
      processingTicks = doneTick - wakeTick;
      UARTPRINTF( "TASK T2: %d ticks - %d ms\n", processingTicks, processingTicks / portTICK_PERIOD_MS );
      xSemaphoreGive( g_pTaskSemaphore );
      taskYIELD();
   }
   vTaskDelete( NULL );
}
#else
static void taskTwo( void *pvParameters )
{
   int32_t processingTicks = 0;
   uint32_t wakeTick = 0;
   uint32_t doneTick = 0;
   unsigned int seqIterations = 50;

   while ( 1 )
   {
      xSemaphoreTake( g_pTaskSemaphore, portMAX_DELAY );
      UARTPRINTF( "\n***** TASK T2 *****\n" );
      wakeTick = TimerValueGet( TIMER0_BASE, TIMER_A );
      calculateFibonacciNumbers( seqIterations, _40MsIterations );
      doneTick = TimerValueGet( TIMER0_BASE, TIMER_A );
      processingTicks = wakeTick - doneTick;
      UARTPRINTF( "TASK T2: %d ticks - %d ms\n", processingTicks, processingTicks / 50000 );
      xSemaphoreGive( g_pTaskSemaphore );
      taskYIELD();
   }
   vTaskDelete( NULL );
}
#endif

uint32_t TaskTwoInit( void )
{
   if ( xTaskCreate( taskTwo,
         (const portCHAR *)"T2",
         TASKTWOSTACKSIZE,
         NULL,
         tskIDLE_PRIORITY + PRIORITY_TASK_TWO,
         NULL) != pdTRUE )
   {
      return 1;
   }

   return 0;

}


