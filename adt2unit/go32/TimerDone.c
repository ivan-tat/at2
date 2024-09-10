// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ void TimerDone (void) {
  DBG_ENTER ("TimerDone");

  ISS_DisableTimerIRQ ();

  if (timer_poll_proc_ptr) {
    ISS_StopTimer (timer_poll_proc_ptr);
    timer_poll_proc_ptr = NULL;
  }

  ISS_EnableTimerIRQ ();
}
