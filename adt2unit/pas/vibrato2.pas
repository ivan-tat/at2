// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure vibrato2(chan: Byte);

var
  freq,old_freq: Word;

begin
  Inc(vibr_table2[chan].pos,vibr_table2[chan].speed*vibtrem_speed_factor);
  freq := calc_vibtrem_shift(chan,vibr_table2);
  old_freq := freq_table[chan];
  If (vibr_table2[chan].dir = 0) then portamento_down(chan,freq,nFreq(0))
  else portamento_up(chan,freq,nFreq(12*8+1));
  freq_table[chan] := old_freq;
end;
