// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileNotice: Based on ISS_TIM.PAS version 1.1.1 from Inquisition Sound Server for Free Pascal Compiler.
// SPDX-FileNotice: Original code is (C) 1998-2001 Charlie/Inquisition <charlie@scenergy.dfmk.hu>.
// SPDX-FileNotice: Original developers: <http://scenergy.dfmk.hu/inqcoders/>
// SPDX-FileNotice: Inquisition website: <http://scenergy.dfmk.hu/inquisition/>

#include "defines.h"
#if USE_FPC
#include "pascal.h"
#include "pascal/dos.h"
#include "pascal/pc.h"
#include "pascal/string.h"
#else // !USE_FPC
#include <dos.h>
#include <pc.h>
#include <string.h>
#endif // !USE_FPC
#include "go32/adt2dpmi.h"
#include "go32/PIT.h"
#include "go32/iss_tim.h"

uint32_t    ISS_TimerSpeed = PIT_FREQ_MAX;
TTimerStruc ISS_TimersData[ISS_MaxTimers]; // HINT: (FPC) start index 1
uint32_t    ISS_TimerError;

static uint32_t     TimerSpeed;
static __dpmi_paddr OldTimer;
static uint32_t     OldTimerCnt;
static bool         NewIRQActive;

#include "go32/iss_tim/ISS_DisableTimerIRQ.c"
#include "go32/iss_tim/ISS_EnableTimerIRQ.c"
#include "go32/iss_tim/UpdateUserTimers.c"
#include "go32/iss_tim/SysTimerIRQ.c"
#include "go32/iss_tim/SetTimerSpeed.c"
#include "go32/iss_tim/GetTimerSpeed.c"
#include "go32/iss_tim/ISS_StartTimer.c"
#include "go32/iss_tim/ISS_StopTimer.c"
#include "go32/iss_tim/ISS_GetTimerNumber.c"
#include "go32/iss_tim/init_ISS_Timer.c"
