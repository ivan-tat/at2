// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure add_bank_position(bank_name: String; bank_size: Longint; bank_position: Longint);

var
  idx,idx2: Longint;
  found_flag: Boolean;

begin
  _dbg_enter ({$I %FILE%}, 'add_bank_position');

  found_flag := FALSE;
  bank_name := CutStr(Upper_filename(bank_name));
  For idx := 1 to bank_position_list_size do
    If (bank_position_list[idx].bank_name = bank_name) and
       ((bank_position_list[idx].bank_size = bank_size) or
        (bank_size = -1)) then
      begin
        found_flag := TRUE;
        idx2 := idx;
        BREAK;
      end;

  If found_flag then
    begin
      bank_position_list[idx2].bank_position := bank_position;
      _dbg_leave; EXIT; //add_bank_position
    end;

  If (bank_position_list_size < MAX_NUM_BANK_POSITIONS) then
    Inc(bank_position_list_size)
  else
    begin
      bank_position_list_size := MAX_NUM_BANK_POSITIONS;
      For idx := 1 to PRED(bank_position_list_size) do
        bank_position_list[idx] := bank_position_list[idx+1];
    end;

  bank_position_list[bank_position_list_size].bank_name := bank_name;
  bank_position_list[bank_position_list_size].bank_size := bank_size;
  bank_position_list[bank_position_list_size].bank_position := bank_position;

  _dbg_leave; //EXIT //add_bank_position
end;
