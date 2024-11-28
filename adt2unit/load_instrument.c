// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
void load_instrument (void *data, uint8_t chan)
{
  chan--;
  //
  fmpar_table[chan].connect =  ((uint8_t *)data)[10]       & 1;
  fmpar_table[chan].feedb   = (((uint8_t *)data)[10] >> 1) & 7;
  fmpar_table[chan].multipM =  ((uint8_t *)data)[0]        & 0x0F;
  fmpar_table[chan].kslM    =  ((uint8_t *)data)[2]  >> 6;
  fmpar_table[chan].tremM   =  ((uint8_t *)data)[0]  >> 7;
  fmpar_table[chan].vibrM   = (((uint8_t *)data)[0]  >> 6) & 1;
  fmpar_table[chan].ksrM    = (((uint8_t *)data)[0]  >> 4) & 1;
  fmpar_table[chan].sustM   = (((uint8_t *)data)[0]  >> 5) & 1;
  fmpar_table[chan].multipC =  ((uint8_t *)data)[1]        & 0x0F;
  fmpar_table[chan].kslC    =  ((uint8_t *)data)[3]  >> 6;
  fmpar_table[chan].tremC   =  ((uint8_t *)data)[1]  >> 7;
  fmpar_table[chan].vibrC   = (((uint8_t *)data)[1]  >> 6) & 1;
  fmpar_table[chan].ksrC    = (((uint8_t *)data)[1]  >> 4) & 1;
  fmpar_table[chan].sustC   = (((uint8_t *)data)[1]  >> 5) & 1;

  fmpar_table[chan].adsrw_car.attck = ((uint8_t *)data)[5] >> 4;
  fmpar_table[chan].adsrw_mod.attck = ((uint8_t *)data)[4] >> 4;
  fmpar_table[chan].adsrw_car.dec   = ((uint8_t *)data)[5] & 0x0F;
  fmpar_table[chan].adsrw_mod.dec   = ((uint8_t *)data)[4] & 0x0F;
  fmpar_table[chan].adsrw_car.sustn = ((uint8_t *)data)[7] >> 4;
  fmpar_table[chan].adsrw_mod.sustn = ((uint8_t *)data)[6] >> 4;
  fmpar_table[chan].adsrw_car.rel   = ((uint8_t *)data)[7] & 0x0F;
  fmpar_table[chan].adsrw_mod.rel   = ((uint8_t *)data)[6] & 0x0F;
  fmpar_table[chan].adsrw_car.wform = ((uint8_t *)data)[9] & 0x07;
  fmpar_table[chan].adsrw_mod.wform = ((uint8_t *)data)[8] & 0x07;

  panning_table[chan] = ((uint8_t *)data)[11] & 3;
  volume_table[chan] = concw (((uint8_t *)data)[2] & 0x3F,
                              ((uint8_t *)data)[3] & 0x3F);

  update_modulator_adsrw (chan + 1);
  update_carrier_adsrw (chan + 1);
  update_fmpar (chan + 1);
}
