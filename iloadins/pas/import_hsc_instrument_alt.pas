// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure import_hsc_instrument_alt(var data);
begin
  _dbg_enter ({$I %FILE%}, 'import_hsc_instrument_alt');

  With temp_instrument do
    begin
      fm_data.AM_VIB_EG_carrier   := pBYTE(@data)[0];
      fm_data.AM_VIB_EG_modulator := pBYTE(@data)[1];
      fm_data.KSL_VOLUM_carrier   := pBYTE(@data)[2];
      fm_data.KSL_VOLUM_modulator := pBYTE(@data)[3];
      fm_data.ATTCK_DEC_carrier   := pBYTE(@data)[4];
      fm_data.ATTCK_DEC_modulator := pBYTE(@data)[5];
      fm_data.SUSTN_REL_carrier   := pBYTE(@data)[6];
      fm_data.SUSTN_REL_modulator := pBYTE(@data)[7];
      fm_data.FEEDBACK_FM         := pBYTE(@data)[8]  AND $0f;
      fm_data.WAVEFORM_carrier    := pBYTE(@data)[9]  AND 3;
      fm_data.WAVEFORM_modulator  := pBYTE(@data)[10] AND 3;
    end;

  temp_instrument.panning := 0;
  temp_instrument.fine_tune := pBYTE(@data)[11] SHR 4;

  _dbg_leave; //EXIT //import_hsc_instrument_alt
end;
