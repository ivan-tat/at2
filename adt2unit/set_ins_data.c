// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
void set_ins_data (uint8_t ins, uint8_t chan) {
  uint8_t old_ins;

  if ((ins != event_table[chan - 1].instr_def) || reset_chan[chan - 1]) {
      opl3out (_instr[2] + _chan_m[chan - 1], 63);
      opl3out (_instr[3] + _chan_c[chan - 1], 63);

      if (pan_lock[chan - 1])
        panning_table[chan - 1] = songdata.lock_flags[chan - 1] & 3;
      else
        panning_table[chan - 1] = ins_parameter (ins, 11);

      opl3out (_instr[ 0] + _chan_m[chan - 1], ins_parameter (ins,  0));
      opl3out (_instr[ 1] + _chan_c[chan - 1], ins_parameter (ins,  1));
      opl3out (_instr[ 4] + _chan_m[chan - 1], ins_parameter (ins,  4));
      opl3out (_instr[ 5] + _chan_c[chan - 1], ins_parameter (ins,  5));
      opl3out (_instr[ 6] + _chan_m[chan - 1], ins_parameter (ins,  6));
      opl3out (_instr[ 7] + _chan_c[chan - 1], ins_parameter (ins,  7));
      opl3out (_instr[ 8] + _chan_m[chan - 1], ins_parameter (ins,  8));
      opl3out (_instr[ 9] + _chan_c[chan - 1], ins_parameter (ins,  9));
      opl3out (_instr[10] + _chan_n[chan - 1], ins_parameter (ins, 10)
                                               | _panning[panning_table[chan - 1]]);

      fmpar_table[chan - 1].connect =  ins_parameter (ins, 10)       & 1;
      fmpar_table[chan - 1].feedb   = (ins_parameter (ins, 10) >> 1) & 7;
      fmpar_table[chan - 1].multipM =  ins_parameter (ins,  0)       & 0x0F;
      fmpar_table[chan - 1].kslM    =  ins_parameter (ins,  2) >> 6;
      fmpar_table[chan - 1].tremM   =  ins_parameter (ins,  0) >> 7;
      fmpar_table[chan - 1].vibrM   = (ins_parameter (ins,  0) >> 6) & 1;
      fmpar_table[chan - 1].ksrM    = (ins_parameter (ins,  0) >> 4) & 1;
      fmpar_table[chan - 1].sustM   = (ins_parameter (ins,  0) >> 5) & 1;
      fmpar_table[chan - 1].multipC =  ins_parameter (ins,  1)       & 0x0F;
      fmpar_table[chan - 1].kslC    =  ins_parameter (ins,  3) >> 6;
      fmpar_table[chan - 1].tremC   =  ins_parameter (ins,  1) >> 7;
      fmpar_table[chan - 1].vibrC   = (ins_parameter (ins,  1) >> 6) & 1;
      fmpar_table[chan - 1].ksrC    = (ins_parameter (ins,  1) >> 4) & 1;
      fmpar_table[chan - 1].sustC   = (ins_parameter (ins,  1) >> 5) & 1;

      fmpar_table[chan - 1].adsrw_car.attck = ins_parameter (ins, 5) >> 4;
      fmpar_table[chan - 1].adsrw_mod.attck = ins_parameter (ins, 4) >> 4;
      fmpar_table[chan - 1].adsrw_car.dec   = ins_parameter (ins, 5) & 0x0F;
      fmpar_table[chan - 1].adsrw_mod.dec   = ins_parameter (ins, 4) & 0x0F;
      fmpar_table[chan - 1].adsrw_car.sustn = ins_parameter (ins, 7) >> 4;
      fmpar_table[chan - 1].adsrw_mod.sustn = ins_parameter (ins, 6) >> 4;
      fmpar_table[chan - 1].adsrw_car.rel   = ins_parameter (ins, 7) & 0x0F;
      fmpar_table[chan - 1].adsrw_mod.rel   = ins_parameter (ins, 6) & 0x0F;
      fmpar_table[chan - 1].adsrw_car.wform = ins_parameter (ins, 9) & 0x07;
      fmpar_table[chan - 1].adsrw_mod.wform = ins_parameter (ins, 8) & 0x07;

      if (reset_chan[chan - 1]) {
        voice_table[chan - 1] = ins;
        reset_ins_volume (chan);
        reset_chan[chan - 1] = false;
      } else
        keyoff_loop[chan - 1] = false;

      if ((1 <= (event_table[chan - 1].note & 0x7F))
      &&  ((event_table[chan - 1].note & 0x7F) <= 12 * 8 + 1))
        init_macro_table (chan, event_table[chan - 1].note & 0x7F, ins, freq_table[chan - 1]);
      else
        init_macro_table (chan, 0, ins, freq_table[chan - 1]);
  }

  vscale_table[chan - 1] = concw (fmpar_table[chan - 1].kslM << 6,
                                  fmpar_table[chan - 1].kslC << 6);

  voice_table[chan - 1] = ins;
  old_ins = event_table[chan - 1].instr_def;
  event_table[chan - 1].instr_def = ins;

  if ((!volume_lock[chan - 1]) || (ins != old_ins))
    reset_ins_volume (chan);

#if !ADT2PLAY
  ai_table[ins - 1] = 1;
#endif // !ADT2PLAY
}
