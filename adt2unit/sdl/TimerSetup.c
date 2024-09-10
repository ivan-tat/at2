// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static uint32_t TimerCallback (__UNUSED uint32_t interval,
                               __UNUSED void *param) {
  if (timer_handler)
    timer_handler ();

  return _interval; // Trick to alter delay rate on the fly
}

void TimerSetup (uint32_t Hz) {
  _interval = 1000 / Hz;

  // Only activate timer once, later only alter delay rate
  if (!TimerID) {
    TimerID = SDL_AddTimer (_interval, (SDL_NewTimerCallback) &TimerCallback, NULL);
    if (!TimerID)
      printf ("SDL: Error creating timer" NL);
  }

  snd_SetTimer (Hz);
}
