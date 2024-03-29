// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void PosChar (uint8_t x, uint8_t y) {
  absolute_pos = ((y-1)*MaxCol + x-1) << 1;
}

static void DupChar (uint8_t x, uint8_t y, char c, uint8_t attr,
                     uint8_t count, void *dest) {

  // exactly the same as in `PosChar':
  absolute_pos = ((y-1)*MaxCol + x-1) << 1;

  if (count) {
    uint_least16_t a = c + (attr << 8);
    uint16_t *p = (uint16_t *) (((char *) dest) + absolute_pos);

    do {
      *p = a;
      p++;
    } while (--count);
  }
}
