// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __LOCK_FUNC void UpdateUserTimers (void) {
  TTimerStruc *t = ISS_TimersData;
  int c;

  for (c = ISS_MaxTimers; c; c--) {
    if (t->TActive) {
      t->TCount += TimerSpeed;
      if (t->TCount > t->TSpeed) {
        t->TCount -= t->TSpeed;
        t->TPrevCount = t->TCount;
        t->TProc ();
      }
    }

    t++;
  }
}

END_OF_STATIC_FUNCTION (UpdateUserTimers)

static void lock_UpdateUserTimers (bool lock) {
  LOCK_DECL (lock, v, f);

  LOCK_VARIABLE (v, ISS_TimersData);
  LOCK_VARIABLE (v, TimerSpeed);
  LOCK_FUNCTION (f, UpdateUserTimers);
}
