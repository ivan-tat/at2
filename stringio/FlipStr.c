// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t FlipStr (const String *str) {
  String_t r;
  size_t len = Length (str);

  r.len = len;

  if (len) {
    const unsigned char *s = GetStr (str) + len - 1;
    unsigned char *d = r.str;

    do {
      *d = *s;
      d++;
      s--;
    } while (--len);
  }

  return r;
}
