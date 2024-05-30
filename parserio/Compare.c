// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool Compare (const char *buf1, const char *buf2, size_t size) {
  if (size < 16) {
    while (size) {
      if (*buf1 != *buf2)
        return false;
      buf1++;
      buf2++;
      size--;
    }
  } else {
    size_t count = size / sizeof (int), tail = size % sizeof (int);

    while (count) {
      if (*(int *)buf1 != *(int *)buf2)
        return false;
      buf1 += sizeof (int);
      buf2 += sizeof (int);
      count--;
    }

    while (tail) {
      if (*buf1 != *buf2)
        return false;
      buf1++;
      buf2++;
      tail--;
    }
  }

  return true;
}
