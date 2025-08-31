// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool Empty (const void *buf, size_t size)
{
  if (size < 4 * sizeof (int))
  {
    for (; size; size--) if (*((char *)buf++)) return false;
  }
  else
  {
    size_t count = size / sizeof (int), tail = size % sizeof (int);

    for (; count; count--) if (*((int *)buf++)) return false;

    for (; tail; tail--) if (*((char *)buf++)) return false;
  }

  return true;
}
