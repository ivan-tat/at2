// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void import_sbi_instrument_alt (tADTRACK2_INS *ins, const void *data)
{
  DBG_ENTER ("import_sbi_instrument_alt");

  ins->fm_data.AM_VIB_EG_modulator = ((uint8_t *)data)[0];
  ins->fm_data.AM_VIB_EG_carrier   = ((uint8_t *)data)[1];
  ins->fm_data.KSL_VOLUM_modulator = ((uint8_t *)data)[2];
  ins->fm_data.KSL_VOLUM_carrier   = ((uint8_t *)data)[3];
  ins->fm_data.ATTCK_DEC_modulator = ((uint8_t *)data)[4];
  ins->fm_data.ATTCK_DEC_carrier   = ((uint8_t *)data)[5];
  ins->fm_data.SUSTN_REL_modulator = ((uint8_t *)data)[6];
  ins->fm_data.SUSTN_REL_carrier   = ((uint8_t *)data)[7];
  ins->fm_data.WAVEFORM_modulator  = ((uint8_t *)data)[8]  & 3;
  ins->fm_data.WAVEFORM_carrier    = ((uint8_t *)data)[9]  & 3;
  ins->fm_data.FEEDBACK_FM         = ((uint8_t *)data)[10] & 0x0F;

  ins->panning = 0;
  ins->fine_tune = 0;

  DBG_LEAVE (); //EXIT //import_sbi_instrument_alt
}
