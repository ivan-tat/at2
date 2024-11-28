// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure load_instrument(var data; chan: Byte);
begin
  fmpar_table[chan].connect := pBYTE(@data)[10] AND 1;
  fmpar_table[chan].feedb   := pBYTE(@data)[10] SHR 1 AND 7;
  fmpar_table[chan].multipM := pBYTE(@data)[0]  AND $0f;
  fmpar_table[chan].kslM    := pBYTE(@data)[2]  SHR 6;
  fmpar_table[chan].tremM   := pBYTE(@data)[0]  SHR 7;
  fmpar_table[chan].vibrM   := pBYTE(@data)[0]  SHR 6 AND 1;
  fmpar_table[chan].ksrM    := pBYTE(@data)[0]  SHR 4 AND 1;
  fmpar_table[chan].sustM   := pBYTE(@data)[0]  SHR 5 AND 1;
  fmpar_table[chan].multipC := pBYTE(@data)[1]  AND $0f;
  fmpar_table[chan].kslC    := pBYTE(@data)[3]  SHR 6;
  fmpar_table[chan].tremC   := pBYTE(@data)[1]  SHR 7;
  fmpar_table[chan].vibrC   := pBYTE(@data)[1]  SHR 6 AND 1;
  fmpar_table[chan].ksrC    := pBYTE(@data)[1]  SHR 4 AND 1;
  fmpar_table[chan].sustC   := pBYTE(@data)[1]  SHR 5 AND 1;

  fmpar_table[chan].adsrw_car.attck := pBYTE(@data)[5] SHR 4;
  fmpar_table[chan].adsrw_mod.attck := pBYTE(@data)[4] SHR 4;
  fmpar_table[chan].adsrw_car.dec   := pBYTE(@data)[5] AND $0f;
  fmpar_table[chan].adsrw_mod.dec   := pBYTE(@data)[4] AND $0f;
  fmpar_table[chan].adsrw_car.sustn := pBYTE(@data)[7] SHR 4;
  fmpar_table[chan].adsrw_mod.sustn := pBYTE(@data)[6] SHR 4;
  fmpar_table[chan].adsrw_car.rel   := pBYTE(@data)[7] AND $0f;
  fmpar_table[chan].adsrw_mod.rel   := pBYTE(@data)[6] AND $0f;
  fmpar_table[chan].adsrw_car.wform := pBYTE(@data)[9] AND $07;
  fmpar_table[chan].adsrw_mod.wform := pBYTE(@data)[8] AND $07;

  panning_table[chan] := pBYTE(@data)[11] AND 3;
  volume_table[chan] := concw(pBYTE(@data)[2] AND $3f,
                              pBYTE(@data)[3] AND $3f);

  update_modulator_adsrw(chan);
  update_carrier_adsrw(chan);
  update_fmpar(chan);
end;
