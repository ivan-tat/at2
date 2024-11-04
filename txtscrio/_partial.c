// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// TODO: optimize using integer math
static uint16_t _partial (uint16_t max, uint16_t val, uint8_t base)
{
  uint16_t r = 0;
  double step = (double) max / base;
  double x = (double) step / 2;

  while (x < val)
  {
    x += step;
    r++;
  }

  return r;
}
