// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns number of patterns in pattern order
static int get_pat_order_dfm (tFIXED_SONGDATA *dst, pat_order_dfm_t *src)
{
  int r = -1;

  for (uint_least8_t pos = 0; pos < PATTERN_ORDER_LEN_DFM; pos++)
  {
    uint_least8_t pat = (*src)[pos];

    if (pat < PATTERNS_MAX_DFM)
      dst->pattern_order[pos] = pat;
    else if (pat == PATTERN_ORDER_END_DFM)
    {
      if (r < 0) r = pos;
      break;
    }
    else
      dst->pattern_order[pos] = PATTERN_ORDER_JUMP + pos;
  }

  return r;
}
