// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function check_4op_instrument(ins: Byte): Word;

var
  result: Word;

begin
  result := 0;
  If check_4op_flag(ins) then
    result := SUCC(ins)+ins SHL 8
  else If (ins > 1) and check_4op_flag(PRED(ins)) then
         result := ins+PRED(ins) SHL 8;
  check_4op_instrument := result;
end;
