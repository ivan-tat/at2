// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure reset_marked_instruments;
begin
  If (marked_instruments < 2) then
   If (songdata.instr_names[instrum_page][1] = _ins_mark_chr) then
     songdata.instr_names[instrum_page][1] := ' '
   else
  else begin
         songdata.instr_names[_2nd_marked][1] := ' ';
         songdata.instr_names[_1st_marked][1] := ' ';
       end;
  update_4op_flag_marks;
end;
