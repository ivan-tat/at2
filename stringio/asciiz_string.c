// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t asciiz_string (const String *str) {
  String_t r;
  size_t len = Length (str);

  if (len) {
    len = strnlen ((const char *)GetStr (str), len);
    if (len)
      memcpy (r.str, GetStr (str), len);
  }

  r.len = len;

  return r;
}
