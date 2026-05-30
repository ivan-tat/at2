// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure a2t_file_loader;

type
  tOLD_HEADER1 = Record
                   ident: array[1..15] of Char;
                   crc32: Longint;
                   ffver: Byte;
                   patts: Byte;
                   tempo: Byte;
                   speed: Byte;
                   b0len: Word;
                   b1len: Word;
                   b2len: Word;
                   b3len: Word;
                   b4len: Word;
                   b5len: Word;
                 end;
type
  tOLD_HEADER2 = Record
                   ident: array[1..15] of Char;
                   crc32: Longint;
                   ffver: Byte;
                   patts: Byte;
                   tempo: Byte;
                   speed: Byte;
                   cflag: Byte;
                   b0len: Word;
                   b1len: Word;
                   b2len: Word;
                   b3len: Word;
                   b4len: Word;
                   b5len: Word;
                   b6len: Word;
                   b7len: Word;
                   b8len: Word;
                   b9len: Word;
                 end;
type
  tOLD_HEADER3 = Record
                   ident: array[1..15] of Char;
                   crc32: Longint;
                   ffver: Byte;
                   patts: Byte;
                   tempo: Byte;
                   speed: Byte;
                   cflag: Byte;
                   patln: Word;
                   nmtrk: Byte;
                   mcspd: Word;
                   b0len: Longint;
                   b1len: Longint;
                   b2len: Longint;
                   b3len: Longint;
                   b4len: array[0..15] of Longint;
                 end;
type
  tOLD_HEADER4 = Record
                   ident: array[1..15] of Char;
                   crc32: Longint;
                   ffver: Byte;
                   patts: Byte;
                   tempo: Byte;
                   speed: Byte;
                   cflag: Byte;
                   patln: Word;
                   nmtrk: Byte;
                   mcspd: Word;
                   is4op: Byte;
                   locks: array[1..20] of Byte;
                   b0len: Longint;
                   b1len: Longint;
                   b2len: Longint;
                   b3len: Longint;
                   b4len: array[0..15] of Longint;
                 end;
type
  tHEADER = Record
              ident: array[1..15] of Char;
              crc32: Longint;
              ffver: Byte;
              patts: Byte;
              tempo: Byte;
              speed: Byte;
              cflag: Byte;
              patln: Word;
              nmtrk: Byte;
              mcspd: Word;
              is4op: Byte;
              locks: array[1..20] of Byte;
              b0len: Longint;
              b1len: Longint;
              b2len: Longint;
              b3len: Longint;
              b4len: Longint;
              b5len: array[0..15] of Longint;
            end;
const
  id = '_A2tiny_module_';

var
  f: File;
  header: tHEADER;
  header2: tOLD_HEADER1;
  header3: tOLD_HEADER2;
  header4: tOLD_HEADER3;
  header5: tOLD_HEADER4;
  temp,temp2: Longint;
  crc: Longint;
  xlen: array[0..6] of Word;

begin
  _dbg_enter ({$I %FILE%}, 'a2t_file_loader');

  {$i-}
  Assign(f,songdata_source);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //a2t_file_loader
    end;

  FillChar(buf1,SizeOf(buf1),0);
  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and (header.ident = id)) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //a2t_file_loader
    end;

{$IFDEF ADT2PLAY}
  load_flag := $7f;
{$ENDIF}
  If NOT (header.ffver in [1..FFVER_A2T]) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('UNKNOWN FiLE FORMAT VERSiON$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //a2t_file_loader
    end;

  init_old_songdata;
  If (header.ffver in [1..4]) then
    begin
      FillChar(adsr_carrier,SizeOf(adsr_carrier),BYTE(FALSE));
      ResetF(f);
      BlockReadF(f,header2,SizeOf(header2),temp);
      If NOT ((temp = SizeOf(header2)) and (header2.ident = id)) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      xlen[0] := header2.b3len;
      xlen[1] := header2.b4len;
      xlen[2] := header2.b5len;

      crc := DWORD_NULL;
      BlockReadF(f,buf1,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header2.b1len,temp);
      If NOT (temp = header2.b1len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header2.b2len,temp);
      If NOT (temp = header2.b2len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      For temp2 := 0 to 2 do
        If ((header2.patts-1) DIV 16 > temp2) then
          begin
            BlockReadF(f,buf1,xlen[temp2],temp);
            If NOT (temp = xlen[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2t_file_loader
              end;
            crc := Update32(buf1,temp,crc);
          end;

      crc := Update32(header2.b0len,2,crc);
      crc := Update32(header2.b1len,2,crc);
      crc := Update32(header2.b2len,2,crc);

      For temp2 := 0 to 2 do
        crc := Update32(xlen[temp2],2,crc);

      If (crc <> header2.crc32) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      init_songdata;
      load_flag := 0;

      songdata.patt_len := 64;
      If adjust_tracks then songdata.nm_tracks := 9
      else If (songdata.nm_tracks < 9) then songdata.nm_tracks := 9;

      SeekF(f,SizeOf(header2));
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      BlockReadF(f,buf1,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      old_songdata.tempo := header2.tempo;
      old_songdata.speed := header2.speed;

      Case header2.ffver of
        4: Move(buf1,old_songdata.instr_data,header2.b0len);
        3: LZSS_decompress(buf1,old_songdata.instr_data,header2.b0len);
        2: LZW_decompress(buf1,old_songdata.instr_data);
        1: SIXPACK_decompress(buf1,old_songdata.instr_data,header2.b0len);
      end;

      For temp := 1 to 250 do
        old_songdata.instr_data[temp].panning := 0;

      BlockReadF(f,buf1,header2.b1len,temp);
      If NOT (temp = header2.b1len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      Case header2.ffver of
        4: Move(buf1,old_songdata.pattern_order,header2.b1len);
        3: LZSS_decompress(buf1,old_songdata.pattern_order,header2.b1len);
        2: LZW_decompress(buf1,old_songdata.pattern_order);
        1: SIXPACK_decompress(buf1,old_songdata.pattern_order,header2.b1len);
      end;

      BlockReadF(f,buf1,header2.b2len,temp);
      If NOT (temp = header2.b2len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      FillChar(old_hash_buffer,SizeOf(old_hash_buffer),0);
      Case header2.ffver of
        4: Move(buf1,old_hash_buffer,header2.b2len);
        3: LZSS_decompress(buf1,old_hash_buffer,header2.b2len);
        2: LZW_decompress(buf1,old_hash_buffer);
        1: SIXPACK_decompress(buf1,old_hash_buffer,header2.b2len);
      end;
      import_old_a2m_patterns1(0,16);

      For temp2 := 0 to 2 do
        If ((header2.patts-1) DIV 16 > temp2) then
          begin
            BlockReadF(f,buf1,xlen[temp2],temp);
            If NOT (temp = xlen[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2t_file_loader
              end;

            FillChar(old_hash_buffer,SizeOf(old_hash_buffer),0);
            Case header2.ffver of
              4: Move(buf1,old_hash_buffer,header2.b3len);
              3: LZSS_decompress(buf1,old_hash_buffer,header2.b3len);
              2: LZW_decompress(buf1,old_hash_buffer);
              1: SIXPACK_decompress(buf1,old_hash_buffer,header2.b3len);
            end;
            import_old_a2m_patterns1(SUCC(temp2),16);
          end;

      replace_old_adsr(header2.patts);
      import_old_songdata(Addr(old_songdata));
    end;

  If (header.ffver in [5..8]) then
    begin
      ResetF(f);
      BlockReadF(f,header3,SizeOf(header3),temp);
      If NOT ((temp = SizeOf(header3)) and (header3.ident = id)) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      xlen[0] := header3.b3len;
      xlen[1] := header3.b4len;
      xlen[2] := header3.b5len;
      xlen[3] := header3.b6len;
      xlen[4] := header3.b7len;
      xlen[5] := header3.b8len;
      xlen[6] := header3.b9len;

      crc := DWORD_NULL;
      BlockReadF(f,buf1,header3.b0len,temp);
      If NOT (temp = header3.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header3.b1len,temp);
      If NOT (temp = header3.b1len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header3.b2len,temp);
      If NOT (temp = header3.b2len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      For temp2 := 0 to 6 do
        If ((header3.patts-1) DIV 8 > temp2) then
          begin
            BlockReadF(f,buf1,xlen[temp2],temp);
            If NOT (temp = xlen[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2t_file_loader
              end;
            crc := Update32(buf1,temp,crc);
          end;

      crc := Update32(header3.b0len,2,crc);
      crc := Update32(header3.b1len,2,crc);
      crc := Update32(header3.b2len,2,crc);

      For temp2 := 0 to 6 do
        crc := Update32(xlen[temp2],2,crc);

      If (crc <> header3.crc32) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      init_songdata;
      load_flag := 0;

      songdata.patt_len := 64;
      If adjust_tracks then songdata.nm_tracks := 18
      else If (songdata.nm_tracks < 18) then songdata.nm_tracks := 18;

      SeekF(f,SizeOf(header3));
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      BlockReadF(f,buf1,header3.b0len,temp);
      If NOT (temp = header3.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      old_songdata.tempo := header3.tempo;
      old_songdata.speed := header3.speed;
      old_songdata.common_flag := header3.cflag;

      Case header3.ffver of
        8: Move(buf1,old_songdata.instr_data,header3.b0len);
        7: LZSS_decompress(buf1,old_songdata.instr_data,header3.b0len);
        6: LZW_decompress(buf1,old_songdata.instr_data);
        5: SIXPACK_decompress(buf1,old_songdata.instr_data,header3.b0len);
      end;

      BlockReadF(f,buf1,header3.b1len,temp);
      If NOT (temp = header3.b1len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      Case header3.ffver of
        8: Move(buf1,old_songdata.pattern_order,header3.b1len);
        7: LZSS_decompress(buf1,old_songdata.pattern_order,header3.b1len);
        6: LZW_decompress(buf1,old_songdata.pattern_order);
        5: SIXPACK_decompress(buf1,old_songdata.pattern_order,header3.b1len);
      end;

      BlockReadF(f,buf1,header3.b2len,temp);
      If NOT (temp = header3.b2len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      FillChar(hash_buffer,SizeOf(hash_buffer),0);
      Case header3.ffver of
        8: Move(buf1,hash_buffer,header3.b2len);
        7: LZSS_decompress(buf1,hash_buffer,header3.b2len);
        6: LZW_decompress(buf1,hash_buffer);
        5: SIXPACK_decompress(buf1,hash_buffer,header3.b2len);
      end;
      import_old_a2m_patterns2(0,8);

      For temp2 := 0 to 6 do
        If ((header3.patts-1) DIV 8 > temp2) then
          begin
            BlockReadF(f,buf1,xlen[temp2],temp);
            If NOT (temp = xlen[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2t_file_loader
              end;

            FillChar(hash_buffer,SizeOf(hash_buffer),0);
            Case header3.ffver of
              8: Move(buf1,hash_buffer,header3.b3len);
              7: LZSS_decompress(buf1,hash_buffer,header3.b3len);
              6: LZW_decompress(buf1,hash_buffer);
              5: SIXPACK_decompress(buf1,hash_buffer,header3.b3len);
            end;
            import_old_a2m_patterns2(SUCC(temp2),8);
          end;
      import_old_songdata(Addr(old_songdata));
    end;

  If (header.ffver = 9) then
    begin
      ResetF(f);
      BlockReadF(f,header4,SizeOf(header4),temp);
      If NOT ((temp = SizeOf(header4)) and (header4.ident = id)) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := DWORD_NULL;
      BlockReadF(f,buf1,header4.b0len,temp);
      If NOT (temp = header4.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header4.b1len,temp);
      If NOT (temp = header4.b1len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header4.b2len,temp);
      If NOT (temp = header4.b2len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header4.b3len,temp);
      If NOT (temp = header4.b3len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header4.b4len[0],temp);
      If NOT (temp = header4.b4len[0]) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      For temp2 := 1 to 15 do
        If ((header4.patts-1) DIV 8 > PRED(temp2)) then
          begin
            BlockReadF(f,buf1,header4.b4len[temp2],temp);
            If NOT (temp = header4.b4len[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2t_file_loader
              end;
            crc := Update32(buf1,temp,crc);
          end;

      crc := Update32(header4.b0len,2,crc);
      crc := Update32(header4.b1len,2,crc);
      crc := Update32(header4.b2len,2,crc);
      crc := Update32(header4.b3len,2,crc);

      For temp2 := 0 to 15 do
        crc := Update32(header4.b4len[temp2],2,crc);

      If (crc <> header4.crc32) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      init_songdata;
      load_flag := 0;

      SeekF(f,SizeOf(header4));
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      BlockReadF(f,buf1,header4.b0len,temp);
      If NOT (temp = header4.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      APACK_decompress(buf1,songdata.instr_data);
      BlockReadF(f,buf1,header4.b1len,temp);
      If NOT (temp = header4.b1len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      APACK_decompress(buf1,songdata.instr_macros);
      BlockReadF(f,buf1,header4.b2len,temp);
      If NOT (temp = header4.b2len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      APACK_decompress(buf1,songdata.macro_table);
      BlockReadF(f,buf1,header4.b3len,temp);
      If NOT (temp = header4.b3len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      songdata.tempo := header4.tempo;
      songdata.speed := header4.speed;
      songdata.common_flag := header4.cflag;
      songdata.patt_len := header4.patln;
      songdata.nm_tracks := header4.nmtrk;
      songdata.macro_speedup := header4.mcspd;
      import_old_flags;

      APACK_decompress(buf1,songdata.pattern_order);
      BlockReadF(f,buf1,header4.b4len[0],temp);
      If NOT (temp = header4.b4len[0]) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      APACK_decompress(buf1,pattdata^[0]);
      For temp2 := 1 to 15 do
        If ((header4.patts-1) DIV 8 > PRED(temp2)) then
          begin
            BlockReadF(f,buf1,header4.b4len[temp2],temp);
            If NOT (temp = header4.b4len[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2t_file_loader
              end;

            If (temp2*8+8 <= max_patterns) then
              APACK_decompress(buf1,pattdata^[temp2])
            else limit_exceeded := TRUE;
          end;
    end;

  If (header.ffver = 10) then
    begin
      ResetF(f);
      BlockReadF(f,header5,SizeOf(header5),temp);
      If NOT ((temp = SizeOf(header5)) and (header5.ident = id)) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := DWORD_NULL;
      BlockReadF(f,buf1,header5.b0len,temp);
      If NOT (temp = header5.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header5.b1len,temp);
      If NOT (temp = header5.b1len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header5.b2len,temp);
      If NOT (temp = header5.b2len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header5.b3len,temp);
      If NOT (temp = header5.b3len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header5.b4len[0],temp);
      If NOT (temp = header5.b4len[0]) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      For temp2 := 1 to 15 do
        If ((header5.patts-1) DIV 8 > PRED(temp2)) then
          begin
            BlockReadF(f,buf1,header5.b4len[temp2],temp);
            If NOT (temp = header5.b4len[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2t_file_loader
              end;
            crc := Update32(buf1,temp,crc);
          end;

      crc := Update32(header5.b0len,2,crc);
      crc := Update32(header5.b1len,2,crc);
      crc := Update32(header5.b2len,2,crc);
      crc := Update32(header5.b3len,2,crc);

      For temp2 := 0 to 15 do
        crc := Update32(header5.b4len[temp2],2,crc);

      If (crc <> header5.crc32) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      init_songdata;
      load_flag := 0;

      SeekF(f,SizeOf(header5));
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      BlockReadF(f,buf1,header5.b0len,temp);
      If NOT (temp = header5.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      APACK_decompress(buf1,songdata.instr_data);
      BlockReadF(f,buf1,header5.b1len,temp);
      If NOT (temp = header5.b1len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      APACK_decompress(buf1,songdata.instr_macros);
      BlockReadF(f,buf1,header5.b2len,temp);
      If NOT (temp = header5.b2len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      APACK_decompress(buf1,songdata.macro_table);
      BlockReadF(f,buf1,header5.b3len,temp);
      If NOT (temp = header5.b3len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      songdata.tempo := header5.tempo;
      songdata.speed := header5.speed;
      songdata.common_flag := header5.cflag;
      songdata.patt_len := header5.patln;
      songdata.nm_tracks := header5.nmtrk;
      songdata.macro_speedup := header5.mcspd;
      songdata.flag_4op := header5.is4op;
      Move(header5.locks,songdata.lock_flags,SizeOf(songdata.lock_flags));

      APACK_decompress(buf1,songdata.pattern_order);
      BlockReadF(f,buf1,header5.b4len[0],temp);
      If NOT (temp = header5.b4len[0]) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      APACK_decompress(buf1,pattdata^[0]);
      For temp2 := 1 to 15 do
        If ((header5.patts-1) DIV 8 > PRED(temp2)) then
          begin
            BlockReadF(f,buf1,header5.b4len[temp2],temp);
            If NOT (temp = header5.b4len[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2t_file_loader
              end;

            If (temp2*8+8 <= max_patterns) then
              APACK_decompress(buf1,pattdata^[temp2])
            else limit_exceeded := TRUE;
          end;
    end;

  If (header.ffver = 11) then
    begin
      crc := DWORD_NULL;
      BlockReadF(f,buf1,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header.b1len,temp);
      If NOT (temp = header.b1len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header.b2len,temp);
      If NOT (temp = header.b2len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header.b3len,temp);
      If NOT (temp = header.b3len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header.b4len,temp);
      If NOT (temp = header.b4len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header.b5len[0],temp);
      If NOT (temp = header.b5len[0]) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      For temp2 := 1 to 15 do
        If ((header.patts-1) DIV 8 > PRED(temp2)) then
          begin
            BlockReadF(f,buf1,header.b5len[temp2],temp);
            If NOT (temp = header.b5len[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2t_file_loader
              end;
            crc := Update32(buf1,temp,crc);
          end;

      crc := Update32(header.b0len,2,crc);
      crc := Update32(header.b1len,2,crc);
      crc := Update32(header.b2len,2,crc);
      crc := Update32(header.b3len,2,crc);
      crc := Update32(header.b4len,2,crc);

      For temp2 := 0 to 15 do
        crc := Update32(header.b5len[temp2],2,crc);

      If (crc <> header.crc32) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      init_songdata;
      load_flag := 0;

      SeekF(f,SizeOf(header));
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      BlockReadF(f,buf1,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      APACK_decompress(buf1,songdata.instr_data);
      BlockReadF(f,buf1,header.b1len,temp);
      If NOT (temp = header.b1len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      APACK_decompress(buf1,songdata.instr_macros);
      BlockReadF(f,buf1,header.b2len,temp);
      If NOT (temp = header.b2len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      APACK_decompress(buf1,songdata.macro_table);
      BlockReadF(f,buf1,header.b3len,temp);
      If NOT (temp = header.b3len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      APACK_decompress(buf1,songdata.dis_fmreg_col);
      BlockReadF(f,buf1,header.b4len,temp);
      If NOT (temp = header.b4len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      songdata.tempo := header.tempo;
      songdata.speed := header.speed;
      songdata.common_flag := header.cflag;
      songdata.patt_len := header.patln;
      songdata.nm_tracks := header.nmtrk;
      songdata.macro_speedup := header.mcspd;
      songdata.flag_4op := header.is4op;
      Move(header.locks,songdata.lock_flags,SizeOf(songdata.lock_flags));

      APACK_decompress(buf1,songdata.pattern_order);
      BlockReadF(f,buf1,header.b5len[0],temp);
      If NOT (temp = header.b5len[0]) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      APACK_decompress(buf1,pattdata^[0]);
      For temp2 := 1 to 15 do
        If ((header.patts-1) DIV 8 > PRED(temp2)) then
          begin
            BlockReadF(f,buf1,header.b5len[temp2],temp);
            If NOT (temp = header.b5len[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2t_file_loader
              end;

            If (temp2*8+8 <= max_patterns) then
              APACK_decompress(buf1,pattdata^[temp2])
            else limit_exceeded := TRUE;
          end;
    end;

  If (header.ffver in [12,13,FFVER_A2T]) then
    begin
      crc := DWORD_NULL;
      BlockReadF(f,buf1,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header.b1len,temp);
      If NOT (temp = header.b1len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header.b2len,temp);
      If NOT (temp = header.b2len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header.b3len,temp);
      If NOT (temp = header.b3len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header.b4len,temp);
      If NOT (temp = header.b4len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header.b5len[0],temp);
      If NOT (temp = header.b5len[0]) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      For temp2 := 1 to 15 do
        If ((header.patts-1) DIV 8 > PRED(temp2)) then
          begin
            BlockReadF(f,buf1,header.b5len[temp2],temp);
            If NOT (temp = header.b5len[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2t_file_loader
              end;
            crc := Update32(buf1,temp,crc);
          end;

      crc := Update32(header.b0len,2,crc);
      crc := Update32(header.b1len,2,crc);
      crc := Update32(header.b2len,2,crc);
      crc := Update32(header.b3len,2,crc);
      crc := Update32(header.b4len,2,crc);

      For temp2 := 0 to 15 do
        crc := Update32(header.b5len[temp2],2,crc);

      If (crc <> header.crc32) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      init_songdata;
      load_flag := 0;

      SeekF(f,SizeOf(header));
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      BlockReadF(f,buf1,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

{$IFNDEF ADT2PLAY}
      progress_num_steps := (header.patts-1) DIV 8 +6;
{$ENDIF}
      temp := LZH_decompress(buf1,buf2,header.b0len);
      temp2 := 0;
      If (header.ffver = FFVER_A2T) then
        begin
          Move(buf2[temp2],songdata.bpm_data,SizeOf(songdata.bpm_data));
          Inc(temp2,SizeOf(songdata.bpm_data));
        end;
      Move(buf2[temp2],songdata.ins_4op_flags,SizeOf(songdata.ins_4op_flags));
      Inc(temp2,SizeOf(songdata.ins_4op_flags));
      Move(buf2[temp2],songdata.reserved_data,SizeOf(songdata.reserved_data));
      Inc(temp2,SizeOf(songdata.reserved_data));
      Move(buf2[temp2],songdata.instr_data,temp-temp2);
{$IFNDEF ADT2PLAY}
      Inc(progress_step);
{$ENDIF}
      If (header.b1len <> 0) then
        begin
          BlockReadF(f,buf1,header.b1len,temp);
          If NOT (temp = header.b1len) then
            begin
              CloseF(f);
{$IFNDEF ADT2PLAY}
              Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                     'LOADiNG STOPPED$',
                     '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
              _dbg_leave; EXIT; //a2t_file_loader
            end;

          LZH_decompress(buf1,songdata.instr_macros,header.b1len);
        end;

{$IFNDEF ADT2PLAY}
      Inc(progress_step);
{$ENDIF}
      If (header.b2len <> 0) then
        begin
          BlockReadF(f,buf1,header.b2len,temp);
          If NOT (temp = header.b2len) then
            begin
              CloseF(f);
{$IFNDEF ADT2PLAY}
              Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                     'LOADiNG STOPPED$',
                     '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
              _dbg_leave; EXIT; //a2t_file_loader
            end;

          LZH_decompress(buf1,songdata.macro_table,header.b2len);
        end;

{$IFNDEF ADT2PLAY}
      Inc(progress_step);
{$ENDIF}
      If (header.b3len <> 0) then
        begin
          BlockReadF(f,buf1,header.b3len,temp);
          If NOT (temp = header.b3len) then
            begin
              CloseF(f);
{$IFNDEF ADT2PLAY}
              Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                     'LOADiNG STOPPED$',
                     '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
              _dbg_leave; EXIT; //a2t_file_loader
            end;

          LZH_decompress(buf1,songdata.dis_fmreg_col,header.b3len);
        end;

{$IFNDEF ADT2PLAY}
      Inc(progress_step);
{$ENDIF}
      BlockReadF(f,buf1,header.b4len,temp);
      If NOT (temp = header.b4len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      songdata.tempo := header.tempo;
      songdata.speed := header.speed;
      songdata.common_flag := header.cflag;
      songdata.patt_len := header.patln;
      songdata.nm_tracks := header.nmtrk;
      songdata.macro_speedup := header.mcspd;
      songdata.flag_4op := header.is4op;
      Move(header.locks,songdata.lock_flags,SizeOf(songdata.lock_flags));

      LZH_decompress(buf1,songdata.pattern_order,header.b4len);
{$IFNDEF ADT2PLAY}
      Inc(progress_step);
{$ENDIF}
      BlockReadF(f,buf1,header.b5len[0],temp);
      If NOT (temp = header.b5len[0]) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2t_file_loader
        end;

      LZH_decompress(buf1,pattdata^[0],header.b5len[0]);
{$IFNDEF ADT2PLAY}
      Inc(progress_step);
{$ENDIF}
      For temp2 := 1 to 15 do
        If ((header.patts-1) DIV 8 > PRED(temp2)) then
          begin
            BlockReadF(f,buf1,header.b5len[temp2],temp);
            If NOT (temp = header.b5len[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2T LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2t_file_loader
              end;

            If (temp2*8+8 <= max_patterns) then
              begin
                LZH_decompress(buf1,pattdata^[temp2],header.b5len[temp2]);
{$IFNDEF ADT2PLAY}
                Inc(progress_step);
{$ENDIF}
              end
            else limit_exceeded := TRUE;
          end;
    end;

  speed := songdata.speed;
  tempo := songdata.tempo;

  CloseF(f);
  songdata_title := NameOnly(songdata_source);
{$IFDEF ADT2PLAY}
  Case header.ffver of
    1..4: load_flag := 3;
    else  load_flag := 4;
  end;
{$ELSE}
  temp := calc_max_speedup(songdata.tempo);
  If (songdata.macro_speedup > temp) then
     begin
       Dialog('DUE TO SYSTEM LiMiTATiONS, ~MACRO SPEEDUP~ VALUE IS ~CHANGED~$'+
              'SLOWDOWN: ~'+Num2str(songdata.macro_speedup,10)+'X -> '+Num2str(temp,10)+'X~$',
              '~O~KAY$',' A2T LOADER ',1);
       songdata.macro_speedup := temp;
     end;

  load_flag := 1;
{$ENDIF}

  _dbg_leave; //EXIT //a2t_file_loader
end;
