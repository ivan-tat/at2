// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void decay_bars_refresh (void)
{
  struct decay_bars_window_t *w_decay_bars;

  DBG_ENTER ("decay_bars_refresh");

  w_decay_bars = &decay_bars_window;

  // TODO: choose the right place to do this
  w_decay_bars->area.top_left.y = 1 + 7 + 3 + MAX_PATTERN_ROWS + 1 + 1;
  w_decay_bars->area.bottom_right.x = MAX_COLUMNS;
  w_decay_bars->area.bottom_right.y = w_decay_bars->area.top_left.y + 8;

  decay_bars_window__draw_overall_volume_bar (w_decay_bars);
  decay_bars_window__draw_global_volume_bar (w_decay_bars);
  decay_bars_window__update_decay_bar_levels (w_decay_bars);
  decay_bars_window__draw_decay_bars (w_decay_bars);

  DBG_LEAVE (); //EXIT //decay_bars_refresh
}
