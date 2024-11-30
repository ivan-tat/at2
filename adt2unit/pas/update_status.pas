// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure update_status;

var
  temp: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'calibrate_player.update_status');

  temp := songdata.pattern_order[current_order];
  If NOT (temp <= $7f) then temp := 0;
  show_str(17,03,byte2hex(current_order),pattern_bckg+status_dynamic_txt);
  show_str(20,03,byte2hex(temp),pattern_bckg+status_dynamic_txt);
  show_str(17,04,'--',pattern_bckg+status_dynamic_txt);
{$IFNDEF GO32V2}
  _draw_screen_without_delay := TRUE;
{$ENDIF}
  draw_screen;

  _dbg_leave; //EXIT //calibrate_player.update_status
end;
