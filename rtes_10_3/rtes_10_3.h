/*
 * rtes_10_1.h
 *
 *  Created on: Mar 15, 2020
 *      Author: baquerrj
 */

#ifndef RTES_10_3_H_
#define RTES_10_3_H_

#include "semphr.h"


//*****************************************************************************
//
// The mutex that protects concurrent access of UART from multiple tasks.
//
//*****************************************************************************
xSemaphoreHandle g_pUARTSemaphore;

// Semaphore used to signal processing task from ISR
xSemaphoreHandle g_p30MsTaskSemaphore;
xSemaphoreHandle g_p80MsTaskSemaphore;


portTickType g_wakeTick;
portTickType g_lastWakeTick;

#define UARTPRINTF( fmt, ... ) xSemaphoreTake( g_pUARTSemaphore, portMAX_DELAY );\
    UARTprintf( fmt, ##__VA_ARGS__ ); \
    xSemaphoreGive( g_pUARTSemaphore )


#endif /* RTES_10_3_H_ */
