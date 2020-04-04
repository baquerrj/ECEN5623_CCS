/*
 * rtes_10_1.h
 *
 *  Created on: Mar 15, 2020
 *      Author: baquerrj
 */

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "semphr.h"

// Sequencer - 60 Hz
//                   [gives semaphores to all other services]
// Service_1 - 30 Hz, every other Sequencer loop
//                   [buffers 3 images per second]
static const unsigned int task1Frequency = 1;   // 30Hz

// Service_2 - 10 Hz, every 6th Sequencer loop
//                   [time-stamp middle sample image with cvPutText or header]
static const unsigned int task2Frequency = 6;  // 10Hz

// Service_3 - 5 Hz , every 12th Sequencer loop
//                   [difference current and previous time stamped images]
static const unsigned int task3Frequency = 12;  // 5Hz

// Service_4 - 10 Hz, every 6th Sequencer loop
//                   [save time stamped image with cvSaveImage or write()]
static const unsigned int task4Frequency = 6;  // 10Hz

// Service_5 - 5 Hz , every 12th Sequencer loop
//                   [save difference image with cvSaveImage or write()]
static const unsigned int task5Frequency = 12;  // 5Hz

// Service_6 - 10 Hz, every 6th Sequencer loop
//                   [write current time-stamped image to TCP socket server]
static const unsigned int task6Frequency = 6;  // 10Hz

// Service_7 - 1 Hz , every 60th Sequencer loop
//                   [syslog the time for debug]
//
static const unsigned int task7Frequency = 60; // 1Hz

// With the above, priorities by RM policy would be:
//
// Sequencer = RT_MAX   @ 60 Hz
// Servcie_1 = RT_MAX-1 @ 30 Hz
// Service_2 = RT_MAX-2 @ 10 Hz
// Service_3 = RT_MAX-3 @ 5  Hz
// Service_4 = RT_MAX-2 @ 10 Hz
// Service_5 = RT_MAX-3 @ 5  Hz
// Service_6 = RT_MAX-2 @ 10 Hz
// Service_7 = RT_MIN   @ 1  Hz

//! Mutex for UART access
xSemaphoreHandle g_pUARTSemaphore;

//! Binary Semaphores used for signaling tasks from Timer ISR
xSemaphoreHandle pSemaphoreS1;
xSemaphoreHandle pSemaphoreS2;
xSemaphoreHandle pSemaphoreS3;
xSemaphoreHandle pSemaphoreS4;
xSemaphoreHandle pSemaphoreS5;
xSemaphoreHandle pSemaphoreS6;
xSemaphoreHandle pSemaphoreS7;

bool abortS1;
bool abortS2;
bool abortS3;
bool abortS4;
bool abortS5;
bool abortS6;
bool abortS7;

//! Ticks for ISR timing
portTickType g_wakeTick;
portTickType g_lastWakeTick;

//! printf macro for writing to UART just one line
#define UARTPRINTF( fmt, ... ) xSemaphoreTake( g_pUARTSemaphore, portMAX_DELAY );\
    UARTprintf( fmt, ##__VA_ARGS__ ); \
    xSemaphoreGive( g_pUARTSemaphore )

//! Macro to simply logging task information
#define TASKLOGTIME( _task, _release,  _tick ) xSemaphoreTake( g_pUARTSemaphore, portMAX_DELAY );\
   UARTprintf( "\n%s release %d at %d ms\n", _task, _release, _tick / portTICK_PERIOD_MS);\
   xSemaphoreGive( g_pUARTSemaphore )

#endif /* SEQUENCER_H_ */
