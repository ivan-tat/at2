// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext5.pas"

function get_4op_to_test_temp: Word;

var
  result: Word;
  curr_inst: Byte;

begin
  result := 0;
  curr_inst := mn_environment.curr_pos;
  If (curr_inst in [1..255]) and (songdata.flag_4op <> 0) then
    If (_4op_flag_column[curr_inst] = _4op_flag_chr_beg) then
      result := SUCC(curr_inst)+curr_inst SHL 8
    else If (curr_inst > 1) and (_4op_flag_column[curr_inst] = _4op_flag_chr_end) then
           result := curr_inst+PRED(curr_inst) SHL 8;
  get_4op_to_test_temp := result;
end;
