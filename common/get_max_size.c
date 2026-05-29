// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

size_t get_max_size (const size_t *values, size_t count)
{
  size_t result = 0;

  for (size_t i = 0; i < count; i++)
    if (result < values[i]) result = values[i];

  return result;
}
