// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void init_ISS_Timer (void) {
  memset (ISS_TimersData, 0, sizeof (ISS_TimersData));
  NewIRQActive = false;
  TimerSpeed = 0;
}
