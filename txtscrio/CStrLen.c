// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint8_t CStrLen (const String *str) {

  const unsigned char *s; // input string
  uint_least8_t n; // number of input characters left to proceed
  uint_least8_t len; // actual number of printable characters

  len = 0;

  n = Length (str);
  s = GetStr (str);
  while (n) {
    if (*s != '~') // color switch mark
      len++;
    s++;
    n--;
  }

  return len;
}
