// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

type
  tFIN_DATA = Record
                dname: array[1..12] of Char;
                iname: array[1..27] of Char;
                idata: tFM_INST_DATA;
              end;

procedure fin_file_loader_alt(fname: String);

var
  f: File;
  buffer: tFIN_DATA;
  temp: Longint;

begin
  _dbg_enter ({$I %FILE%}, 'fin_file_loader_alt');

  FillChar(temp_instrument,SizeOf(temp_instrument),0);
  {$i-}
  Assign(f,fname);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //fin_file_loader_alt
    end;

  BlockReadF(f,buffer,SizeOf(buffer),temp);
  If (temp <> SizeOf(buffer)) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //fin_file_loader_alt
    end;

  import_fin_instrument_alt(buffer.idata);
  CloseF(f);
  load_flag_alt := 1;

  _dbg_leave; //EXIT //fin_file_loader_alt
end;
