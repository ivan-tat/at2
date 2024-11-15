// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ void generate_custom_vibrato (uint8_t value)
{
  if (value == 0)
  {
    // 0: set default speed table
    vibtrem_table_size = def_vibtrem_table_size;
    memmove (vibtrem_table, def_vibtrem_table, sizeof (vibtrem_table));
  }
  else if (value <= 239)
  {
    // 1..239: set custom speed table (fixed size = 32)
    float k = value / 16.0;
    unsigned i, n;
    uint8_t *a, *b;

    vibtrem_table_size = def_vibtrem_table_size;

    // Fill first 32 values

    vibtrem_table[0] = 0;

    for (i = 1; i <= 16; i++)
      vibtrem_table[i] = i * k;

    for (i = 17; i <= 31; i++)
      vibtrem_table[i] = (32 - i) * k;

    // Copy first 32 values to the rest

    a = vibtrem_table;
    b = &vibtrem_table[32];
    n = (sizeof (vibtrem_table) / sizeof (vibtrem_table[0])) - 32;
    do
    {
      *b = *a;
      a++;
      b++;
    } while (--n);
  }
  else
  {
    // 240..255: set custom speed table (speed factor = 1..4)
    unsigned l, k, i, n;

    value -= 240; // 0..15
    vibtrem_speed_factor = (value % 4) + 1; // 1..4
    l = 16 << (value >> 2); // 16, 32, 64, 128
    vibtrem_table_size = 2 * l; // 32, 64, 128, 256 // FIXME: 256 is truncated to 0
    k = (sizeof (vibtrem_table) / sizeof (vibtrem_table[0])) / l; // 2, 4, 8, 16

    // Fill first (2 * l) values

    vibtrem_table[0] = 0;

    for (i = 1; i <= l; i++)
    {
      int32_t x = i * k - 1;

      vibtrem_table[i] = (x >= 0) ? x : 0;
    }

    n = 2 * l - 1;
    for (i = l + 1; i <= n; i++)
    {
      int32_t x = (2 * l - i) * k - 1;

      vibtrem_table[i] = (x >= 0) ? x : 0;
    }

    // Copy first (2 * l) values to the rest

    n = (sizeof (vibtrem_table) / sizeof (vibtrem_table[0])) - 2 * l;
    if (n)
    {
      uint8_t *a = vibtrem_table, *b = &vibtrem_table[2 * l];

      do
      {
        *b = *a;
        a++;
        b++;
      } while (--n);
    }
  }
}
