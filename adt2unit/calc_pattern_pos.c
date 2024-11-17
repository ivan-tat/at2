// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint8_t calc_pattern_pos (uint8_t pattern)
{
  uint8_t r = BYTE_NULL; // result (pattern position)
  uint8_t n = 0; // jumps count
  int16_t i; // index

  DBG_ENTER ("calc_pattern_pos");

  i = calc_following_order (0);
  while ((i != -1) && (n + 1 < 0x80))
  {
    if (songdata.pattern_order[i] != pattern)
    {
      if (i + 1 < 0x80)
      {
        i = calc_following_order (i + 1);
        n++;
      } else
        break;
    }
    else
    {
      r = i;
      break;
    }
  }

  DBG_LEAVE (); //EXIT //calc_pattern_pos
  return r;
}
