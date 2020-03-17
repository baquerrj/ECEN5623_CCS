//*****************************************************************************
//
// freertos_demo.c - Simple FreeRTOS example.
//
// Copyright (c) 2012-2017 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 2.1.4.178 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

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
#include "taskOne.h"
#include "taskTwo.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "rtes_10_3.h"

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

//#define ISR_TIMING
//*****************************************************************************
// Interrupt Handler for Timer ISR
//*****************************************************************************
void Timer0InterruptHandler( void )
{
   static uint32_t count = 0;

   BaseType_t xHigherPriorityTaskWoken;
#ifdef ISR_TIMING
   // Get the current system tick
   g_wakeTick = xTaskGetTickCountFromISR();
   portTickType elapsed = 0;

   elapsed = g_wakeTick - g_lastWakeTick;
   UARTprintf( "\nISR: time elapsed %d ticks, %d ms\n", elapsed, elapsed / portTICK_PERIOD_MS );
   g_lastWakeTick = g_wakeTick;

   // Clear the interrupt
   ROM_TimerIntClear( TIMER0_BASE, TIMER_TIMA_TIMEOUT );
#else
   // Clear the interrupt
   ROM_TimerIntClear( TIMER0_BASE, TIMER_TIMA_TIMEOUT );

   // Set xHigherPriorityTaskWoken to pdFALSE - will be set to pdTRUE if a
   // context switch is required
   xHigherPriorityTaskWoken = pdFALSE;

   if ( count % 3 == 0 )
   {
      xSemaphoreGiveFromISR( g_p30MsTaskSemaphore, &xHigherPriorityTaskWoken );
   }
   else if ( count % 8 == 0 )
   {
      xSemaphoreGiveFromISR( g_p80MsTaskSemaphore, &xHigherPriorityTaskWoken );
   }

   portYIELD_FROM_ISR( &xHigherPriorityTaskWoken );
#endif
   count++;
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

void calculateFibonacciNumbers( uint32_t terms, uint32_t iterations )
{
   uint32_t n1 = 0;
   uint32_t n2 = 1;
   uint32_t i = 0;
   uint32_t j = 0;
   uint32_t nextTerm = 0;

   for ( i = 0; i < iterations; ++i )
   {
      nextTerm = n1 + n2;
      while ( j < terms )
      {
         n1 = n2;
         n2 = nextTerm;
         nextTerm = n1 + n2;
         j++;
      }
   }
}

//*****************************************************************************
//
// Initialize FreeRTOS and start the initial set of tasks.
//
//*****************************************************************************
int main( void )
{
   //
   // Set the clocking to run at 50 MHz from the PLL.
   //
   SysCtlClockSet( SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN );

   //
   // Initialize the UART and configure it for 115,200, 8-N-1 operation.
   //
   ConfigureUART();

   ROM_SysCtlPeripheralEnable( SYSCTL_PERIPH_TIMER0 );

   // Enable processor interrupts
   ROM_IntMasterEnable();

   // Timer Configuration
   ROM_TimerConfigure( TIMER0_BASE, TIMER_CFG_PERIODIC );
   uint32_t period = ROM_SysCtlClockGet() / 100;
   ROM_TimerLoadSet( TIMER0_BASE, TIMER_A, period - 1 );

   UARTprintf( "ROM_SysCtlClockGet() = %d\n", ROM_SysCtlClockGet() );

   // Enable interrupts from TIMER0
   ROM_IntEnable( INT_TIMER0A );

   // Enable timeout timer interrupt for TIMER0
   ROM_TimerIntEnable( TIMER0_BASE, TIMER_TIMA_TIMEOUT ); //Timer 0 in Periodic mode

   // Timer Enable
   ROM_TimerEnable( TIMER0_BASE, TIMER_A );
   //
   // Print demo introduction.
   //
   UARTprintf( "\n\nRTES Problem 10.3 on EK-TM4C123GXL\n" );

   g_lastWakeTick = 0;
   g_wakeTick = 0;

   _10MsIterations = 49600;
   _40MsIterations = 199000;
   //
   // Create a mutex to guard the UART.
   //
   g_pUARTSemaphore = xSemaphoreCreateMutex();

   // Create a binary semaphore for task signaling
   g_p30MsTaskSemaphore = xSemaphoreCreateBinary();
   g_p80MsTaskSemaphore = xSemaphoreCreateBinary();

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
