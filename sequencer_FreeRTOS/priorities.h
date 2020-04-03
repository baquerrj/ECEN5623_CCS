//*****************************************************************************
//
// priorities.h - Priorities for the various FreeRTOS tasks.
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

#ifndef __PRIORITIES_H__
#define __PRIORITIES_H__

#include "FreeRTOSConfig.h"

//*****************************************************************************
//
// The priorities of the various tasks.
//
//*****************************************************************************
#define THREEHZ_PRIORITY      (configMAX_PRIORITIES-1)
#define ONEHZ_PRIORITY        (configMAX_PRIORITIES-2)
#define ONEHALFHZ_PRIORITY    (configMAX_PRIORITIES-3)
#define ONETENHZ_PRIORITY     (configMAX_PRIORITIES-4)

#define PRIORITY_TASK_ONE     (THREEHZ_PRIORITY)
#define PRIORITY_TASK_TWO     (ONEHZ_PRIORITY)
#define PRIORITY_TASK_THREE   (ONEHALFHZ_PRIORITY)
#define PRIORITY_TASK_FOUR    (ONEHZ_PRIORITY)
#define PRIORITY_TASK_FIVE    (ONEHALFHZ_PRIORITY)
#define PRIORITY_TASK_SIX     (ONEHZ_PRIORITY)
#define PRIORITY_TASK_SEVEN   (ONETENHZ_PRIORITY)


#endif // __PRIORITIES_H__
