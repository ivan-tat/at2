// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t FilterStr1 (const String *str, char chr0) {
  String_t r;
  size_t len = Length (str);

  if (len) {
    size_t n = len;
    const unsigned char *s = GetStr (str);
    unsigned char *d = r.str;

    len = 0;
    do {
      if (*s != chr0) {
        *d = *s;
        d++;
        len++;
      }
      s++;
    } while (--n);
  }

  r.len = len;

  return r;
}
