// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure tremolo(chan: Byte);

var
  vol,old_vol: Word;

begin
  Inc(trem_table[chan].pos,trem_table[chan].speed*vibtrem_speed_factor);
  vol := calc_vibtrem_shift(chan,trem_table);
  old_vol := volume_table[chan];
  If (trem_table[chan].dir = 0) then slide_volume_down(chan,vol)
  else slide_volume_up(chan,vol);
  volume_table[chan] := old_vol;
end;
