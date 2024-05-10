// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t ExpC2StrL (const String *str, uint8_t size, char chr) {
  String_t r;
  size_t len = Length (str), clen = CStr2Len (str); // All are 0-255

  if (clen < size) {
    size_t n = size - clen;

    memset (r.str, chr, n);

    if (n + len > 255) // Check target buffer overrun
      len = 255 - n;

    if (len)
      memcpy (r.str + n, GetStr (str), len);

    len += n;
  } else
    memcpy (r.str, GetStr (str), len);

  r.len = len;

  return r;
}
