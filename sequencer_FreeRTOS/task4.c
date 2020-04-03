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
//   struct timeval current_time_val;
//   double current_time;
//   unsigned long long S1Cnt=0;
//   threadParams_t *threadParams = (threadParams_t *)threadp;
//
//   gettimeofday(&current_time_val, (struct timezone *)0);
//   syslog(LOG_CRIT, "Frame Sampler thread @ sec=%d, msec=%d\n", (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);
//   printf("Frame Sampler thread @ sec=%d, msec=%d\n", (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);
//
//   while(!abortS1)
//   {
//       sem_wait(&semS1);
//       S1Cnt++;
//
//       gettimeofday(&current_time_val, (struct timezone *)0);
//       syslog(LOG_CRIT, "Frame Sampler release %llu @ sec=%d, msec=%d\n", S1Cnt, (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);
//   }
//
   portTickType wakeTick = 0;
   portTickType doneTick = 0;
   portTickType wcet = 0;
   uint32_t releases = 0;
   const char * taskName = ( const char* ) pcTaskGetTaskName( taskHandle );

   TASKLOGTIME( taskName, releases, xTaskGetTickCount() );
   while ( !abortS4 )
   {
      if ( pdPASS == xSemaphoreTake( pSemaphoreS4, portMAX_DELAY ) )
      {
         wakeTick = xTaskGetTickCount();
         releases++;
         TASKLOGTIME( taskName, releases, wakeTick );
         doneTick = xTaskGetTickCount();
         if ( ( doneTick - wakeTick ) > wcet )
         {
            wcet = doneTick - wakeTick;
         }
      }
   }
   UARTPRINTF( "\n*** %s: WCET = %d ms ***\n", taskName, wcet / portTICK_PERIOD_MS );
   vTaskDelete( NULL );
}

uint32_t TaskFourInit( void )
{
   taskHandle = NULL;
   if ( xTaskCreate( taskFour,
         (const portCHAR *)"S4",
         TASKFOURSTACKSIZE,
         NULL,
         PRIORITY_TASK_FOUR,
         &taskHandle ) != pdTRUE )
   {
      return 1;
   }

   return 0;

}
