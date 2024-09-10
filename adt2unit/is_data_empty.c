// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Return value: `true' if empty
/*static*/ bool is_data_empty (const void *buf, size_t size) {
  const char *p = buf;

  if (size < 16) {
    while (size) {
      size--;
      if (*p)
        return false;
      p++;
    }
  } else {
    size_t count = size / sizeof (int);
    size_t tail = size % sizeof (int);

    while (count) {
      count--;
      if (*(int *)p)
        return false;
      p += sizeof (int);
    }

    while (tail) {
      tail--;
      if (*p)
        return false;
      p++;
    }
  }

  return true;
}
