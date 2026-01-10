// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void import_bnk_instrument (tADTRACK2_INS *ins, const tBNK_DATA_RECORD *rec)
{
  DBG_ENTER ("import_bnk_instrument");

  memset (ins, 0, sizeof (*ins));

  ins->fm_data.AM_VIB_EG_modulator =  (rec->modulator.freq_mult & 0x0F)
                                   + ((rec->modulator.ksr       &    1) << 4)
                                   + ((rec->modulator.sustain   &    1) << 5)
                                   + ((rec->modulator.vib       &    1) << 6)
                                   + ((rec->modulator.am        &    1) << 7);

  ins->fm_data.AM_VIB_EG_carrier =  (rec->carrier.freq_mult & 0x0F)
                                 + ((rec->carrier.ksr       &    1) << 4)
                                 + ((rec->carrier.sustain   &    1) << 5)
                                 + ((rec->carrier.vib       &    1) << 6)
                                 + ((rec->carrier.am        &    1) << 7);

  ins->fm_data.KSL_VOLUM_modulator =  (rec->modulator.output & 0x3F)
                                   + ((rec->modulator.ksl    &    3) << 6);

  ins->fm_data.KSL_VOLUM_carrier =  (rec->carrier.output & 0x3F)
                                 + ((rec->carrier.ksl    &    3) << 6);

  ins->fm_data.ATTCK_DEC_modulator =  (rec->modulator.decay  & 0x0F)
                                   + ((rec->modulator.attack & 0x0F) << 4);

  ins->fm_data.ATTCK_DEC_carrier =  (rec->carrier.decay  & 0x0F)
                                 + ((rec->carrier.attack & 0x0F) << 4);

  ins->fm_data.SUSTN_REL_modulator =  (rec->modulator.release    & 0xF0)
                                   + ((rec->modulator.sust_level & 0xF0) << 4);

  ins->fm_data.SUSTN_REL_carrier =  (rec->carrier.release    & 0x0F)
                                 + ((rec->carrier.sust_level & 0x0F) << 4);

  ins->fm_data.WAVEFORM_modulator = rec->wform_mod & 7;

  ins->fm_data.WAVEFORM_carrier = rec->wform_car & 7;

  ins->fm_data.FEEDBACK_FM = ((rec->modulator.fm       & 1) ^  1)
                           + ((rec->modulator.feedback & 7) << 1);

  ins->perc_voice = (rec->voice_num >= 6) && (rec->voice_num <= 10) ? rec->voice_num - 6 + 1 : 0;

  DBG_LEAVE (); //EXIT //import_bnk_instrument
}
