// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool Empty (const char *buf, size_t size) {
  if (size < 16) {
    while (size) {
      if (*buf)
        return false;
      buf++;
      size--;
    }
  } else {
    size_t count = size / sizeof (int), tail = size % sizeof (int);

    while (count) {
      if ((*(int *)buf))
        return false;
      buf += sizeof (int);
      count--;
    }

    while (tail) {
      if (*buf)
        return false;
      buf++;
      tail--;
    }
  }

  return true;
}
