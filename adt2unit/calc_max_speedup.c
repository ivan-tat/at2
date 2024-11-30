// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint16_t calc_max_speedup (uint8_t tempo)
{
  uint16_t r = MAX_IRQ_FREQ / tempo; // result
  int_least32_t x;

  DBG_ENTER ("calc_max_speedup");

  do
  {
    if ((tempo == PIT_FREQ_FLOOR) && timer_fix)
      x = trunc ((tempo + 0.2) * 20);
    else
      x = 250;

    while (x % (tempo * r))
      x++;

    if (x <= MAX_IRQ_FREQ)
      r++;
  }
  while (x <= MAX_IRQ_FREQ);

  DBG_LEAVE (); //EXIT //calc_max_speedup
  return r - 1;
}
