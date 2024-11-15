// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure init_macro_table(chan,note,ins: Byte; freq: Word);
begin
  macro_table[chan].fmreg_count := 1;
  macro_table[chan].fmreg_pos := 0;
  macro_table[chan].fmreg_duration := 0;
  macro_table[chan].fmreg_table := ins;
  macro_table[chan].arpg_count := 1;
  macro_table[chan].arpg_pos := 0;
  macro_table[chan].arpg_table := songdata.instr_macros[ins].arpeggio_table;
  macro_table[chan].arpg_note := note;
  macro_table[chan].vib_count := 1;
  macro_table[chan].vib_paused := FALSE;
  macro_table[chan].vib_pos := 0;
  macro_table[chan].vib_table := songdata.instr_macros[ins].vibrato_table;
  macro_table[chan].vib_freq := freq;
  macro_table[chan].vib_delay := songdata.macro_table[macro_table[chan].vib_table].vibrato.delay;
  zero_fq_table[chan] := 0;
end;
