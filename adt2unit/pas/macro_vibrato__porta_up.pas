// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure macro_vibrato__porta_up(chan: Byte; depth: Byte);

var
  freq: Word;

begin
  freq := calc_freq_shift_up(macro_table[chan].vib_freq AND $1fff,depth);
  If (freq <= nFreq(12*8+1)) then change_freq(chan,freq)
  else change_freq(chan,nFreq(12*8+1));
end;
