// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext3.pas"

procedure sgi_file_loader;

type
  tSGI_DATA = Record
           { 0} attack_m,
           { 1} decay_m,
           { 2} sustain_m,
           { 3} release_m,
           { 4} waveform_m,
           { 5} mfmult_m,
           { 6} ksl_m,
           { 7} volume_m,
           { 8} ksr_m,
           { 9} tremolo_m,
           {10} vibrato_m,
           {11} eg_type_m,
           {12} attack_c,
           {13} decay_c,
           {14} sustain_c,
           {15} release_c,
           {16} waveform_c,
           {17} mfmult_c,
           {18} ksl_c,
           {19} volume_c,
           {20} ksr_c,
           {21} tremolo_c,
           {22} vibrato_c,
           {23} eg_type_c,
           {24} feedback,
           {25} fm:        Byte;
              end;

var
  f: File;
  buffer: tSGI_DATA;
  temp: Longint;
  temp_str: String;

begin
  _dbg_enter ({$I %FILE%}, 'sgi_file_loader');

  {$i-}
  Assign(f,instdata_source);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' SGi LOADER ',1);
      _dbg_leave; EXIT; //sgi_file_loader
    end;

  BlockReadF(f,buffer,SizeOf(buffer),temp);
  If (temp <> SizeOf(buffer)) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' SGi LOADER ',1);
      _dbg_leave; EXIT; //sgi_file_loader
    end;

  FillChar (songdata.instr_data[current_inst], SizeOf (songdata.instr_data[current_inst]), 0);
  import_sgi_instrument (songdata.instr_data[current_inst], buffer);
  temp_str := Lower(NameOnly(instdata_source));
  songdata.instr_names[current_inst] :=
    Copy(songdata.instr_names[current_inst],1,9)+Copy(temp_str,1,32);

  CloseF(f);
  load_flag := 1;

  _dbg_leave; //EXIT //sgi_file_loader
end;
