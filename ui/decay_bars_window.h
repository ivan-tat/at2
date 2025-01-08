// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*** DECAY BARS window ***/

// Track's vertical volume bar

#define DECAY_BARS_WINDOW__TRACK_WIDTH_MAX 15
#define DECAY_BARS_WINDOW__BAR_HEIGHT_MAX 7

extern struct decay_bars_window_t
{
  struct text_rect_t area;
  uint8_t track_width; // in characters
  uint8_t bar_height; // in characters
  uint8_t levels[20][2];
}
decay_bars_window;
