// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext3.pas"

procedure sbi_file_loader;

const
  id = 'SBI'+#26;

type
  tSBI_DATA = Record
                ident: array[1..4]  of Char;
                iname: array[1..32] of Char;
                idata: tFM_INST_DATA;
                dummy: array[1..5]  of Byte;
              end;
var
  f: File;
  buffer: tSBI_DATA;
  temp: Longint;
  temp_str: String;

begin
  _dbg_enter ({$I %FILE%}, 'sbi_file_loader');

  {$i-}
  Assign(f,instdata_source);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' SBi LOADER ',1);
      _dbg_leave; EXIT; //sbi_file_loader
    end;

  BlockReadF(f,buffer,SizeOf(buffer),temp);
  If NOT ((temp = SizeOf(buffer)) and (buffer.ident = id)) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' SBi LOADER ',1);
      _dbg_leave; EXIT; //sbi_file_loader
    end;

  import_standard_instrument(current_inst,buffer.idata);
  temp_str := truncate_string(buffer.iname);
  If (temp_str = '') then temp_str := Lower(NameOnly(instdata_source));

  songdata.instr_names[current_inst] :=
    Copy(songdata.instr_names[current_inst],1,9)+Copy(temp_str,1,32);

  CloseF(f);
  load_flag := 1;

  _dbg_leave; //EXIT //sbi_file_loader
end;
