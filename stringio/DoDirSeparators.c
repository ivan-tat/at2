// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void DoDirSeparators (String *p) {
  size_t count = Length (p);
  unsigned char *s = GetStr (p);

  while (count) {
    if ((*s == '/') || (*s == '\\'))
      *s = DIR_SEP;
    s++;
    count--;
  }
}
