// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool ISS_StopTimer (void (*const proc) (void)) {
  bool ok = false;
  int count = 0, i;

  disable ();

  for (i = 0; i < ISS_MaxTimers; i++)
    if (ISS_TimersData[i].TActive) {
      count++; // Found active timer
      if ((ISS_TimersData[i].TProc == proc) && !ok) {
        ISS_TimersData[i].TActive = false;
        ok = true; // Found first occurrence of target timer
      }
    }

  if (ok) {
    if (count == 1) {
      // This is the last active timer: shutdown ISS
      TimerSpeed = 0;
      SetTimerSpeed (PIT_LATENCY_MAX);
      __dpmi_set_protected_mode_interrupt_vector (PIT_IRQ, &OldTimer);
      lock_SysTimerIRQ (false);
    }
  } else
    ISS_TimerError = ISS_TENotFound;

  enable ();

  return ok;
}
