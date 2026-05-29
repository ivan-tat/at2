// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

uint16_t get_max_uint16_LE (const uint16_t *values, size_t count)
{
  uint16_t result = 0;

  for (size_t i = 0; i < count; i++)
  {
    uint16_t x = uint16_LE (values[i]);
    if (result < x) result = x;
  }

  return result;
}
