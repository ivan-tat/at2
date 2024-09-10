// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void done_timer_proc (void) {
  DBG_ENTER ("done_timer_proc");

  if (timer_initialized) {
    timer_initialized = false;
    TimerDone ();
    TimerRemoveHandler ();
  }
}
