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
//
//
// Sequencer = RT_MAX   @ 3000 Hz
// Servcie_1 = RT_MAX-1 @ 300 Hz
// Service_2 = RT_MAX-2 @ 100 Hz
// Service_3 = RT_MAX-3 @ 50 Hz
// Service_4 = RT_MAX-2 @ 100 Hz
// Service_5 = RT_MAX-3 @ 50 Hz
// Service_6 = RT_MAX-2 @ 100 Hz
// Service_7 = RT_MIN   10 Hz

#define PRIORITY_TASK_ONE     (configMAX_PRIORITIES-1)
#define PRIORITY_TASK_TWO     (configMAX_PRIORITIES-2)
#define PRIORITY_TASK_THREE   (configMAX_PRIORITIES-3)
#define PRIORITY_TASK_FOUR    (configMAX_PRIORITIES-2)
#define PRIORITY_TASK_FIVE    (configMAX_PRIORITIES-3)
#define PRIORITY_TASK_SIX     (configMAX_PRIORITIES-2)
#define PRIORITY_TASK_SEVEN   (configMAX_PRIORITIES-4)

#endif // __PRIORITIES_H__
