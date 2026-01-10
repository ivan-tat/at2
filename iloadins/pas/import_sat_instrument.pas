// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "iloaders.inc"

procedure import_sat_instrument(inst: Byte; var data);
begin
  With songdata.instr_data[inst] do
    begin
      fm_data.FEEDBACK_FM         := pBYTE(@data)[0] AND $0f;
      fm_data.AM_VIB_EG_modulator := pBYTE(@data)[1];
      fm_data.AM_VIB_EG_carrier   := pBYTE(@data)[2];
      fm_data.ATTCK_DEC_modulator := pBYTE(@data)[3];
      fm_data.ATTCK_DEC_carrier   := pBYTE(@data)[4];
      fm_data.SUSTN_REL_modulator := pBYTE(@data)[5];
      fm_data.SUSTN_REL_carrier   := pBYTE(@data)[6];
      fm_data.WAVEFORM_modulator  := pBYTE(@data)[7] AND 3;
      fm_data.WAVEFORM_carrier    := pBYTE(@data)[8] AND 3;
      fm_data.KSL_VOLUM_modulator := pBYTE(@data)[9];
      fm_data.KSL_VOLUM_carrier   := pBYTE(@data)[10];
    end;

  songdata.instr_data[inst].panning := 0;
  songdata.instr_data[inst].fine_tune := 0;
end;
