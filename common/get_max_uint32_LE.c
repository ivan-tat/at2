// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

uint32_t get_max_uint32_LE (const uint32_t *values, size_t count)
{
  uint32_t result = 0;

  for (size_t i = 0; i < count; i++)
  {
    uint32_t x = uint32_LE (values[i]);
    if (result < x) result = x;
  }

  return result;
}
