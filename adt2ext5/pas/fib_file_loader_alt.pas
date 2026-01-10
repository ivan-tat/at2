// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext5.pas"

procedure fib_file_loader_alt(instr: Word);

const
  id = 'FIB'+#244;

var
  f: File;
  ident: array[1..4] of Char;
  header: tFIB_HEADER;
  temp: Longint;
  instrument_data: tFIN_DATA;

begin
  _dbg_enter ({$I %FILE%}, 'fib_file_loader_alt');

  {$i-}
  Assign(f,instdata_source);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //fib_file_loader_alt
    end;

  BlockReadF(f,header,SizeOf(header),temp);
  If (temp <> SizeOf(header)) or
     (header.ident <> id) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //fib_file_loader_alt
    end;

  SeekF(f,SizeOf(header)+header.nmins*SizeOf(instrument_data));
  If (IOresult <> 0) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //fib_file_loader_alt
    end;

  BlockReadF(f,ident,SizeOf(ident),temp);
  If (temp <> SizeOf(ident)) or
     (ident <> id) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //fib_file_loader_alt
    end;

  SeekF(f,SizeOf(header)+PRED(instr)*SizeOf(instrument_data));
  If (IOresult <> 0) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //fib_file_loader_alt
    end;

  BlockReadF(f,instrument_data,SizeOf(instrument_data),temp);
  If (temp <> SizeOf(instrument_data)) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //fib_file_loader_alt
    end;

  import_standard_instrument_alt (temp_instrument, instrument_data.idata);
  CloseF(f);
  load_flag_alt := 1;

  _dbg_leave; //EXIT //fib_file_loader_alt
end;
