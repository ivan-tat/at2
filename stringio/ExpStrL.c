// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t ExpStrL (const String *str, uint8_t size, char chr) {
  String_t r;
  size_t len = Length (str);
  const unsigned char *s = GetStr (str);

  if (len < size) {
    memset (r.str, chr, size - len);
    if (len)
      memcpy (r.str + size - len, s, len);
    len = size;
  } else
    memcpy (r.str, s, len);

  r.len = len;

  return r;
}
