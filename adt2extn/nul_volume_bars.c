// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void nul_volume_bars (void)
{
  String_t t; // 14+1 bytes

  t = ExpStrR ((String *)"\x00" "", 14, charmap.bd_heavy_horiz); // "━".."━"

  for (uint8_t i = 0; i < MAX_TRACKS; i++)
    if (channel_flag[chan_pos + i])
      show_str (8 + i * 15, 10 + MAX_PATTERN_ROWS + 1,
                (String *) &t,
                pattern_bckg + pattern_border);
}
