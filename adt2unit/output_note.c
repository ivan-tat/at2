// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// note: 1..12*8+1, ins: 1..., chan: 1..20
void output_note (uint8_t note, uint8_t ins, uint8_t chan,
                  bool restart_macro, bool restart_adsr)
{
  uint16_t freq;

  chan--;

  if ((note == 0) && (ftune_table[chan] == 0))
    return; //output_note

  if ((note < 1) || (note > 12*8+1))
    freq = freq_table[chan];
  else
  {
#if ADT2PLAY
    uint8_t pos;
#endif // ADT2PLAY

    freq = nFreq (note - 1) + (int8_t)(ins_parameter (ins, 12));

    if (restart_adsr)
      key_on (chan + 1);

    freq_table[chan] |= 0x2000;

#if ADT2PLAY
    pos = (uint8_t)((double)note * (DECAY_BARS-1) / (12*8+1));
    if (decay_bar[pos].lvl != 0.0)
    {
      if ((pos > 0) && (decay_bar[pos - 1].dir != 1))
        pos--;
      else if ((pos < DECAY_BARS-1) && (decay_bar[pos + 1].dir != 1))
        pos++;
    }

    decay_bar[pos].dir = 1;
    if (is_4op_chan (chan + 1))
    {
      if (!(ins_parameter (voice_table[chan], 10) & 1))
        decay_bar[pos].max_lvl = (  carrier_vol[chan - 1]
                                  + carrier_vol[chan]) / 2;
      else
        decay_bar[pos].max_lvl = (  carrier_vol[chan - 1]
                                  + modulator_vol[chan - 1]
                                  + carrier_vol[chan]
                                  + modulator_vol[chan]) / 4;
    }
    else
    {
      if (!(ins_parameter (voice_table[chan], 10) & 1))
        decay_bar[pos].max_lvl = carrier_vol[chan];
      else
        decay_bar[pos].max_lvl = (  carrier_vol[chan]
                                  + modulator_vol[chan]) / 2;
    }

#else // !ADT2PLAY
    if (channel_flag[chan])
    {
      if (is_4op_chan (chan + 1))
      {
        if (!(percussion_mode && (chan >= 16) && (chan <= 19)))
        {
          if (ins_parameter (voice_table[chan], 10) & 1)
          {
            if (volum_bar[chan].lvl < (carrier_vol[chan] + modulator_vol[chan]) / 2)
              volum_bar[chan].dir = 1;
          }
          else
          {
            if (volum_bar[chan].lvl < carrier_vol[chan])
              volum_bar[chan].dir = 1;
          }
        }
        else
        {
          if (volum_bar[chan].lvl < modulator_vol[chan])
            volum_bar[chan].dir = 1;
        }

        if (!(percussion_mode && ((chan - 1) >= 16) && ((chan - 1) <= 19)))
        {
          if (ins_parameter (voice_table[chan - 1], 10) & 1)
          {
            if (volum_bar[chan - 1].lvl < (carrier_vol[chan - 1] + modulator_vol[chan - 1]) / 2)
              volum_bar[chan - 1].dir = 1;
          }
          else
          {
            if (volum_bar[chan - 1].lvl < carrier_vol[chan - 1])
              volum_bar[chan - 1].dir = 1;
          }
        }
        else
        {
          if (volum_bar[chan - 1].lvl < modulator_vol[chan - 1])
            volum_bar[chan - 1].dir = 1;
        }

        if (decay_bar[chan].lvl1 < carrier_vol[chan])
          decay_bar[chan].dir1 = 1;

        if (decay_bar[chan].lvl2 < modulator_vol[chan])
          decay_bar[chan].dir2 = 1;

        if (decay_bar[chan - 1].lvl1 < carrier_vol[chan - 1])
          decay_bar[chan - 1].dir1 = 1;

        if (decay_bar[chan - 1].lvl2 < modulator_vol[chan - 1])
          decay_bar[chan - 1].dir2 = 1;

        if (play_status != isPlaying)
        {
          volum_bar[chan].dir = -1;
          decay_bar[chan].dir1 = -1;
          decay_bar[chan].dir2 = -1;
          decay_bar[chan - 1].dir1 = -1;
          decay_bar[chan - 1].dir2 = -1;
        }
      }
      else
      {
        if (!(percussion_mode && (chan >= 16) && (chan <= 19)))
        {
          if (ins_parameter (voice_table[chan], 10) & 1)
          {
            if (volum_bar[chan].lvl < (carrier_vol[chan] + modulator_vol[chan]) / 2)
              volum_bar[chan].dir = 1;
          }
          else
          {
            if (volum_bar[chan].lvl < carrier_vol[chan])
              volum_bar[chan].dir = 1;
          }
        }
        else
          if (volum_bar[chan].lvl < modulator_vol[chan])
            volum_bar[chan].dir = 1;

        if (decay_bar[chan].lvl1 < carrier_vol[chan])
          decay_bar[chan].dir1 = 1;

        if (decay_bar[chan].lvl2 < modulator_vol[chan])
          decay_bar[chan].dir2 = 1;

        if (play_status != isPlaying)
        {
          volum_bar[chan].dir = -1;
          decay_bar[chan].dir1 = -1;
          decay_bar[chan].dir2 = -1;
        }
      }
    }
#endif // !ADT2PLAY
  }

  if (ftune_table[chan] == -127)
    ftune_table[chan] = 0;

  freq += ftune_table[chan];
  change_frequency (chan + 1, freq);

  if (note)
  {
    event_table[chan].note = note;

    if (is_4op_chan (chan + 1))
      event_table[chan - 1].note = note;

    if (restart_macro)
    {
      tCHUNK *et = &event_table[chan];

      if ((    (et->effect_def   == ef_Extended)
           && ((et->effect / 16) == ef_ex_ExtendedCmd2)
           && ((et->effect % 16) == ef_ex_cmd2_NoRestart))
          || (    (et->effect_def2   == ef_Extended)
              && ((et->effect2 / 16) == ef_ex_ExtendedCmd2)
              && ((et->effect2 % 16) == ef_ex_cmd2_NoRestart)))
        macro_table[chan].arpg_note = note;
      else
        init_macro_table (chan + 1, note, ins, freq);
    }
  }

  //EXIT //output_note
}
