// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure change_frequency(chan: Byte; freq: Word);
begin
  macro_table[chan].vib_paused := TRUE;
  change_freq(chan,freq);

  If is_4op_chan(chan) then
    If (chan in _4op_tracks_hi) then
      begin
        macro_table[SUCC(chan)].vib_count := 1;
        macro_table[SUCC(chan)].vib_pos := 0;
        macro_table[SUCC(chan)].vib_freq := freq;
        macro_table[SUCC(chan)].vib_paused := FALSE;
      end
    else
      begin
        macro_table[PRED(chan)].vib_count := 1;
        macro_table[PRED(chan)].vib_pos := 0;
        macro_table[PRED(chan)].vib_freq := freq;
        macro_table[PRED(chan)].vib_paused := FALSE;
      end;

  macro_table[chan].vib_count := 1;
  macro_table[chan].vib_pos := 0;
  macro_table[chan].vib_freq := freq;
  macro_table[chan].vib_paused := FALSE;
end;
