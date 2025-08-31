// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
// board_pos: 1..29
static bool test_instrument_alt_output_note (uint8_t chan, uint8_t board_pos, uint8_t ins1, uint8_t ins2)
{
  bool result = false;
  int_least16_t note;
  uint16_t freq;

  DBG_ENTER ("test_instrument_alt_output_note");

  chan--; // chan: 0..19
  board_pos--; // board_pos: 0..28

  note = board_pos + 12 * (current_octave - 1);
  if ((note < 0) || (note > 12*8+1)) goto _exit;

  chan_handle[chan] = board_scancodes[board_pos];
  if (ins2 != 0) chan = _4op_main_chan[chan] - 1;
  load_instrument (&temp_instrument, chan + 1);

  if (   percussion_mode
      && (   (temp_instrument.perc_voice == 4)
          || (temp_instrument.perc_voice == 5)))
    load_instrument (&temp_instrument, _perc_sim_chan[chan - 18]);

  if (ins2 != 0)
    load_instrument (&temp_instrument2, chan - 1);

  macro_speedup = songdata.macro_speedup;
  if (play_status == isStopped) update_timer (songdata.tempo);

  freq = nFreq (note - 1) + 0x2000 + ((int8_t *)&temp_instrument)[12];
  event_table[chan].note = note;
  freq_table[chan] = freq;
  freqtable2[chan] = freq;
  key_on (chan + 1);
  change_freq (chan + 1, freq);

  init_macro_table (chan + 1, note, ins1, freq);
  if (ins2 != 0)
  {
    freq_table[chan - 1] = freq;
    freqtable2[chan - 1] = freq;
    init_macro_table (chan - 1 + 1, note, ins2, freq);
  }

  result = true;

_exit:
  DBG_LEAVE (); //EXIT //test_instrument_alt_output_note
  return result;
}
