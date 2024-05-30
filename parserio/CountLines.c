// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

size_t CountLines (const char *buf, size_t size) {
  size_t r = 0;

  while (size) {
    bool f = *buf == '\r';

    buf++;
    size--;

    if (f && size && (*buf == '\n')) {
      buf++;
      size--;
      r++;
    }
  }

  return r;
}
