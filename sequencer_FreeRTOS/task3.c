/*
 * task3.c
 *
 *  Created on: Apr 3, 2020
 *      Author: baquerrj
 */

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
#include "task3.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "sequencer.h"

#define TASKTHREESTACKSIZE        128         // Stack size in words

extern xSemaphoreHandle g_pUARTSemaphore;
extern xSemaphoreHandle pSemaphoreS3;

extern bool abortS3;

static TaskHandle_t taskHandle;

static void taskThree( void *pvParameters )
{
   portTickType wakeTick = 0;
   portTickType doneTick = 0;
   portTickType wcet = 0;
   uint32_t releases = 0;
   const char * taskName = ( const char* ) pcTaskGetName( taskHandle );

   TASKLOGTIME( taskName, releases, xTaskGetTickCount() );
   while ( !abortS3 )
   {
      if ( pdPASS == xSemaphoreTake( pSemaphoreS3, portMAX_DELAY ) )
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

uint32_t TaskThreeInit( void )
{
   taskHandle = NULL;
   if ( xTaskCreate( taskThree,
         (const portCHAR *)"S3",
         TASKTHREESTACKSIZE,
         NULL,
         PRIORITY_TASK_THREE,
         &taskHandle) != pdTRUE )
   {
      return 1;
   }

   return 0;

}

