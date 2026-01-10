// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 0..19
// note: 0..12*8
// test_ins1: 0..254
// test_ins2: 0..254 (-1 if none)
static bool test_instrument_alt_output_note (const temp_instrument_t *temp_ins,
                                             uint8_t chan, uint8_t note,
                                             int16_t test_ins1, int16_t test_ins2)
{
  uint16_t freq;

  DBG_ENTER ("test_instrument_alt_output_note");

  if (test_ins2 >= 0) chan = _4op_main_chan[chan] - 1; // chan: 0..19
  load_instrument (&temp_ins->ins1.fm, chan + 1);

  if (   percussion_mode
      && (   (temp_ins->ins1.fm.perc_voice == 4)
          || (temp_ins->ins1.fm.perc_voice == 5)))
    load_instrument (&temp_ins->ins1.fm, _perc_sim_chan[chan - 18]);

  if (test_ins2 >= 0)
    load_instrument (&temp_ins->ins2.fm, chan + 1 - 1);

  macro_speedup = songdata.macro_speedup;
  if (play_status == isStopped) update_timer (songdata.tempo);

  freq = nFreq (note) + 0x2000 + temp_ins->ins1.fm.fine_tune;
  event_table[chan].note = note + 1;
  freq_table[chan] = freq;
  freqtable2[chan] = freq;
  key_on (chan + 1);
  change_freq (chan + 1, freq);

  init_macro_table (chan + 1, note + 1, test_ins1 + 1, freq);
  if (test_ins2 >= 0)
  {
    freq_table[chan - 1] = freq;
    freqtable2[chan - 1] = freq;
    init_macro_table (chan - 1 + 1, note + 1, test_ins2 + 1, freq);
  }

  DBG_LEAVE (); //EXIT //test_instrument_alt_output_note
  return true;
}
