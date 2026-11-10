// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void truncate_string_rad (const uint8_t **end_ptr, size_t *len)
{
  size_t l = *len;
  if (l != 0) // try to truncate string
  {
    const uint8_t *p = *end_ptr - 1;
    while ((l != 0) && ((*p == ' ') || (*p == (uint8_t) '\xFF')))
    {
      p--;
      l--;
    }
    *end_ptr = p + 1;
    *len = l;
  }
}
