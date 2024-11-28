// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function get_bank_position(bank_name: String; bank_size: Longint): Longint;

var
  idx: Longint;
  result: Longint;

begin
  _dbg_enter ({$I %FILE%}, 'get_bank_position');

  result := 0;
  bank_name := CutStr(Upper_filename(bank_name));
  For idx := 1 to bank_position_list_size do
    If (bank_position_list[idx].bank_name = bank_name) and
       ((bank_position_list[idx].bank_size = bank_size) or
        (bank_size = -1)) then
      begin
        result := bank_position_list[idx].bank_position;
        BREAK;
      end;
  get_bank_position := result;

  _dbg_leave; //EXIT //get_bank_position
end;
