// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// instr: 0..254
// instr2: 0..254 (-1 if none)
// chan: 0..19
// note: 0..12*8
static bool INSTRUMENT_test_output_note (int16_t instr, int16_t instr2,
                                         uint8_t chan, uint8_t note,
                                         bool _1op_preview, bool _4op_mode, bool process_macros)
{
  uint16_t freq;
  tADTRACK2_INS ins;

  DBG_ENTER ("INSTRUMENT_test_output_note");

  if (_4op_mode) chan = _4op_main_chan[chan] - 1; // chan: 0..19

  if (_1op_preview)
  {
    uint_least8_t i;

    ins = songdata.instr_data[   _instedit_operator_enabled[0]
                              || _instedit_operator_enabled[1] ? instr : instr2];
    ins.fm_data.FEEDBACK_FM |= 1;
    load_instrument (&ins, chan + 1);
    i =  _instedit_operator_enabled[0]
      || _instedit_operator_enabled[1] ? 0 : 2;
    set_ins_volume (0x3F - (_instedit_operator_enabled[i + 0] ? 0x3F - (volume_table[chan] & 0xFF) : 0),
                    0x3F - (_instedit_operator_enabled[i + 1] ? 0x3F - (volume_table[chan] >> 8) : 0),
                    chan + 1);
  }
  else
  {
    load_instrument (&songdata.instr_data[instr], chan + 1);
    set_ins_volume (0x3F - (_instedit_operator_enabled[0] ? 0x3F - (volume_table[chan] & 0xFF) : 0),
                    0x3F - (_instedit_operator_enabled[1] ? 0x3F - (volume_table[chan] >> 8) : 0),
                    chan + 1);
    if (percussion_mode && (   (songdata.instr_data[instr].perc_voice == 4)
                            || (songdata.instr_data[instr].perc_voice == 5)))
      load_instrument (&songdata.instr_data[instr], _perc_sim_chan[chan]);
    if (_4op_mode)
    {
      load_instrument (&songdata.instr_data[instr2], chan - 1 + 1);
      set_ins_volume (0x3F - (_instedit_operator_enabled[2] ? 0x3F - (volume_table[chan - 1] & 0xFF) : 0),
                      0x3F - (_instedit_operator_enabled[3] ? 0x3F - (volume_table[chan - 1] >> 8) : 0),
                      chan - 1 + 1);
    }
  }

  macro_speedup = songdata.macro_speedup;
  if (play_status == isStopped) update_timer (songdata.tempo);

  freq = nFreq (note) + 0x2000 + songdata.instr_data[instr].fine_tune;
  event_table[chan].note = note + 1;
  freq_table[chan] = freq;
  freqtable2[chan] = freq;
  key_on (chan + 1);
  change_freq (chan + 1, freq);

  if (process_macros)
    init_macro_table (chan + 1, note + 1,
                      (_1op_preview && (   _instedit_operator_enabled[2]
                                        || _instedit_operator_enabled[3]) ? instr2 : instr) + 1,
                      freq);
  else
  {
    macro_table[chan].fmreg_table = 0;
    macro_table[chan].arpg_table = 0;
    macro_table[chan].vib_table = 0;
  }

  if (_4op_mode && !_1op_preview)
  {
    if (process_macros)
      init_macro_table (chan - 1 + 1, note + 1, instr2 + 1, freq);
    else
    {
      macro_table[chan - 1].fmreg_table = 0;
      macro_table[chan - 1].arpg_table = 0;
      macro_table[chan - 1].vib_table = 0;
    }
  }

  DBG_LEAVE (); //EXIT //INSTRUMENT_test_output_note
  return false;
}
