// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure portamento_down(chan: Byte; slide: Word; limit: Word);

var
  freq: Word;

begin
  If (freq_table[chan] AND $1fff = 0) then
    EXIT; //portamento_down
  freq := calc_freq_shift_down(freq_table[chan] AND $1fff,slide);
  If (freq >= limit) then change_frequency(chan,freq)
  else change_frequency(chan,limit);

  //EXIT //portamento_down
end;
