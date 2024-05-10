// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t ExpC2StrR (const String *str, uint8_t size, char chr) {
  String_t r;
  size_t len = Length (str), clen = CStr2Len (str);

  if (len)
    memcpy (r.str, GetStr (str), len);

  if (clen < size) {
    size_t n = size - clen;

    if (n + len > 255) // Check target buffer overrun
      n = 255 - len;

    if (n) {
      memset (r.str + len, chr, n);
      len += n;
    }
  }

  r.len = len;

  return r;
}
