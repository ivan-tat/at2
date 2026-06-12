// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext3.pas"

procedure cif_file_loader;

const
  id = '<CUD-FM-Instrument>'+#26;

type
  tCIF_DATA = Record
                ident: array[1..20] of Char;
                idata: tFM_INST_DATA;
                resrv: Byte;
                iname: array[1..20] of Char;
              end;
var
  f: File;
  buffer: tCIF_DATA;
  temp: Longint;
  temp_str: String;

const
  MIN_CIF_SIZE = SizeOf(buffer.ident)+
                 SizeOf(buffer.idata)+
                 SizeOf(buffer.resrv);
begin
  _dbg_enter ({$I %FILE%}, 'cif_file_loader');

  {$i-}
  Assign(f,instdata_source);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' CiF LOADER ',1);
      _dbg_leave; EXIT; //cif_file_loader
    end;

  BlockReadF(f,buffer,SizeOf(buffer),temp);
  If NOT ((temp >= MIN_CIF_SIZE) and (buffer.ident = id)) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' CiF LOADER ',1);
      _dbg_leave; EXIT; //cif_file_loader
    end;

  import_hsc_instrument_alt (songdata.instr_data[current_inst], buffer.idata);
  songdata.instr_data[current_inst].fine_tune := 0;

  temp_str := truncate_string(buffer.iname);
  If (temp_str = '') then temp_str := Lower(NameOnly(instdata_source));

  songdata.instr_names[current_inst] :=
    Copy(songdata.instr_names[current_inst],1,9)+temp_str;

  CloseF(f);
  load_flag := 1;

  _dbg_leave; //EXIT //cif_file_loader
end;
