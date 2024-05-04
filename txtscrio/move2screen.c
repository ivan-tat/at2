// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void move2screen (void) {
  void *screen_ptr_backup;

#if GO32
  DBG_ENTER ("move2screen");
#endif // GO32
  HideCursor ();
  screen_ptr_backup = screen_ptr;
  screen_ptr = move_to_screen_data;
  area_x1 = 0;
  area_y1 = 0;
  area_x2 = 0;
  area_y2 = 0;
  scroll_pos0 = UINT8_NULL;
  scroll_pos1 = UINT8_NULL;
  scroll_pos2 = UINT8_NULL;
  scroll_pos3 = UINT8_NULL;
  scroll_pos4 = UINT8_NULL;
  PATTERN_ORDER_page_refresh (pattord_page);
  PATTERN_page_refresh (pattern_page);
  status_refresh ();
  decay_bars_refresh ();
  ScreenMemCopy (screen_ptr, screen_ptr_backup);
  screen_ptr = screen_ptr_backup;
  SetCursor (cursor_backup);
}
