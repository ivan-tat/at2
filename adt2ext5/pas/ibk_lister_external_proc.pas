// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext5.pas"

procedure ibk_lister_external_proc;
begin
  _dbg_enter ({$I %FILE%}, 'ibk_lister_external_proc');

  mn_environment.curr_item :=
    Num2str(mn_environment.curr_pos+ibk_skip,10)+'.ibk';
  If NOT shift_pressed and NOT alt_pressed and NOT ctrl_pressed then
    test_instrument_alt2(count_channel(pattern_hpos),
                         mn_environment.keystroke);

  _dbg_leave; //EXIT //ibk_lister_external_proc
end;
