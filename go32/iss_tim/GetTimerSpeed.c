// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static uint32_t GetTimerSpeed (void) {
  TTimerStruc *t = ISS_TimersData;
  uint32_t fastest = PIT_LATENCY_MAX;
  int c;

  for (c = ISS_MaxTimers; c; c--) {
    if (t->TActive && (t->TSpeed < fastest))
      fastest = t->TSpeed;

    t++;
  }

  return fastest;
}
