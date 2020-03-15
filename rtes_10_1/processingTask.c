/*
 * processingTask.c
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
#include "processingTask.h"
#include "led_task.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "rtes_10_1.h"

#define PROCESSINGTASKSTACKSIZE        128         // Stack size in words

extern xSemaphoreHandle g_pUARTSemaphore;
extern xSemaphoreHandle g_pTaskSemaphore;

static void processingTask( void *pvParameters )
{
    uint32_t timer1val = 0;
    uint32_t count = 0;
    int32_t  difference = 0;
    while ( 1 )
    {
        UARTPRINTF( "Waiting for task semaphore\n" );

        timer1val = TimerValueGet( TIMER1_BASE, TIMER_A );
        difference = count - timer1val;
        if( difference < 0 )
        {
            difference = -1 * (difference - 100000000);
        }
        UARTPRINTF( "Got Task Semaphore after %d Clock Cycles\n", difference );
        count = timer1val;
    }
    vTaskDelete( NULL );
}

uint32_t ProcessingTaskInit( void )
{
    if( xTaskCreate( processingTask,
                     (const portCHAR *)"ProcTask",
                     PROCESSINGTASKSTACKSIZE,
                     NULL,
                     tskIDLE_PRIORITY + PRIORITY_PROCESSING_TASK,
                     NULL) != pdTRUE )
    {
        return 1;
    }

    return 0;

}
