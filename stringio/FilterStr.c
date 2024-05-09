// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t FilterStr (const String *str, char chr0, char chr1) {
  String_t r;
  size_t len = Length (str);

  r.len = len;

  if (len) {
    size_t i = 0;
    const unsigned char *s = GetStr (str);

    do {
      r.str[i] = (s[i] == chr0) ? chr1 : s[i];
      i++;
      len--;
    } while (len);
  }

  return r;
}
