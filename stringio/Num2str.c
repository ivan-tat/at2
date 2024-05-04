// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t Num2str (uint32_t num, uint8_t base) {
  String_t r;

  if (num && (base >= 2) && (base <= 16)) {
    char str[32];
    uint_least8_t len = 0, i = 0;

    do {
      char n = num % base;

      num /= base;
      str[len++] = n + ((n <= 9) ? '0' : ('A' - 10));
    } while (num);

    r.len = len;

    do {
      r.str[i++] = str[--len];
    } while (len);
  } else {
    r.len = 1;
    r.str[0] = '0';
  }

  return r;
}
