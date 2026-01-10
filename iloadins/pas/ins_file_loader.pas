// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext3.pas"

procedure ins_file_loader;

type
  tINS_DATA = Record
                idata: tFM_INST_DATA;
                slide: Byte;
                _SAdT: array[0..18] of Byte;
              end;
var
  f: File;
  buffer: tINS_DATA;
  temp: Longint;

function correct_ins(var data): Boolean;

var
  result: Boolean;

begin
  result := TRUE;
  If NOT (tADTRACK2_INS(data).fm_data.WAVEFORM_modulator in [0..3]) then
    result := FALSE;
  If NOT (tADTRACK2_INS(data).fm_data.WAVEFORM_carrier in [0..3]) then
    result := FALSE;
  If NOT (tADTRACK2_INS(data).fm_data.FEEDBACK_FM in [0..15]) then
    result := FALSE;
  correct_ins := result;
end;

begin { ins_file_loader }
  _dbg_enter ({$I %FILE%}, 'ins_file_loader');

  {$i-}
  Assign(f,instdata_source);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' iNS LOADER ',1);
      _dbg_leave; EXIT; //ins_file_loader
    end;

  If (FileSize(f) > SizeOf(buffer)) then
    begin
      CloseF(f);
      Dialog('UNKNOWN FiLE FORMAT TYPE$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' iNS LOADER ',1);
      _dbg_leave; EXIT; //ins_file_loader
    end;

  BlockReadF(f,buffer,FileSize(f),temp);
  If (temp <> FileSize(f)) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' iNS LOADER ',1);
      _dbg_leave; EXIT; //ins_file_loader
    end;

  Case force_ins of
    0: begin
         If (temp = 12) then
           import_standard_instrument(current_inst,buffer.idata);
         If (temp = 12) and NOT correct_ins(buffer.idata) then
           import_hsc_instrument(current_inst,buffer.idata)
         else If (temp > 12) then
                import_sat_instrument(current_inst,buffer.idata);
       end;

    1: import_hsc_instrument(current_inst,buffer.idata);
    2: import_sat_instrument(current_inst,buffer.idata);
    3: import_standard_instrument(current_inst,buffer.idata);
  end;

  songdata.instr_names[current_inst] :=
    Copy(songdata.instr_names[current_inst],1,9)+
    Lower(NameOnly(instdata_source));

  CloseF(f);
  load_flag := 1;

  _dbg_leave; //EXIT //ins_file_loader
end;
