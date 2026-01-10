// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure set_4op_flag(ins: Byte);

var
  temp_ins_4op_flags: tINS_4OP_FLAGS;
  idx: Byte;

begin
  If (ins = 255) then
    EXIT; //set_4op_flag
  If check_4op_flag(ins) then
    reset_4op_flag(ins);
  If (ins > 1) and check_4op_flag(PRED(ins)) then
    reset_4op_flag(PRED(ins));
  If (ins < 255) and check_4op_flag(SUCC(ins)) then
    reset_4op_flag(SUCC(ins));
  temp_ins_4op_flags.num_4op := 0;
  idx := 1;
  While (idx <= songdata.ins_4op_flags.num_4op) and
        (songdata.ins_4op_flags.idx_4op[idx] < ins) do
    begin
      Inc(temp_ins_4op_flags.num_4op);
      temp_ins_4op_flags.idx_4op[temp_ins_4op_flags.num_4op] :=
        songdata.ins_4op_flags.idx_4op[idx];
      Inc(idx);
    end;
  Inc(temp_ins_4op_flags.num_4op);
  temp_ins_4op_flags.idx_4op[temp_ins_4op_flags.num_4op] := ins;
  While (idx <= songdata.ins_4op_flags.num_4op) do
    begin
      Inc(temp_ins_4op_flags.num_4op);
      temp_ins_4op_flags.idx_4op[temp_ins_4op_flags.num_4op] :=
        songdata.ins_4op_flags.idx_4op[idx];
      Inc(idx);
    end;
  songdata.ins_4op_flags := temp_ins_4op_flags;

  //EXIT //set_4op_flag
end;
