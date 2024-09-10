// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure key_off(chan: Byte);
begin
  freq_table[chan] := LO(freq_table[chan])+
                      (HI(freq_table[chan]) AND NOT $20) SHL 8;
  change_freq(chan,freq_table[chan]);
  event_table[chan].note := event_table[chan].note OR keyoff_flag;
end;
