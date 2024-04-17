// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int ISS_GetTimerNumber (void (*const proc) (void)) {
  int i;

  for (i = 0; i < ISS_MaxTimers; i++)
    if (ISS_TimersData[i].TActive && (ISS_TimersData[i].TProc == proc))
      return i;

  return -1; // not found
}
