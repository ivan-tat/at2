// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure ibk_file_loader_alt(instr: Word);

const
  id = 'IBK'+#26;

var
  f: File;
  header: array[1..4] of Char;
  temp: Longint;
  instrument_data: Record
                     idata: tFM_INST_DATA;
                     dummy: array[1..5] of Byte;
                   end;

begin
  _dbg_enter ({$I %FILE%}, 'ibk_file_loader_alt');

  {$i-}
  Assign(f,instdata_source);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //ibk_file_loader_alt
    end;

  BlockReadF(f,header,SizeOf(header),temp);
  If (temp <> SizeOf(header)) or
     (header <> id) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //ibk_file_loader_alt
    end;

  SeekF(f,$004+PRED(instr)*SizeOf(instrument_data));
  If (IOresult <> 0) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //ibk_file_loader_alt
    end;

  BlockReadF(f,instrument_data,SizeOf(instrument_data),temp);
  If (temp <> SizeOf(instrument_data)) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //ibk_file_loader_alt
    end;

  import_sbi_instrument_alt(instrument_data);
  CloseF(f);
  load_flag_alt := 1;

  _dbg_leave; //EXIT //ibk_file_loader_alt
end;
