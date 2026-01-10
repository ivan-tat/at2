// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext3.pas"

procedure fin_file_loader;

var
  f: File;
  buffer: tFIN_DATA;
  temp: Longint;
  temp_str: String;

begin
  _dbg_enter ({$I %FILE%}, 'fin_file_loader');

  {$i-}
  Assign(f,instdata_source);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' FiN LOADER ',1);
      _dbg_leave; EXIT; //fin_file_loader
    end;

  BlockReadF(f,buffer,SizeOf(buffer),temp);
  If (temp <> SizeOf(buffer)) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' FiN LOADER ',1);
      _dbg_leave; EXIT; //fin_file_loader
    end;

  import_fin_instrument(current_inst,buffer.idata);
  If (Length(truncate_string(buffer.iname)) <= 32) then
    temp_str := truncate_string(buffer.iname)
  else temp_str := Lower(truncate_string(buffer.dname));
  If (temp_str = '') then temp_str := Lower(NameOnly(instdata_source));

  songdata.instr_names[current_inst] :=
    Copy(songdata.instr_names[current_inst],1,9)+Copy(temp_str,1,32);

  CloseF(f);
  load_flag := 1;

  _dbg_leave; //EXIT //fin_file_loader
end;
