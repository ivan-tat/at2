// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t Lower (const String *str) {
  String_t r;
  size_t len = Length (str);
  const unsigned char *s = GetStr (str);
  unsigned char *d = r.str;

  r.len = len;

  while (len) {
    *d = LoCase (*s);
    s++;
    d++;
    len--;
  }

  return r;
}
