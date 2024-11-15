// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// ins: 1..., chan: 1..20
void set_ins_data (uint8_t ins, uint8_t chan)
{
  int_least16_t old_ins;

  chan--;
  ins--;

  if ((ins != (event_table[chan].instr_def - 1)) || reset_chan[chan])
  {
    opl3out (_instr[2] + _chan_m[chan], 63);
    opl3out (_instr[3] + _chan_c[chan], 63);

    panning_table[chan] = (!pan_lock[chan]) ? ins_parameter (ins + 1, 11)
                                            : (songdata.lock_flags[chan] & 3);

    opl3out (_instr[ 0] + _chan_m[chan], ins_parameter (ins + 1,  0));
    opl3out (_instr[ 1] + _chan_c[chan], ins_parameter (ins + 1,  1));
    opl3out (_instr[ 4] + _chan_m[chan], ins_parameter (ins + 1,  4));
    opl3out (_instr[ 5] + _chan_c[chan], ins_parameter (ins + 1,  5));
    opl3out (_instr[ 6] + _chan_m[chan], ins_parameter (ins + 1,  6));
    opl3out (_instr[ 7] + _chan_c[chan], ins_parameter (ins + 1,  7));
    opl3out (_instr[ 8] + _chan_m[chan], ins_parameter (ins + 1,  8));
    opl3out (_instr[ 9] + _chan_c[chan], ins_parameter (ins + 1,  9));
    opl3out (_instr[10] + _chan_n[chan], ins_parameter (ins + 1, 10)
                                         | _panning[panning_table[chan]]);

    fmpar_table[chan].connect =  ins_parameter (ins + 1, 10)       & 1;
    fmpar_table[chan].feedb   = (ins_parameter (ins + 1, 10) >> 1) & 7;
    fmpar_table[chan].multipM =  ins_parameter (ins + 1,  0)       & 0x0F;
    fmpar_table[chan].kslM    =  ins_parameter (ins + 1,  2) >> 6;
    fmpar_table[chan].tremM   =  ins_parameter (ins + 1,  0) >> 7;
    fmpar_table[chan].vibrM   = (ins_parameter (ins + 1,  0) >> 6) & 1;
    fmpar_table[chan].ksrM    = (ins_parameter (ins + 1,  0) >> 4) & 1;
    fmpar_table[chan].sustM   = (ins_parameter (ins + 1,  0) >> 5) & 1;
    fmpar_table[chan].multipC =  ins_parameter (ins + 1,  1)       & 0x0F;
    fmpar_table[chan].kslC    =  ins_parameter (ins + 1,  3) >> 6;
    fmpar_table[chan].tremC   =  ins_parameter (ins + 1,  1) >> 7;
    fmpar_table[chan].vibrC   = (ins_parameter (ins + 1,  1) >> 6) & 1;
    fmpar_table[chan].ksrC    = (ins_parameter (ins + 1,  1) >> 4) & 1;
    fmpar_table[chan].sustC   = (ins_parameter (ins + 1,  1) >> 5) & 1;

    fmpar_table[chan].adsrw_car.attck = ins_parameter (ins + 1, 5) >> 4;
    fmpar_table[chan].adsrw_mod.attck = ins_parameter (ins + 1, 4) >> 4;
    fmpar_table[chan].adsrw_car.dec   = ins_parameter (ins + 1, 5) & 0x0F;
    fmpar_table[chan].adsrw_mod.dec   = ins_parameter (ins + 1, 4) & 0x0F;
    fmpar_table[chan].adsrw_car.sustn = ins_parameter (ins + 1, 7) >> 4;
    fmpar_table[chan].adsrw_mod.sustn = ins_parameter (ins + 1, 6) >> 4;
    fmpar_table[chan].adsrw_car.rel   = ins_parameter (ins + 1, 7) & 0x0F;
    fmpar_table[chan].adsrw_mod.rel   = ins_parameter (ins + 1, 6) & 0x0F;
    fmpar_table[chan].adsrw_car.wform = ins_parameter (ins + 1, 9) & 0x07;
    fmpar_table[chan].adsrw_mod.wform = ins_parameter (ins + 1, 8) & 0x07;

    if (!reset_chan[chan])
      keyoff_loop[chan] = false;
    else
    {
      voice_table[chan] = ins + 1;
      reset_ins_volume (chan + 1);
      reset_chan[chan] = false;
    }

    if (   ((event_table[chan].note & 0x7F) >= 1)
        && ((event_table[chan].note & 0x7F) <= (12*8+1)))
      init_macro_table (chan + 1, event_table[chan].note & 0x7F, ins + 1, freq_table[chan]);
    else
      init_macro_table (chan + 1, 0, ins + 1, freq_table[chan]);
  }

  vscale_table[chan] =   ((fmpar_table[chan].kslM & 3) <<  6)
                       + ((fmpar_table[chan].kslC & 3) << 14);
  voice_table[chan] = ins + 1;
  old_ins = event_table[chan].instr_def - 1;
  event_table[chan].instr_def = ins + 1;

  if ((!volume_lock[chan]) || (ins != old_ins))
    reset_ins_volume (chan + 1);

#if !ADT2PLAY
  ai_table[ins] = 1;
#endif // !ADT2PLAY
}
