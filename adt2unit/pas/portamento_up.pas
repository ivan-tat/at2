// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure portamento_up(chan: Byte; slide: Word; limit: Word);

var
  freq: Word;

begin
  If (freq_table[chan] AND $1fff = 0) then
    EXIT; //portamento_up
  freq := calc_freq_shift_up(freq_table[chan] AND $1fff,slide);
  If (freq <= limit) then change_frequency(chan,freq)
  else change_frequency(chan,limit);

  //EXIT //portamento_up
end;
