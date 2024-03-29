// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void ShowVStr (tSCREEN_MEM *dest, uint8_t x, uint8_t y, const String *str,
               uint8_t attr) {

  uint_least8_t n; // number of input characters left to proceed

  PosChar (x, y); // set `absolute_pos'

  n = Length (str);
  if (n) {
    const unsigned char *s; // input string
    uint16_t *p; // output screen address
    uint_least16_t a; // prepared attribute

    s = GetString (str);
    p = (uint16_t *) (((char *) dest) + absolute_pos);
    a = attr << 8;
    do {
      *p = *s | a;
      s++;
      p += MaxCol; // vertical step, in 2-bytes units
    } while (--n);
  }
}
