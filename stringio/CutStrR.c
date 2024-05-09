// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t CutStrR (const String *str, uint8_t margin) {
  String_t r;
  size_t len = Length (str);
  size_t count = 1 + (margin < len) ? len - margin : 0;
  const unsigned char *s = GetStr (str);

  len--;
  while (count && (s[len] == ' ')) {
    count--;
    len--;
  }

  r.len = len + 1;
  memcpy (r.str, s, r.len);

  return r;
}
