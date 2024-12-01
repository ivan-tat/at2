// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
// level: 0..63
void show_bar (uint8_t chan, uint8_t level, uint8_t xpos, uint8_t from_pos,
               uint8_t to_pos, uint8_t attr)
{
  String r[BAR_HEIGHT_MAX+2+1], *s = GetStr (r); // final string
  uint_least8_t i; // last position in the string
  uint8_t x0, x1, y; // output screen coordinates
  uint8_t attrs[2]; // output screen attributes

  chan--;

  if (level)
  {
    uint_least8_t full; // number of full-filled characters (0 = none)
    uint_least8_t tail; // character for tail (0 = none)

    // scale `level' to fit in `bar_height' characters (`bar_subdivs' lines each)
    level = (uint16_t)(level * bar_height * bar_subdivs + 31) / 63; // (+31 for rounding)

    full = level / bar_subdivs; // 0..bar_height
    tail = level % bar_subdivs;

    i = bar_height + 2; // +2 characters for color switching
    SetLength (r, i);

    i--;
    s[i] = '~'; // switch color

    while (full)
    {
      i--;
      s[i] = bar_char_full;
      full--;
    }

    if (tail)
    {
      i--;
      s[i] = bar_char_tail_first + tail - 1;
    }

    i--;
    s[i] = '~'; // switch color
  }
  else
  {
    i = 7;
    SetLength (r, i);
  }

  while (i)
  {
    i--;
    s[i] = bar_char_none;
  }

  x0 = xpos + chan * 15 + from_pos - 1;
  x1 = xpos + chan * 15 + to_pos - 1;
  y = MAX_PATTERN_ROWS + 14;
  attrs[0] = analyzer_bckg + analyzer;
  attrs[1] = analyzer_bckg + (attr >> 4);

  for (uint8_t x = x0; x <= x1; x++)
    show_vcstr (x, y, r, attrs[0], attrs[1]);
}
