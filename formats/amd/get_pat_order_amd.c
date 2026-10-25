// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_pat_order_amd (tFIXED_SONGDATA *dst,
                               pat_order_amd_t *src, uint8_t pat_order_len,
                               unsigned num_patterns,
                               uint8_t *state)
{
  if (pat_order_len > PATTERN_ORDER_LEN_AMD)
    pat_order_len = PATTERN_ORDER_LEN_AMD;

  for (unsigned i = 0; i < pat_order_len; i++)
    if ((*src)[i] < num_patterns)
    {
      dst->pattern_order[i] = (*src)[i];
      *state = 2;
    }
}
