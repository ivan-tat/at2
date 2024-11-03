// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void TimerSetup (uint32_t Hz) {
  DBG_ENTER ("TimerSetup");

  if (Hz < PIT_FREQ_MIN)
    Hz = PIT_FREQ_MIN;
  if (Hz > PIT_FREQ_MAX)
    Hz = PIT_FREQ_MAX;

  ISS_DisableTimerIRQ ();

  if (timer_poll_proc_ptr)
    ISS_StopTimer (timer_poll_proc_ptr);

  timer_poll_proc_ptr = &timer_poll_proc;
  ISS_StartTimer (timer_poll_proc_ptr, ISS_TimerSpeed / Hz);

  ISS_EnableTimerIRQ ();

  DBG_LEAVE (); //TimerSetup
}
