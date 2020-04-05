/*
 * rtes_10_3.c
 *
 *  Created on: Mar 15, 2020
 *      Author: baquerrj
 */


#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"
#include "task5.h"
#include "task6.h"
#include "task7.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "sequencer.h"

static const uint32_t ITERATIONS = 1800;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void __error__( char *pcFilename, uint32_t ui32Line )
{
}

#endif

//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
void vApplicationStackOverflowHook( xTaskHandle *pxTask, char *pcTaskName )
{
   //
   // This function can not return, so loop forever.  Interrupts are disabled
   // on entry to this function, so no processor interrupts will interrupt
   // this loop.
   //
   while ( 1 )
   {
   }
}

//*****************************************************************************
// Interrupt Handler for Timer ISR
//*****************************************************************************
void Timer0InterruptHandler( void )
{
   static uint8_t state = 0;

   g_wakeTick = xTaskGetTickCountFromISR();
   count++;
   // Clear the interrupt
   ROM_TimerIntClear( TIMER0_BASE, TIMER_TIMA_TIMEOUT );
   BaseType_t xHigherPriorityTaskWoken;
   // Set xHigherPriorityTaskWoken to pdFALSE - will be set to pdTRUE if a
   // context switch is required
   xHigherPriorityTaskWoken = pdFALSE;
#ifdef ISR_TIMING
   state = (state == 0) ? 1 : 0;
   GPIOPinWrite( GPIO_PORTB_BASE, GPIO_PIN_0, state );
#else
   // Clear the interrupt
   ROM_TimerIntClear( TIMER0_BASE, TIMER_TIMA_TIMEOUT );

   // Set xHigherPriorityTaskWoken to pdFALSE - will be set to pdTRUE if a
   // context switch is required
   xHigherPriorityTaskWoken = pdFALSE;

   if ( ITERATIONS > count )
   {
      if ( count % task1Frequency == 0 )
      {
         xSemaphoreGiveFromISR( pSemaphoreS1, &xHigherPriorityTaskWoken );
      }
      if ( count % task2Frequency == 0 )
      {
         xSemaphoreGiveFromISR( pSemaphoreS2, &xHigherPriorityTaskWoken );
      }
      if ( count % task3Frequency == 0 )
      {
         xSemaphoreGiveFromISR( pSemaphoreS3, &xHigherPriorityTaskWoken );
      }
      if ( count % task4Frequency == 0 )
      {
         xSemaphoreGiveFromISR( pSemaphoreS4, &xHigherPriorityTaskWoken );
      }
      if ( count % task5Frequency == 0 )
      {
         xSemaphoreGiveFromISR( pSemaphoreS5, &xHigherPriorityTaskWoken );
      }
      if ( count % task6Frequency == 0 )
      {
         xSemaphoreGiveFromISR( pSemaphoreS6, &xHigherPriorityTaskWoken );
      }
      if ( count % task7Frequency == 0 )
      {
         xSemaphoreGiveFromISR( pSemaphoreS7, &xHigherPriorityTaskWoken );
      }
   }
   else
   {
      if ( !abortS1 )
      {
         abortS1 = true;
         xSemaphoreGiveFromISR( pSemaphoreS1, &xHigherPriorityTaskWoken );
      }
      if ( !abortS2 )
      {
         abortS2 = true;
         xSemaphoreGiveFromISR( pSemaphoreS2, &xHigherPriorityTaskWoken );
      }
      if ( !abortS3 )
      {
         abortS3 = true;
         xSemaphoreGiveFromISR( pSemaphoreS3, &xHigherPriorityTaskWoken );
      }
      if ( !abortS4 )
      {
         abortS4 = true;
         xSemaphoreGiveFromISR( pSemaphoreS4, &xHigherPriorityTaskWoken );
      }
      if ( !abortS5 )
      {
         abortS5 = true;
         xSemaphoreGiveFromISR( pSemaphoreS5, &xHigherPriorityTaskWoken );
      }
      if ( !abortS6 )
      {
         abortS6 = true;
         xSemaphoreGiveFromISR( pSemaphoreS6, &xHigherPriorityTaskWoken );
      }
      if ( !abortS7 )
      {
         abortS7 = true;
         xSemaphoreGiveFromISR( pSemaphoreS7, &xHigherPriorityTaskWoken );
      }
   }
#endif

//   portYIELD_FROM_ISR( &xHigherPriorityTaskWoken );
}

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void ConfigureUART( void )
{
   //
   // Enable the GPIO Peripheral used by the UART.
   //
   ROM_SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOA );

   //
   // Enable UART0
   //
   ROM_SysCtlPeripheralEnable( SYSCTL_PERIPH_UART0 );

   //
   // Configure GPIO Pins for UART mode.
   //
   ROM_GPIOPinConfigure( GPIO_PA0_U0RX );
   ROM_GPIOPinConfigure( GPIO_PA1_U0TX );
   ROM_GPIOPinTypeUART( GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1 );

   //
   // Use the internal 16MHz oscillator as the UART clock source.
   //
   UARTClockSourceSet( UART0_BASE, UART_CLOCK_PIOSC );

   //
   // Initialize the UART for console I/O.
   //
   UARTStdioConfig( 0, 115200, 16000000 );
}

void configureGPIOForTiming( void )
{
   ROM_SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOB );
   GPIOPinTypeGPIOOutput( GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 );
}

//*****************************************************************************
//
// Initialize FreeRTOS and start the initial set of tasks.
//
//*****************************************************************************
int main( void )
{
   //
   // Set the clocking to run at 80 MHz from the PLL.
   //
   SysCtlClockSet( SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN );

   //
   // Initialize the UART and configure it for 115,200, 8-N-1 operation.
   //
   ConfigureUART();
   configureGPIOForTiming();
   ROM_SysCtlPeripheralEnable( SYSCTL_PERIPH_TIMER0 );

   // Enable processor interrupts
   ROM_IntMasterEnable();

   // Timer Configuration
   ROM_TimerConfigure( TIMER0_BASE, TIMER_CFG_PERIODIC );
   uint32_t period = ROM_SysCtlClockGet() / 3000; // 60Hz interrupt
   ROM_TimerLoadSet( TIMER0_BASE, TIMER_A, period );

   // Enable interrupts from TIMER0
   ROM_IntEnable( INT_TIMER0A );
   ROM_IntPrioritySet( INT_TIMER0A, 7 << 5 );   // set to a lower priority than configMAX_SYSCALL_INTERRUPT_PRIORITY

   // Enable timeout timer interrupt for TIMER0
   ROM_TimerIntEnable( TIMER0_BASE, TIMER_TIMA_TIMEOUT );

   // Timer Enable
   ROM_TimerEnable( TIMER0_BASE, TIMER_A );
   //
   // Print demo introduction.
   //
   UARTprintf( "\nECEN5623: Exercise-5 Problem 3\n" );
   UARTprintf( "FreeRTOS revision of seqgen2 running on TM4C123GXL\n" );


   g_lastWakeTick = 0;
   g_wakeTick = 0;
   count = 0;
   // Create a mutex to guard the UART.
   g_pUARTSemaphore = xSemaphoreCreateMutex();

#ifdef ISR_TIMING
   pSemaphoreS1 = xSemaphoreCreateBinary();
   abortS1 = false;
   if ( NULL == pSemaphoreS1 )
   {
      UARTprintf( "error sempahore init!\n" );
      while(1);
   }
   if ( TaskOneInit() != 0 )
   {
      while ( 1 )
      {
      }
   }
#else
   // Create a binary semaphore for task signaling
   pSemaphoreS1 = xSemaphoreCreateBinary();
   pSemaphoreS2 = xSemaphoreCreateBinary();
   pSemaphoreS3 = xSemaphoreCreateBinary();
   pSemaphoreS4 = xSemaphoreCreateBinary();
   pSemaphoreS5 = xSemaphoreCreateBinary();
   pSemaphoreS6 = xSemaphoreCreateBinary();
   pSemaphoreS7 = xSemaphoreCreateBinary();

   abortS1 = false;
   abortS2 = false;
   abortS3 = false;
   abortS4 = false;
   abortS5 = false;
   abortS6 = false;
   abortS7 = false;

   if ( TaskOneInit() != 0 )
   {
      while ( 1 )
      {
      }
   }

   if ( TaskTwoInit() != 0 )
   {
      while ( 1 )
      {
      }
   }
   if ( TaskThreeInit() != 0 )
   {
      while ( 1 )
      {
      }
   }
   if ( TaskFourInit() != 0 )
   {
      while ( 1 )
      {
      }
   }
   if ( TaskFiveInit() != 0 )
   {
      while ( 1 )
      {
      }
   }
   if ( TaskSixInit() != 0 )
   {
      while ( 1 )
      {
      }
   }
   if ( TaskSevenInit() != 0 )
   {
      while ( 1 )
      {
      }
   }

#endif
   //
   // Start the scheduler.  This should not return.
   //
   vTaskStartScheduler();

   //
   // In case the scheduler returns for some reason, print an error and loop
   // forever.
   //

   while ( 1 )
   {
   }
}
