// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext5.pas"

function check_4op_flag_temp(ins: Byte): Boolean;

var
  result: Boolean;
  idx: Byte;

begin
  result := FALSE;
  For idx := 1 to temp_songdata.ins_4op_flags.num_4op do
    If (temp_songdata.ins_4op_flags.idx_4op[idx] = ins) then
      begin
        result := TRUE;
        BREAK;
      end;
  check_4op_flag_temp := result;
end;
