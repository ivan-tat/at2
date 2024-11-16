// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int16_t calc_order_jump (void)
{
  int16_t r = 0; // result
  uint8_t n = 0; // jumps count

  DBG_ENTER ("calc_order_jump");

  do
  {
    if (songdata.pattern_order[current_order] >= 0x80)
      current_order = songdata.pattern_order[current_order] - 0x80;

    n++;
  } while ((n < 0x80) && (songdata.pattern_order[current_order] >= 0x80));

  if (n >= 0x80)
  {
    stop_playing ();
    r = -1;
  }

  DBG_LEAVE (); //EXIT //calc_order_jump
  return r;
}
