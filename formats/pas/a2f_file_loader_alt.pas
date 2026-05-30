// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure a2f_file_loader_alt(fname: String);

type
  tHEADER = Record
              ident: array[1..18] of Char;
              crc32: Longint;
              ffver: Byte;
              b0len: Word;
            end;
const
  id = '_a2ins_w/fm-macro_';

var
  f: File;
  header: tHEADER;
  crc,temp,temp2: Longint;

begin
  _dbg_enter ({$I %FILE%}, 'a2f_file_loader_alt');

  FillChar(temp_instrument,SizeOf(temp_instrument),0);
  FillChar(temp_instrument_macro,SizeOf(temp_instrument_macro),0);
  FillChar(temp_instrument_dis_fmreg_col,SizeOf(temp_instrument_dis_fmreg_col),0);
  {$i-}
  Assign(f,fname);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //a2f_file_loader_alt
    end;

  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and (header.ident = id)) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //a2f_file_loader_alt
    end;

  If NOT (header.ffver in [1..FFVER_A2F]) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //a2f_file_loader_alt
    end;

  If (header.ffver = 1) then
    begin
      BlockReadF(f,buf2,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //a2f_file_loader_alt
        end;

      crc := DWORD_NULL;
      crc := Update32(header.b0len,1,crc);
      crc := Update32(buf2,header.b0len,crc);

      If (crc <> header.crc32) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //a2f_file_loader_alt
        end;

      APACK_decompress(buf2,buf3);
      Move(buf3,temp_instrument,SizeOf(temp_instrument));
      Move(buf3[SizeOf(songdata.instr_data[current_inst])+
                buf3[SizeOf(songdata.instr_data[current_inst])]+1],
           temp_instrument_macro,
           SizeOf(temp_instrument_macro));

      Move(buf3[SizeOf(songdata.instr_data[current_inst])+
                buf3[SizeOf(songdata.instr_data[current_inst])]+1+
                SizeOf(songdata.instr_macros[current_inst])],
           temp_instrument_dis_fmreg_col,
           SizeOf(temp_instrument_dis_fmreg_col));
    end;

  If (header.ffver = FFVER_A2F) then
    begin
      BlockReadF(f,buf2,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //a2f_file_loader_alt
        end;

      crc := DWORD_NULL;
      crc := Update32(header.b0len,1,crc);
      crc := Update32(buf2,header.b0len,crc);

      If (crc <> header.crc32) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //a2f_file_loader_alt
        end;

      progress_num_steps := 0;
      temp := 0;
      temp2 := LZH_decompress(buf2,buf3,header.b0len);
      Move(buf3[temp],temp_instrument,SizeOf(temp_instrument));
      Inc(temp,SizeOf(temp_instrument)); // instrument data
      Inc(temp,SUCC(buf3[temp])); // instrument name
      Move(buf3[temp],temp_instrument_macro,
           SizeOf(temp_instrument_macro));
      Inc(temp,SizeOf(temp_instrument_macro)); // instrument macro
      Move(buf3[temp],temp_instrument_dis_fmreg_col,
           SizeOf(temp_instrument_dis_fmreg_col));
      Inc(temp,SizeOf(temp_instrument_dis_fmreg_col)); // disabled FM-macro columns
      If (temp < temp2) then // more data present => 4op instrument
        begin
          Move(temp_instrument,temp_instrument2,SizeOf(temp_instrument2));
          Move(temp_instrument_macro,temp_instrument_macro2,SizeOf(temp_instrument_macro2));
          Move(temp_instrument_dis_fmreg_col,temp_instrument_dis_fmreg_col2,SizeOf(temp_instrument_dis_fmreg_col2));
          Move(buf3[temp],temp_instrument,SizeOf(temp_instrument));
          Inc(temp,SizeOf(temp_instrument));
          Inc(temp,SUCC(buf3[temp]));
          Move(buf3[temp],temp_instrument_macro,
               SizeOf(temp_instrument_macro));
          Inc(temp,SizeOf(temp_instrument_macro));
          Move(buf3[temp],temp_instrument_dis_fmreg_col,
               SizeOf(temp_instrument_dis_fmreg_col));
        end;
    end;

  CloseF(f);
  load_flag_alt := 1;

  _dbg_leave; //EXIT //a2f_file_loader_alt
end;
