// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void status_refresh (void)
{
  struct main_window_t *w_main;
  struct status_window_t *w_status;
  struct pattern_editor_window_t *w_pattern_editor;
  bool w_status_is_visible;

  DBG_ENTER ("status_refresh");

  if (really_no_status_refresh)
  {
    DBG_LEAVE (); //status_refresh
    return;
  }

  w_main = &main_window;
  w_status = &status_window;
  w_pattern_editor = &pattern_editor_window;

  // TODO: choose the right place to do this
  w_main->area.bottom_right.x = MAX_COLUMNS;
  w_main->area.bottom_right.y = w_main->area.top_left.y + 10 + MAX_PATTERN_ROWS + 1;
  w_pattern_editor->area.bottom_right.x = w_main->area.bottom_right.x - 1;
  w_pattern_editor->area.bottom_right.y = w_main->area.bottom_right.y - 1;

  w_status_is_visible = _is_status_window_visible ();

  main_window__status_refresh (w_main, !w_status_is_visible);
  status_window__status_refresh (w_status, w_status_is_visible);
  pattern_editor_window__status_refresh (w_pattern_editor);

  if (no_status_refresh || single_play)
  {
    DBG_LEAVE (); //status_refresh
    return;
  }

  if ((!_IRQFREQ_blink_flag) && _IRQFREQ_update_event)
    _IRQFREQ_update_event = false;

  // this split is made because of `_IRQFREQ_update_event' variable change
  // (used in `status' and `main' windows' routines)

  main_window__status_refresh2 (w_main);
  status_window__status_refresh2 (w_status);
  pattern_editor_window__status_refresh2 (w_pattern_editor);

  DBG_LEAVE (); //EXIT //status_refresh
}
