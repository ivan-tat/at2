// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure ins_file_loader_alt(fname: String);

var
  f: File;
  buffer: tINS_DATA;
  temp: Longint;

function correct_ins(var data): Boolean;

var
  result: Boolean;

begin
  _dbg_enter ({$I %FILE%}, 'ins_file_loader_alt.correct_ins');

  result := TRUE;
  If NOT (tADTRACK2_INS(data).fm_data.WAVEFORM_modulator in [0..3]) then
    result := FALSE;
  If NOT (tADTRACK2_INS(data).fm_data.WAVEFORM_carrier in [0..3]) then
    result := FALSE;
  If NOT (tADTRACK2_INS(data).fm_data.FEEDBACK_FM in [0..15]) then
    result := FALSE;
  correct_ins := result;

  _dbg_leave; //EXIT //ins_file_loader_alt.correct_ins
end;

begin { ins_file_loader_alt }
  _dbg_enter ({$I %FILE%}, 'ins_file_loader_alt');

  FillChar(temp_instrument,SizeOf(temp_instrument),0);
  {$i-}
  Assign(f,fname);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //ins_file_loader_alt
    end;

  If (FileSize(f) > SizeOf(buffer)) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //ins_file_loader_alt
    end;

  BlockReadF(f,buffer,FileSize(f),temp);
  If (temp <> FileSize(f)) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //ins_file_loader_alt
    end;

  Case force_ins of
    0: begin
         If (temp = 12) then
           import_standard_instrument_alt(buffer.idata);
         If (temp = 12) and NOT correct_ins(buffer.idata) then
           import_hsc_instrument_alt(buffer.idata)
         else If (temp > 12) then
                import_sat_instrument_alt(buffer.idata);
       end;

    1: import_hsc_instrument_alt(buffer.idata);
    2: import_sat_instrument_alt(buffer.idata);
    3: import_standard_instrument_alt(buffer.idata);
  end;

  CloseF(f);
  load_flag_alt := 1;

  _dbg_leave; //EXIT //ins_file_loader_alt
end;
