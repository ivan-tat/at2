// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
void reset_chan_data (uint8_t chan)
{
  chan--;

  if (percussion_mode && (chan >= 16) && (chan <= 19))
  {
    if (channel_flag[chan])
      reset_ins_volume (chan + 1);
    else
      set_ins_volume (0, 0, chan + 1);
  }
  else
  {
    opl3out (_instr[2] + _chan_m[chan], 63);
    opl3out (_instr[3] + _chan_c[chan], 63);
    key_on (chan + 1);

    opl3out (_instr[4] + _chan_m[chan], BYTE_NULL);
    opl3out (_instr[5] + _chan_c[chan], BYTE_NULL);
    opl3out (_instr[6] + _chan_m[chan], BYTE_NULL);
    opl3out (_instr[7] + _chan_c[chan], BYTE_NULL);
    key_off (chan + 1);
    update_fmpar (chan + 1);
  }

  reset_adsrw[chan] = true;
  if (play_status != isStopped)
  {
    uint8_t note = event_table[chan].note & 0x7F;

    if (!((note >= 1) && (note <= 12*8+1)))
      note = 0;

    init_macro_table (chan + 1, note, voice_table[chan], freq_table[chan]);
  }
}
