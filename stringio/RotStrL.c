// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t RotStrL (const String *str1, const String *str2, uint8_t shift) {
  String_t r;
  size_t l1 = Length (str1), l2 = Length (str2); // All are 0-255

  if (l1 > shift) {
    l1 -= shift;
    memcpy (r.str, GetStr (str1) + shift, l1);
  } else
    l1 = 0;

  if (l2 && shift) {
    if (l2 > shift)
      l2 = shift;
    if (l2 > 255 - l1) // Check target buffer overrun
      l2 = 255 - l1;
    if (l2)
      memcpy (r.str + l1, GetStr (str2), l2);
  } else
    l2 = 0;

  r.len = l1 + l2;

  return r;
}
