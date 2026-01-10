// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext3.pas"

procedure fselect_external_proc; cdecl;

var
  test_ins1: Byte;
  test_ins2: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'fselect_external_proc');

  If (songdata.flag_4op <> 0) then
    begin
      If (current_inst < 255) then
        test_ins1 := current_inst
      else test_ins1 := PRED(current_inst);
      test_ins2 := SUCC(test_ins1);
    end
  else
    begin
      test_ins1 := current_inst;
      test_ins2 := 0;
    end;
  If NOT shift_pressed and NOT alt_pressed and NOT ctrl_pressed then
    test_instrument_alt(count_channel(pattern_hpos),
                        mn_environment.keystroke,
                        FALSE,FALSE,FALSE,
                        test_ins1,test_ins2);

  _dbg_leave; //EXIT //fselect_external_proc
end;
