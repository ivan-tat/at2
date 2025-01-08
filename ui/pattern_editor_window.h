// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*** PATTERN EDITOR window ***/

#define PATTERN_EDITOR_WINDOW__TRACK_WIDTH_MAX 15

// Track's horizontal volume bar

#define PATTERN_EDITOR_WINDOW__VOL_BAR_WIDTH_MAX (PATTERN_EDITOR_WINDOW__TRACK_WIDTH_MAX - 1)

extern struct pattern_editor_window_t
{
  struct text_rect_t area;
  uint8_t track_width; // in characters
  uint8_t vol_bar_width; // in characters
}
pattern_editor_window;
