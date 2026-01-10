// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function check_4op_to_test: Word;

var
  result: Word;

begin
  result := 0;
  If check_4op_flag(current_inst) then
    result := SUCC(current_inst)+current_inst SHL 8
  else If (current_inst > 1) and check_4op_flag(PRED(current_inst)) then
         result := current_inst+PRED(current_inst) SHL 8;
  check_4op_to_test := result;
end;
