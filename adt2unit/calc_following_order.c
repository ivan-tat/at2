// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int16_t calc_following_order (uint8_t order)
{
  int16_t r = -1; // result
  uint8_t i = order; // index
  uint8_t n = 0; // jumps count

  DBG_ENTER ("calc_following_order");

  do
  {
    uint8_t j = songdata.pattern_order[i];

    if (j < 0x80)
      r = i;
    else
    {
      i = j - 0x80;
      n++;
    }
  } while ((n < 0x80) && (r == -1));

  DBG_LEAVE (); //EXIT //calc_following_order
  return r;
}
