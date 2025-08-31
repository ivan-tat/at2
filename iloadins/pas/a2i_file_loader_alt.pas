// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure a2i_file_loader_alt(fname: String);

type
  tOLD_HEADER = Record
                  ident: array[1..7] of Char;
                  crc16: Word;
                  ffver: Byte;
                  b0len: Byte;
                end;
type
  tHEADER = Record
              ident: array[1..7] of Char;
              crc16: Word;
              ffver: Byte;
              b0len: Word;
            end;
const
  id = '_A2ins_';

var
  f: File;
  header: tOLD_HEADER;
  header2: tHEADER;
  temp,temp2: Longint;
  crc: Word;

begin
  _dbg_enter ({$I %FILE%}, 'a2i_file_loader_alt');

  FillChar(temp_instrument,SizeOf(temp_instrument),0);
  {$i-}
  Assign(f,fname);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //a2i_file_loader_alt
    end;

  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and (header.ident = id)) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //a2i_file_loader_alt
    end;

  If NOT (header.ffver in [1..FFVER_A2I]) then
    begin
      CloseF(f);
      _dbg_leave; EXIT; //a2i_file_loader_alt
    end;

  If (header.ffver in [1..4]) then
    begin
      BlockReadF(f,buf2,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //a2i_file_loader_alt
        end;

      crc := WORD_NULL;
      crc := Update16(header.b0len,1,crc);
      crc := Update16(buf2,header.b0len,crc);

      If (crc <> header.crc16) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //a2i_file_loader_alt
        end;

      Case header.ffver of
        4: Move(buf2,buf3,header.b0len);
        3: LZSS_decompress(buf2,buf3,header.b0len);
        2: LZW_decompress(buf2,buf3);
        1: SIXPACK_decompress(buf2,buf3,header.b0len);
      end;

      Move(buf3,temp_instrument,SizeOf(temp_instrument));
      temp_instrument.panning := 0;
    end;

  If (header.ffver in [5..8]) then
    begin
      BlockReadF(f,buf2,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //a2i_file_loader_alt
        end;

      crc := WORD_NULL;
      crc := Update16(header.b0len,1,crc);
      crc := Update16(buf2,header.b0len,crc);

      If (crc <> header.crc16) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //a2i_file_loader_alt
        end;

      Case header.ffver of
        8: Move(buf2,buf3,header.b0len);
        7: LZSS_decompress(buf2,buf3,header.b0len);
        6: LZW_decompress(buf2,buf3);
        5: SIXPACK_decompress(buf2,buf3,header.b0len);
      end;

      Move(buf3,temp_instrument,SizeOf(temp_instrument));
    end;

  If (header.ffver = 9) then
    begin
      ResetF(f);
      BlockReadF(f,header2,SizeOf(header2),temp);
      If NOT ((temp = SizeOf(header2)) and (header2.ident = id)) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //a2i_file_loader_alt
        end;

      BlockReadF(f,buf2,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //a2i_file_loader_alt
        end;

      crc := WORD_NULL;
      crc := Update16(header2.b0len,1,crc);
      crc := Update16(buf2,header2.b0len,crc);

      If (crc <> header2.crc16) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //a2i_file_loader_alt
        end;

      APACK_decompress(buf2,buf3);
      Move(buf3,temp_instrument,SizeOf(temp_instrument));
    end;

  If (header.ffver = FFVER_A2I) then
    begin
      ResetF(f);
      BlockReadF(f,header2,SizeOf(header2),temp);
      If NOT ((temp = SizeOf(header2)) and (header2.ident = id)) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //a2i_file_loader_alt
        end;

      BlockReadF(f,buf2,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //a2i_file_loader_alt
        end;

      crc := WORD_NULL;
      crc := Update16(header2.b0len,1,crc);
      crc := Update16(buf2,header2.b0len,crc);

      If (crc <> header2.crc16) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //a2i_file_loader_alt
        end;

      progress_num_steps := 0;
      temp := 0;
      temp2 := LZH_decompress(buf2,buf3,header2.b0len);
      Move(buf3[temp],temp_instrument,SizeOf(temp_instrument));
      Inc(temp,SizeOf(temp_instrument)); // instrument data
      Inc(temp,SUCC(buf3[temp])); // instrument name
      If (temp < temp2) then // more data present => 4op instrument
        begin
          Move(temp_instrument,temp_instrument2,SizeOf(temp_instrument2));
          Move(buf3[temp],temp_instrument,SizeOf(temp_instrument));
          Inc(temp,SizeOf(temp_instrument));
        end;
    end;

  CloseF(f);
  load_flag_alt := 1;

  _dbg_leave; //EXIT //a2i_file_loader_alt
end;
