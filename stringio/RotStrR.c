// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t RotStrR (const String *str1, const String *str2, uint8_t shift) {
  String_t r;
  size_t l1 = Length (str1), l2 = Length (str2); // All are 0-255

  if (l2 && shift) {
    size_t i;

    if (l2 > shift) {
      i = l2 - shift;
      l2 = shift;
    } else
      i = 0;

    memcpy (r.str, GetStr (str2) + i, l2);
  } else
    l2 = 0;

  if (l1 > shift) {
    l1 -= shift;
    if (l1 > 255 - l2) // Check target buffer overrun
      l1 = 255 - l2;
    if (l1)
      memcpy (r.str + l2, GetStr (str1), l1);
  } else
    l1 = 0;

  r.len = l1 + l2;

  return r;
}
