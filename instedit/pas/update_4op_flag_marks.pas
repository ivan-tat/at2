// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure update_4op_flag_marks;

var
  temp: Byte;

begin
  For temp := 1 to PRED(255) do
    If NOT check_4op_flag(temp) then
      begin
        If (songdata.instr_names[temp][1] in _4op_flag_chars) then
          songdata.instr_names[temp][1] := ' ';
        If (songdata.instr_names[SUCC(temp)][1] in _4op_flag_chars) then
          songdata.instr_names[SUCC(temp)][1] := ' ';
      end;
  For temp := 1 to PRED(255) do
    If check_4op_flag(temp) then
      begin
        If (songdata.instr_names[temp][1] = ' ') then
          songdata.instr_names[temp][1] := _4op_flag_chr_beg;
        If (songdata.instr_names[SUCC(temp)][1] = ' ') then
          songdata.instr_names[SUCC(temp)][1] := _4op_flag_chr_end;
      end;
end;
