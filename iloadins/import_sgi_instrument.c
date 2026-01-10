// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void import_sgi_instrument (tADTRACK2_INS *ins, const void *data)
{
  DBG_ENTER ("import_sgi_instrument");

  ins->fm_data.ATTCK_DEC_modulator =  (((uint8_t *)data)[1]  & 0x0F)
                                   + ((((uint8_t *)data)[0]  & 0x0F) << 4);
  ins->fm_data.SUSTN_REL_modulator =  (((uint8_t *)data)[3]  & 0x0F)
                                   + ((((uint8_t *)data)[2]  & 0x0F) << 4);
  ins->fm_data.WAVEFORM_modulator  =  (((uint8_t *)data)[4]  &    3);
  ins->fm_data.KSL_VOLUM_modulator =  (((uint8_t *)data)[7]  & 0x3F)
                                   + ((((uint8_t *)data)[6]  &    3) << 6);
  ins->fm_data.AM_VIB_EG_modulator =  (((uint8_t *)data)[5]  & 0x0F)
                                   + ((((uint8_t *)data)[8]  &    1) << 4)
                                   + ((((uint8_t *)data)[11] &    1) << 5)
                                   + ((((uint8_t *)data)[10] &    1) << 6)
                                   + ((((uint8_t *)data)[9]  &    1) << 7);
  ins->fm_data.ATTCK_DEC_carrier   =  (((uint8_t *)data)[13] & 0x0F)
                                   + ((((uint8_t *)data)[12] & 0x0F) << 4);
  ins->fm_data.SUSTN_REL_carrier   =  (((uint8_t *)data)[15] & 0x0F)
                                   + ((((uint8_t *)data)[14] & 0x0F) << 4);
  ins->fm_data.WAVEFORM_carrier    =  (((uint8_t *)data)[16] &    3);
  ins->fm_data.KSL_VOLUM_carrier   =  (((uint8_t *)data)[19] & 0x3F)
                                   + ((((uint8_t *)data)[18] &    3) << 6);
  ins->fm_data.AM_VIB_EG_carrier   =  (((uint8_t *)data)[17] & 0x0F)
                                   + ((((uint8_t *)data)[20] &    1) << 4)
                                   + ((((uint8_t *)data)[23] &    1) << 5)
                                   + ((((uint8_t *)data)[22] &    1) << 6)
                                   + ((((uint8_t *)data)[21] &    1) << 7);
  ins->fm_data.FEEDBACK_FM         =  (((uint8_t *)data)[25] &    1)
                                   + ((((uint8_t *)data)[24] &    7) << 1);

  DBG_LEAVE (); //EXIT //import_sgi_instrument
}
