// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext5.pas"

procedure import_instrument_from_data_record;
begin
  _dbg_enter ({$I %FILE%}, 'import_instrument_from_data_record');

  FillChar(temp_instrument,SizeOf(temp_instrument),0);
  With temp_instrument.fm_data do
    begin
      AM_VIB_EG_modulator :=
        data_record.modulator.freq_mult AND $0f+
        data_record.modulator.ksr       AND 1 SHL 4+
        data_record.modulator.sustain   AND 1 SHL 5+
        data_record.modulator.vib       AND 1 SHL 6+
        data_record.modulator.am        AND 1 SHL 7;

      AM_VIB_EG_carrier :=
        data_record.carrier.freq_mult AND $0f+
        data_record.carrier.ksr       AND 1 SHL 4+
        data_record.carrier.sustain   AND 1 SHL 5+
        data_record.carrier.vib       AND 1 SHL 6+
        data_record.carrier.am        AND 1 SHL 7;

      KSL_VOLUM_modulator :=
        data_record.modulator.output AND $3f+
        data_record.modulator.ksl    AND 3 SHL 6;

      KSL_VOLUM_carrier :=
        data_record.carrier.output AND $3f+
        data_record.carrier.ksl    AND 3 SHL 6;

      ATTCK_DEC_modulator :=
        data_record.modulator.decay  AND $0f+
        data_record.modulator.attack AND $0f SHL 4;

      ATTCK_DEC_carrier :=
        data_record.carrier.decay  AND $0f+
        data_record.carrier.attack AND $0f SHL 4;

      SUSTN_REL_modulator :=
        data_record.modulator.release    AND $0f+
        data_record.modulator.sust_level AND $0f SHL 4;

      SUSTN_REL_carrier :=
        data_record.carrier.release    AND $0f+
        data_record.carrier.sust_level AND $0f SHL 4;

      WAVEFORM_modulator :=
        data_record.wform_mod AND 7;

      WAVEFORM_carrier :=
        data_record.wform_car AND 7;

      FEEDBACK_FM :=
        data_record.modulator.fm XOR 1 AND 1+
        data_record.modulator.feedback AND 7 SHL 1;
    end;

  If (data_record.voice_num in [6..10]) then
    temp_instrument.perc_voice := data_record.voice_num-5;

  _dbg_leave; //EXIT //import_instrument_from_data_record
end;
