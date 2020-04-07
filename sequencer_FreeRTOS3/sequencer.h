/*
 * rtes_10_1.h
 *
 *  Created on: Mar 15, 2020
 *      Author: baquerrj
 */

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "semphr.h"

static const uint32_t ITERATIONS = 9000;

#define FREQ_MULTIPLIER          (100)

// Sequencer - 3000 Hz
//                   [gives semaphores to all other services]
#define SEQ_FREQUENCY      (30*FREQ_MULTIPLIER)

// Service_1 - 300 Hz  , every 10th Sequencer loop
//                   [buffers 3 images per second]
#define task1Frequency     (10*FREQ_MULTIPLIER)// 300Hz

// Service_2 - 100 Hz  , every 30th Sequencer loop
//                   [time-stamp middle sample image with cvPutText or header]
#define task2Frequency     (30*FREQ_MULTIPLIER)  // 100Hz

// Service_3 - 50 Hz, every 60th Sequencer loop
//                   [difference current and previous time stamped images]
#define task3Frequency     (60*FREQ_MULTIPLIER)  // 5Hz

// Service_4 - 100 Hz, every 30th Sequencer loop
//                   [save time stamped image with cvSaveImage or write()]
#define task4Frequency     (30*FREQ_MULTIPLIER)  // 100Hz

// Service_5 - 50 Hz, every 60th Sequencer loop
//                   [save difference image with cvSaveImage or write()]
#define task5Frequency     (60*FREQ_MULTIPLIER)  // 50Hz

// Service_6 - 100 Hz, every 30th Sequencer loop
//                   [write current time-stamped image to TCP socket server]
#define task6Frequency     (30*FREQ_MULTIPLIER)  // 100Hz

// Service_7 - 10 Hz, every 300th Sequencer loop
//                   [syslog the time for debug]
#define task7Frequency     (300*FREQ_MULTIPLIER) // 10Hz

// With the above, priorities by RM policy would be:
//
// Sequencer = RT_MAX   @ 300 Hz
// Servcie_1 = RT_MAX-1 @ 30 Hz
// Service_2 = RT_MAX-2 @ 10 Hz
// Service_3 = RT_MAX-3 @ 50 Hz
// Service_4 = RT_MAX-2 @ 10 Hz
// Service_5 = RT_MAX-3 @ 50 Hz
// Service_6 = RT_MAX-2 @ 100 Hz
// Service_7 = RT_MIN   10 Hz

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

//#define ISR_TIMING
//#define TASK_LOG_ON
#define TASK_END_LOG

//! Ticks for ISR timing
portTickType g_wakeTick;
portTickType g_lastWakeTick;

uint32_t count;

//! printf macro for writing to UART just one line
#define UARTPRINTF( fmt, ... ) xSemaphoreTake( g_pUARTSemaphore, portMAX_DELAY );\
    UARTprintf( fmt, ##__VA_ARGS__ ); \
    xSemaphoreGive( g_pUARTSemaphore )

//! Macro to simply log task information
#ifdef TASK_LOG_ON
#define TASKLOGTIME( _task, _release,  _tick ) xSemaphoreTake( g_pUARTSemaphore, portMAX_DELAY );\
   UARTprintf( "\n%s release %d at %u ticks\n", _task, _release, _tick );\
   xSemaphoreGive( g_pUARTSemaphore )
#else
#define TASKLOGTIME( _task, _release, _tick )   // noop
#endif

#ifdef TASK_END_LOG
#define TASKLOGEND( _task, _release, _tick ) xSemaphoreTake( g_pUARTSemaphore, portMAX_DELAY );\
   UARTprintf( "\n*** %s: %d times WCET = %d tick ***\n", _task, _release, _tick );\
     xSemaphoreGive( g_pUARTSemaphore )
#else
#define TASKLOGEND( _task, _release, _tick )    // no-op
#endif

inline uint32_t getTimeFromTimer( void )
{
   return TimerValueGet( TIMER1_BASE, TIMER_A );
}

inline uint32_t getTimeDifference( uint32_t start, uint32_t stop )
{
   uint32_t dt = ( start > stop ) ? ( start - stop ) : ( stop - start );
   return dt;
}

#endif /* SEQUENCER_H_ */
