// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns index of instrument + 1 (1..255) if found, 0 otherwise.
uint8_t _1st_marked (void)
{
  for (uint_least8_t i = 0; i <= 254; i++)
    if (GetStr (songdata.instr_names[i])[0] == _INS_MARK_CHR)
      return i + 1;

  return 0;
}
