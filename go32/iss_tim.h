// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileNotice: Based on ISS_TIM.PAS version 1.1.1 from Inquisition Sound Server for Free Pascal Compiler.
// SPDX-FileNotice: Original code is (C) 1998-2001 Charlie/Inquisition <charlie@scenergy.dfmk.hu>.
// SPDX-FileNotice: Original developers: <http://scenergy.dfmk.hu/inqcoders/>
// SPDX-FileNotice: Inquisition website: <http://scenergy.dfmk.hu/inquisition/>

#pragma once

#ifndef GO32_ISS_TIM_H
#define GO32_ISS_TIM_H

#include <stdbool.h>
#include <stdint.h>

#define ISS_MaxTimers 8 // Maximum number of timers

#define ISS_TENoFree   0x01 // Can't add new timer. All timers locked.
#define ISS_TENotFound 0x02 // Can't find specified timer to stop.

typedef struct {
  uint32_t TSpeed;
  uint32_t TCount;        // Tick counter
  uint32_t TPrevCount;    // Tick counter state at prev. activity
  void   (*TProc) (void); // Procedure to call
  bool     TActive;       // `true' if the timer is on
} TTimerStruc;

extern uint32_t    ISS_TimerSpeed;
extern TTimerStruc ISS_TimersData[ISS_MaxTimers]; // HINT: (FPC) start index 0
extern uint32_t    ISS_TimerError; // Last timer error code

void ISS_DisableTimerIRQ (void);
void ISS_EnableTimerIRQ (void);
bool ISS_StartTimer (void (*const proc) (void), uint32_t speed);
bool ISS_StopTimer (void (*const proc) (void));
int ISS_GetTimerNumber (void (*const proc) (void));

void init_ISS_Timer (void);

#endif // !defined(GO32_ISS_TIM_H)
