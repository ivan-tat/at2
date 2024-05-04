// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t ExpStrR (const String *str, uint8_t size, char chr) {
  const char *s = (const char *) GetStr (str);
  uint_least8_t len = Length (str);
  String_t r;

  if (len)
    memcpy (r.str, s, len);

  if (len < size) {
    memset (r.str + len, chr, size - len);
    len = size;
  }

  r.len = len;

  return r;
}
