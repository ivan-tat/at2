// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void SetTimerSpeed (uint32_t speed) {
  if (TimerSpeed != speed) {
    PIT_set_latency (0, speed);
    TimerSpeed = speed;
  }
}
