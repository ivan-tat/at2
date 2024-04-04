// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure move2screen;

var
  screen_ptr_backup: Pointer;

begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'TXTSCRIO.PAS:move2screen';
{$ENDIF}
  HideCursor;
  screen_ptr_backup := screen_ptr;
  screen_ptr := move_to_screen_data;
  area_x1 := 0;
  area_y1 := 0;
  area_x2 := 0;
  area_y2 := 0;
  scroll_pos0 := BYTE_NULL;
  scroll_pos1 := BYTE_NULL;
  scroll_pos2 := BYTE_NULL;
  scroll_pos3 := BYTE_NULL;
  scroll_pos4 := BYTE_NULL;
  PATTERN_ORDER_page_refresh(pattord_page);
  PATTERN_page_refresh(pattern_page);
  status_refresh;
  decay_bars_refresh;
  ScreenMemCopy(screen_ptr,screen_ptr_backup);
  screen_ptr := screen_ptr_backup;
  SetCursor(cursor_backup);
end;
