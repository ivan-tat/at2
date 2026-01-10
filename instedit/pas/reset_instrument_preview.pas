// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure reset_instrument_preview;

var
  temp: Word;

begin
  If (play_status <> isPlaying) then reset_player;
  temp := check_4op_to_test;
  If NOT (temp <> 0) then
    update_instr_data(instrum_page)
  else begin
         update_instr_data(LO(temp));
         update_instr_data(HI(temp));
       end;
end;
