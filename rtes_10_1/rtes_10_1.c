/*
 * rtes_10_1.c
 *
 *  Created on: Mar 15, 2020
 *      Author: baquerrj
 */

#include <stdbool.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "processingTask.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "utils/uartstdio.h"
#include "rtes_10_1.h"

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

//*****************************************************************************
// Interrupt Handler for Timer ISR
//*****************************************************************************
void Timer0InterruptHandler( void )
{
   BaseType_t xHigherPriorityTaskWoken;
   // Get the current system tick
   g_wakeTick = xTaskGetTickCountFromISR();

   // Clear the interrupt
   ROM_TimerIntClear( TIMER0_BASE, TIMER_TIMA_TIMEOUT );

   // Set xHigherPriorityTaskWoken to pdFALSE - will be set to pdTRUE if a
   // context switch is required
   xHigherPriorityTaskWoken = pdFALSE;

   // Give the semaphore
   xSemaphoreGiveFromISR( g_pTaskSemaphore, &xHigherPriorityTaskWoken );
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
   ROM_SysCtlClockSet( SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
   SYSCTL_OSC_MAIN );

   //
   // Initialize the UART and configure it for 115,200, 8-N-1 operation.
   //
   ConfigureUART();

   ROM_SysCtlPeripheralEnable( SYSCTL_PERIPH_TIMER0 );

   // Enable processor interrupts
   ROM_IntMasterEnable();

   // Timer Configuration
   ROM_TimerConfigure( TIMER0_BASE, TIMER_CFG_PERIODIC );

   uint32_t period = ROM_SysCtlClockGet() / 10;
   ROM_TimerLoadSet( TIMER0_BASE, TIMER_A, period - 1 );

   // Enable interrupts from TIMER0
   ROM_IntEnable( INT_TIMER0A );

   // Enable timeout timer interrupt for TIMER0
   ROM_TimerIntEnable( TIMER0_BASE, TIMER_TIMA_TIMEOUT );

   // Timer Enable
   ROM_TimerEnable( TIMER0_BASE, TIMER_A );

   UARTprintf( "\n\nRTES Problem 10.1 on EK-TM4C123GXL\n" );

   // Create a mutex to guard the UART.
   g_pUARTSemaphore = xSemaphoreCreateMutex();

   // Create a binary semaphore for task signaling
   g_pTaskSemaphore = xSemaphoreCreateBinary();

   if ( ProcessingTaskInit() != 0 )
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
