// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t CutStrL (const String *str, uint8_t margin) {
  String_t r;
  size_t len = Length (str), i = 0;
  const unsigned char *s = GetStr (str);

  if ((!margin) || (margin > len))
    margin = len;

  while ((i < margin) && (s[i] == ' '))
    i++;

  len -= i;
  r.len = len;
  if (len)
    memcpy (r.str, s + i, len);

  return r;
}
