// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ uint16_t calc_freq_shift_down (uint16_t freq, uint16_t shift) {
  uint_least16_t a, b, c;

  a = (freq & 0x03FF/*00000011 11111111*/) - shift;
  b =  freq & 0x1C00/*00011100 00000000*/;
  c =  freq & 0xE000/*11100000 00000000*/;

  if (a <= FreqStart) {
    if (b) {
      a += FreqRange;
      b -= 1 << 10;
    } else
      a = FreqStart;
  }

  return a + b + c;
}
