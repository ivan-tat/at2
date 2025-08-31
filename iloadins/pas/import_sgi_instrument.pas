// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure import_sgi_instrument(inst: Byte; var data);
begin
  _dbg_enter ({$I %FILE%}, 'sgi_file_loader_alt.import_sgi_instrument');

  FillChar(temp_instrument,SizeOf(temp_instrument),0);
  With temp_instrument do
    begin
      fm_data.ATTCK_DEC_modulator := (pBYTE(@data)[1]  AND $0f)+
                                     (pBYTE(@data)[0]  AND $0f) SHL 4;
      fm_data.SUSTN_REL_modulator := (pBYTE(@data)[3]  AND $0f)+
                                     (pBYTE(@data)[2]  AND $0f) SHL 4;
      fm_data.WAVEFORM_modulator  := (pBYTE(@data)[4]  AND 3);
      fm_data.KSL_VOLUM_modulator := (pBYTE(@data)[7]  AND $3f)+
                                     (pBYTE(@data)[6]  AND 3) SHL 6;
      fm_data.AM_VIB_EG_modulator := (pBYTE(@data)[5]  AND $0f)+
                                     (pBYTE(@data)[8]  AND 1) SHL 4+
                                     (pBYTE(@data)[11] AND 1) SHL 5+
                                     (pBYTE(@data)[10] AND 1) SHL 6+
                                     (pBYTE(@data)[9]  AND 1) SHL 7;
      fm_data.ATTCK_DEC_carrier   := (pBYTE(@data)[13] AND $0f)+
                                     (pBYTE(@data)[12] AND $0f) SHL 4;
      fm_data.SUSTN_REL_carrier   := (pBYTE(@data)[15] AND $0f)+
                                     (pBYTE(@data)[14] AND $0f) SHL 4;
      fm_data.WAVEFORM_carrier    := (pBYTE(@data)[16] AND 3);
      fm_data.KSL_VOLUM_carrier   := (pBYTE(@data)[19] AND $3f)+
                                     (pBYTE(@data)[18] AND 3) SHL 6;
      fm_data.AM_VIB_EG_carrier   := (pBYTE(@data)[17] AND $0f)+
                                     (pBYTE(@data)[20] AND 1) SHL 4+
                                     (pBYTE(@data)[23] AND 1) SHL 5+
                                     (pBYTE(@data)[22] AND 1) SHL 6+
                                     (pBYTE(@data)[21] AND 1) SHL 7;
      fm_data.FEEDBACK_FM         := (pBYTE(@data)[25] AND 1)+
                                     (pBYTE(@data)[24] AND 7) SHL 1;
    end;

  _dbg_leave; //EXIT //sgi_file_loader_alt.import_sgi_instrument
end;
