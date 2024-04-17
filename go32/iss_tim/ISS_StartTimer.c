// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool ISS_StartTimer (void (*const proc) (void), uint32_t speed) {
  int i;

  for (i = 0; i < ISS_MaxTimers; i++)
    if (!ISS_TimersData[i].TActive) {
      if (!NewIRQActive) {
        __dpmi_paddr p;

        lock_SysTimerIRQ (true);
        __dpmi_get_protected_mode_interrupt_vector (PIT_IRQ, &OldTimer);
        p.offset32 = (uint32_t) SysTimerIRQ;
        p.selector = _go32_my_cs ();
        __dpmi_set_protected_mode_interrupt_vector (PIT_IRQ, &p);
      }

      ISS_TimersData[i].TSpeed = speed;
      ISS_TimersData[i].TCount = 0;
      ISS_TimersData[i].TProc = proc;
      ISS_TimersData[i].TActive = true;
      SetTimerSpeed (GetTimerSpeed ());
      return true;
    }

  ISS_TimerError = ISS_TENoFree;
  return false;
}
