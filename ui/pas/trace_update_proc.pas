// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure trace_update_proc;
begin
  _dbg_enter ({$I %FILE%}, 'trace_update_proc');

  space_pressed := scankey(SC_SPACE);
  If (play_status = isPlaying) then
    If (_traceprc_last_order <> current_order) or
       (_traceprc_last_pattern <> current_pattern) or
       (_traceprc_last_line <> current_line) then
      begin
        _traceprc_last_order := current_order;
        _traceprc_last_pattern := songdata.pattern_order[current_order];
        _traceprc_last_line := current_line;

        If tracing then
          begin
            If NOT play_single_patt then
              pattern_patt := songdata.pattern_order[current_order];

            pattern_page := current_line;
            pattord_page := 0;
            pattord_hpos := 1;
            pattord_vpos := 1;

            While (current_order <> pattord_vpos+4*(pattord_hpos+pattord_page-1)-1) do
              If (pattord_vpos < 4) then Inc(pattord_vpos)
              else If (pattord_hpos < MAX_ORDER_COLS) then begin Inc(pattord_hpos); pattord_vpos := 1; end
                   else If (pattord_page < 23-(MAX_ORDER_COLS-9)) then begin Inc(pattord_page); pattord_vpos := 1; end;
          end;

        PATTERN_ORDER_page_refresh(pattord_page);
        PATTERN_page_refresh(pattern_page);
      end;

  _dbg_leave; //EXIT //trace_update_proc
end;
