// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void init_buffers (void)
{
  DBG_ENTER ("init_buffers");

  memset (fmpar_table, 0, sizeof (fmpar_table));
  memset (pan_lock, panlock, sizeof (pan_lock));
#if ADT2PLAY
  memset (volume_table, 0, sizeof (volume_table));
#else // !ADT2PLAY
  memset (volume_table, 63, sizeof (volume_table));
#endif // !ADT2PLAY
  memset (vscale_table, 0, sizeof (vscale_table));
  memset (modulator_vol, 0, sizeof (modulator_vol));
  memset (carrier_vol, 0, sizeof (carrier_vol));
#if !ADT2PLAY
  memset (decay_bar, 0, sizeof (decay_bar));
  memset (volum_bar, 0, sizeof (volum_bar));
#endif // !ADT2PLAY
  memset (event_table, 0, sizeof (event_table));
  memset (freq_table, 0, sizeof (freq_table));
  memset (zero_fq_table, 0, sizeof (zero_fq_table));
  memset (effect_table, 0, sizeof (effect_table));
  memset (effect_table2, 0, sizeof (effect_table2));
  memset (fslide_table, 0, sizeof (fslide_table));
  memset (fslide_table2, 0, sizeof (fslide_table2));
  memset (glfsld_table, 0, sizeof (glfsld_table));
  memset (glfsld_table2, 0, sizeof (glfsld_table2));
  memset (porta_table, 0, sizeof (porta_table));
  memset (porta_table2, 0, sizeof (porta_table2));
  memset (portaFK_table, false, sizeof (portaFK_table));
  memset (arpgg_table, 0, sizeof (arpgg_table));
  memset (arpgg_table2, 0, sizeof (arpgg_table2));
  memset (vibr_table, 0, sizeof (vibr_table));
  memset (vibr_table2, 0, sizeof (vibr_table2));
  memset (trem_table, 0, sizeof (trem_table));
  memset (trem_table2, 0, sizeof (trem_table2));
  memset (retrig_table, 0, sizeof (retrig_table));
  memset (retrig_table2, 0, sizeof (retrig_table2));
  memset (tremor_table, 0, sizeof (tremor_table));
  memset (tremor_table2, 0, sizeof (tremor_table2));
  memset (last_effect, 0, sizeof (last_effect));
  memset (last_effect2, 0, sizeof (last_effect2));
  memset (voice_table, 0, sizeof (voice_table));
  memset (event_new, 0, sizeof (event_new));
#if !ADT2PLAY
  memset (freqtable2, 0, sizeof (freqtable2));
#endif // !ADT2PLAY
  memset (notedel_table, BYTE_NULL, sizeof (notedel_table));
  memset (notecut_table, BYTE_NULL, sizeof (notecut_table));
  memset (ftune_table, 0, sizeof (ftune_table));
  memset (loopbck_table, BYTE_NULL, sizeof (loopbck_table));
  memset (loop_table, BYTE_NULL, sizeof (loop_table));
  memset (reset_chan, false, sizeof (reset_chan));
#if !ADT2PLAY
  memset (reset_adsrw, false, sizeof (reset_adsrw));
#endif // !ADT2PLAY
  memset (keyoff_loop, false, sizeof (keyoff_loop));
  memset (macro_table, 0, sizeof (macro_table));
#if !ADT2PLAY
  memset (ignore_note_once, false, sizeof (ignore_note_once));
#endif // !ADT2PLAY

  if (!lockvol)
    memset (volume_lock, false, sizeof (volume_lock));
  else
    for (uint8_t i = 0; i < 20; i++)
      volume_lock[i] = (songdata.lock_flags[i] >> 4) & 1;

  if (!panlock)
    memset (panning_table, 0, sizeof (panning_table));
  else
    for (uint8_t i = 0; i < 20; i++)
      panning_table[i] = songdata.lock_flags[i] & 3;

  if (!lockVP)
    memset (peak_lock, false, sizeof (peak_lock));
  else
    for (uint8_t i = 0; i < 20; i++)
      peak_lock[i] = (songdata.lock_flags[i] >> 5) & 1;

  memset (vol4op_lock, false, sizeof (vol4op_lock));
  for (uint8_t i = 0; i < 6; i++)
  {
    vol4op_lock[_4op_main_chan[i] - 1] =
      (songdata.lock_flags[_4op_main_chan[i] - 1] & 0x40) ? true : false;
    vol4op_lock[_4op_main_chan[i] - 1 - 1] =
      (songdata.lock_flags[_4op_main_chan[i] - 1 - 1] & 0x40) ? true : false;
  }

  for (uint8_t i = 0; i < 20; i++)
    volslide_type[i] = (songdata.lock_flags[i] >> 2) & 3;

  DBG_LEAVE (); //EXIT //init_buffers
}
