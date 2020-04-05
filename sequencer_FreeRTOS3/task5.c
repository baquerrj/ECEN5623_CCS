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
//   struct timeval current_time_val;
//   double current_time;
//   unsigned long long S5Cnt=0;
//   threadParams_t *threadParams = (threadParams_t *)threadp;
//
//   gettimeofday(&current_time_val, (struct timezone *)0);
//   syslog(LOG_CRIT, "Frame Sampler thread @ sec=%d, msec=%d\n", (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);
//   printf("Frame Sampler thread @ sec=%d, msec=%d\n", (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);
//
//   while(!abortS5)
//   {
//       sem_wait(&semS5);
//       S5Cnt++;
//
//       gettimeofday(&current_time_val, (struct timezone *)0);
//       syslog(LOG_CRIT, "Frame Sampler release %llu @ sec=%d, msec=%d\n", S5Cnt, (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);
//   }
//
   portTickType wakeTick = 0;
   portTickType doneTick = 0;
   portTickType wcet = 0;
   uint32_t releases = 0;
   const char * taskName = ( const char* ) pcTaskGetName( taskHandle );

   TASKLOGTIME( taskName, releases, xTaskGetTickCount() );
   while ( !abortS5 )
   {
      if ( pdPASS == xSemaphoreTake( pSemaphoreS5, portMAX_DELAY ) )
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

uint32_t TaskFiveInit( void )
{
   taskHandle = NULL;
   if ( xTaskCreate( taskFive,
         (const portCHAR *)"S5",
         TASKFIVESTACKSIZE,
         NULL,
         PRIORITY_TASK_FIVE,
         &taskHandle ) != pdTRUE )
   {
      return 1;
   }

   return 0;

}
