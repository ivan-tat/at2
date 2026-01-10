// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint8_t marked_instruments (void)
{
  uint8_t result = 0;

  for (uint_least8_t i = 0; i < 255; i++)
    if (GetStr (songdata.instr_names[i])[0] == _INS_MARK_CHR)
      result++;

  return result;
}
