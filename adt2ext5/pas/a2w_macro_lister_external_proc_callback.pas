// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext5.pas"

procedure a2w_macro_lister_external_proc_callback;

var
  test_ins1: Byte;
  test_ins2: Byte;
  curr_inst: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'a2w_macro_lister_external_proc_callback');

  If (get_4op_to_test_temp <> 0) then
    begin
      test_ins1 := LO(get_4op_to_test_temp);
      test_ins2 := HI(get_4op_to_test_temp);
    end
  else
    begin
      curr_inst := mn_environment.curr_pos;
      If NOT (curr_inst in [1..255]) then
        begin
          _dbg_leave; EXIT; //a2w_macro_lister_external_proc_callback
        end;
      test_ins1 := curr_inst;
      test_ins2 := 0;
    end;
  If NOT shift_pressed and NOT alt_pressed and NOT ctrl_pressed then
    test_instrument_alt(count_channel(pattern_hpos),
                        mn_environment.keystroke,
                        FALSE,FALSE,TRUE,     // test current instr, with current FM-data and macro
                        test_ins1,test_ins2); // and chosen Arp./Vib. macro tables from bank

  _dbg_leave; //EXIT //a2w_macro_lister_external_proc_callback
end;
