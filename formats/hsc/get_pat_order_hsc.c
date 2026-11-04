// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_pat_order_hsc (tFIXED_SONGDATA *song,
                               uint8_t *pat_order, unsigned pat_order_len)
{
  for (unsigned i = 0; i < pat_order_len; i++)
  {
    uint8_t pat = pat_order[i];

    if (pat < PATTERNS_MAX_HSC)
      song->pattern_order[i] = pat;
    else if ((pat & 0x80) && ((pat & 0x7F) < PATTERN_ORDER_LEN_HSC))
      song->pattern_order[i] = PATTERN_ORDER_JUMP + (pat & 0x7F);
    else
      song->pattern_order[i] = PATTERN_ORDER_JUMP + 0;  // acts as `stop'
  }
}
