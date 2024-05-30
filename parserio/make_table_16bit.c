// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void make_table_16bit (void) {
  unsigned i;

  for (i = 0; i < 0x100; i++) {
    uint_least16_t x = i;
    unsigned n;

    for (n = 8; n; n--)
      x = (x & 1) ? (x >> 1) ^ 0xA001 : (x >> 1);

    CRC16_table[i] = x;
  }
}
