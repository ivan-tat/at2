// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
// note: 1..12*8+1 (0 if none)
// ins: 1..255
void init_macro_table (uint8_t chan, uint8_t note, uint8_t ins, uint16_t freq)
{
  chan--; // 0..19
  ins--; // 0..254

  macro_table[chan].fmreg_count = 1;
  macro_table[chan].fmreg_pos = 0;
  macro_table[chan].fmreg_duration = 0;
  macro_table[chan].fmreg_table = ins + 1;
  macro_table[chan].arpg_count = 1;
  macro_table[chan].arpg_pos = 0;
  macro_table[chan].arpg_table = songdata.instr_macros[ins].arpeggio_table;
  macro_table[chan].arpg_note = note;
  macro_table[chan].vib_count = 1;
  macro_table[chan].vib_paused = false;
  macro_table[chan].vib_pos = 0;
  macro_table[chan].vib_table = songdata.instr_macros[ins].vibrato_table;
  macro_table[chan].vib_freq = freq;
  macro_table[chan].vib_delay = songdata.macro_table[macro_table[chan].vib_table - 1].vibrato.delay;
  zero_fq_table[chan] = 0;
}
