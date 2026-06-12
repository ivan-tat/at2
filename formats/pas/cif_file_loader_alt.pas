// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure cif_file_loader_alt(fname: String);

const
  id = '<CUD-FM-Instrument>'+#26;

var
  f: File;
  buffer: tCIF_DATA;
  temp: Longint;

const
  MIN_CIF_SIZE = SizeOf(buffer.ident)+
                 SizeOf(buffer.idata)+
                 SizeOf(buffer.resrv);
begin
  _dbg_enter ({$I %FILE%}, 'cif_file_loader_alt');

  FillChar(temp_instrument,SizeOf(temp_instrument),0);
  {$i-}
  Assign(f,fname);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //cif_file_loader_alt
    end;

  BlockReadF(f,buffer,SizeOf(buffer),temp);
  If NOT ((temp >= MIN_CIF_SIZE) and (buffer.ident = id)) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //cif_file_loader_alt
    end;

  import_hsc_instrument_alt(buffer.idata);
  temp_instrument.fine_tune := 0;
  CloseF(f);
  load_flag_alt := 1;

  _dbg_leave; //EXIT //cif_file_loader_alt
end;
