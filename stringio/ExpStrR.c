// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t ExpStrR (const String *str, uint8_t size, char chr) {
  String_t r;
  size_t len = Length (str);

  if (len)
    memcpy (r.str, GetStr (str), len);

  if (len < size) {
    memset (r.str + len, chr, size - len);
    len = size;
  }

  r.len = len;

  return r;
}
