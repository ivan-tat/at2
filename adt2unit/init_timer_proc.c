// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void init_timer_proc (void) {
  DBG_ENTER ("init_timer_proc");

#if !ADT2PLAY
#if GO32
  scroll_ticks = 0;
#endif // GO32

  srand (time (NULL));
#endif // !ADT2PLAY

  if (!timer_initialized) {
    timer_initialized = true;
#if !ADT2PLAY
#if !GO32
    TimerInstallHandler (timer_poll_proc);
#endif // !GO32
#endif // !ADT2PLAY
    TimerSetup (50);
  }

  DBG_LEAVE (); //init_timer_proc
}
