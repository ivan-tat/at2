// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint8_t _patts_marked (void)
{
  uint8_t result = 0;

  for (uint8_t i = 0; i < 0x80; i++)
    if (songdata.pattern_names[i][1] == charmap.black_right_triangle)
      result++;

  return result;
}
