// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure a2m_file_loader;

type
  tOLD_HEADER = Record
                  ident: array[1..10] of Char;
                  crc32: Longint;
                  ffver: Byte;
                  patts: Byte;
                  b0len: Word;
                  b1len: Word;
                  b2len: Word;
                  b3len: Word;
                  b4len: Word;
                  b5len: Word;
                  b6len: Word;
                  b7len: Word;
                  b8len: Word;
                end;
type
  tHEADER = Record
              ident: array[1..10] of Char;
              crc32: Longint;
              ffver: Byte;
              patts: Byte;
              b0len: Longint;
              b1len: array[0..15] of Longint;
            end;

const
  id = '_A2module_';

const
  old_a2m_header_size = 26;

var
  f: File;
  header: tHEADER;
  header2: tOLD_HEADER;
  temp,temp2: Longint;
  crc: Longint;
  xlen: array[0..6] of Word;

begin
  _dbg_enter ({$I %FILE%}, 'a2m_file_loader');

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
             '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //a2m_file_loader
    end;

  FillChar(buf1,SizeOf(buf1),0);
  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and (header.ident = id)) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //a2m_file_loader
    end;

{$IFDEF ADT2PLAY}
  load_flag := $7f;
{$ENDIF}
  If NOT (header.ffver in [1..FFVER_A2M]) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('UNKNOWN FiLE FORMAT VERSiON$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //a2m_file_loader
    end;

{$IFNDEF ADT2PLAY}
  If (play_status <> isStopped) then
    begin
      fade_out_playback(FALSE);
      stop_playing;
    end;
{$ENDIF}

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
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      xlen[0] := header2.b2len;
      xlen[1] := header2.b3len;
      xlen[2] := header2.b4len;

      SeekF(f,old_a2m_header_size);
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      crc := DWORD_NULL;
      BlockReadF(f,buf1,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header2.b1len,temp);
      If NOT (temp = header2.b1len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
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
                       '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2m_file_loader
              end;
            crc := Update32(buf1,temp,crc);
          end;

      crc := Update32(header2.b0len,2,crc);
      crc := Update32(header2.b1len,2,crc);

      For temp2 := 0 to 2 do
        crc := Update32(xlen[temp2],2,crc);

      If (crc <> header2.crc32) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      init_songdata;
      load_flag := 0;

      songdata.patt_len := 64;
      If adjust_tracks then songdata.nm_tracks := 9
      else If (songdata.nm_tracks < 9) then songdata.nm_tracks := 9;

      SeekF(f,old_a2m_header_size);
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      BlockReadF(f,buf1,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      Case header2.ffver of
        4: Move(buf1,old_songdata,header2.b0len);
        3: LZSS_decompress(buf1,old_songdata,header2.b0len);
        2: LZW_decompress(buf1,old_songdata);
        1: SIXPACK_decompress(buf1,old_songdata,header2.b0len);
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
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      Case header2.ffver of
        4: Move(buf1,old_hash_buffer,header2.b1len);
        3: LZSS_decompress(buf1,old_hash_buffer,header2.b1len);
        2: LZW_decompress(buf1,old_hash_buffer);
        1: SIXPACK_decompress(buf1,old_hash_buffer,header2.b1len);
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
                       '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2m_file_loader
              end;

            Case header2.ffver of
              4: Move(buf1,old_hash_buffer,xlen[temp2]);
              3: LZSS_decompress(buf1,old_hash_buffer,xlen[temp2]);
              2: LZW_decompress(buf1,old_hash_buffer);
              1: SIXPACK_decompress(buf1,old_hash_buffer,xlen[temp2]);
            end;
            import_old_a2m_patterns1(SUCC(temp2),16);
          end;

      replace_old_adsr(header2.patts);
      import_old_songdata(Addr(old_songdata));
    end;

  If (header.ffver in [5..8]) then
    begin
      ResetF(f);
      BlockReadF(f,header2,SizeOf(header2),temp);
      If NOT ((temp = SizeOf(header2)) and (header2.ident = id)) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      xlen[0] := header2.b2len;
      xlen[1] := header2.b3len;
      xlen[2] := header2.b4len;
      xlen[3] := header2.b5len;
      xlen[4] := header2.b6len;
      xlen[5] := header2.b7len;
      xlen[6] := header2.b8len;

      crc := DWORD_NULL;
      BlockReadF(f,buf1,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header2.b1len,temp);
      If NOT (temp = header2.b1len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      For temp2 := 0 to 6 do
        If ((header2.patts-1) DIV 8 > temp2) then
          begin
            BlockReadF(f,buf1,xlen[temp2],temp);
            If NOT (temp = xlen[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2m_file_loader
              end;
            crc := Update32(buf1,temp,crc);
          end;

      crc := Update32(header2.b0len,2,crc);
      crc := Update32(header2.b1len,2,crc);

      For temp2 := 0 to 6 do
        crc := Update32(xlen[temp2],2,crc);

      If (crc <> header2.crc32) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      init_songdata;
      load_flag := 0;

      songdata.patt_len := 64;
      If adjust_tracks then songdata.nm_tracks := 18
      else If (songdata.nm_tracks < 18) then songdata.nm_tracks := 18;

      SeekF(f,SizeOf(header2));
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      BlockReadF(f,buf1,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      Case header2.ffver of
        8: Move(buf1,old_songdata,header2.b0len);
        7: LZSS_decompress(buf1,old_songdata,header2.b0len);
        6: LZW_decompress(buf1,old_songdata);
        5: SIXPACK_decompress(buf1,old_songdata,header2.b0len);
      end;

      BlockReadF(f,buf1,header2.b1len,temp);
      If NOT (temp = header2.b1len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      Case header2.ffver of
        8: Move(buf1,hash_buffer,header2.b1len);
        7: LZSS_decompress(buf1,hash_buffer,header2.b1len);
        6: LZW_decompress(buf1,hash_buffer);
        5: SIXPACK_decompress(buf1,hash_buffer,header2.b1len);
      end;
      import_old_a2m_patterns2(0,8);

      For temp2 := 0 to 6 do
        If ((header2.patts-1) DIV 8 > temp2) then
          begin
            BlockReadF(f,buf1,xlen[temp2],temp);
            If NOT (temp = xlen[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2m_file_loader
              end;

            Case header2.ffver of
              8: Move(buf1,hash_buffer,header2.b2len);
              7: LZSS_decompress(buf1,hash_buffer,header2.b2len);
              6: LZW_decompress(buf1,hash_buffer);
              5: SIXPACK_decompress(buf1,hash_buffer,header2.b2len);
            end;
            import_old_a2m_patterns2(SUCC(temp2),8);
          end;
      import_old_songdata(Addr(old_songdata));
    end;

  If (header.ffver in [9,10,11]) then
    begin
      crc := DWORD_NULL;
      BlockReadF(f,buf1,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header.b1len[0],temp);
      If NOT (temp = header.b1len[0]) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      For temp2 := 1 to 15 do
        If ((header.patts-1) DIV 8 > PRED(temp2)) then
          begin
            BlockReadF(f,buf1,header.b1len[temp2],temp);
            If NOT (temp = header.b1len[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2m_file_loader
              end;
            crc := Update32(buf1,temp,crc);
          end;

      crc := Update32(header.b0len,2,crc);
      For temp2 := 0 to 15 do
        crc := Update32(header.b1len[temp2],2,crc);

      If (crc <> header.crc32) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
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
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      BlockReadF(f,buf1,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      APACK_decompress(buf1,songdata);
      BlockReadF(f,buf1,header.b1len[0],temp);
      If NOT (temp = header.b1len[0]) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      If (header.ffver = 9) then
        import_old_flags;

{$IFNDEF ADT2PLAY}
      For temp := 1 to 255 do
        Insert(songdata.instr_names[temp][1]+
               'iNS_'+byte2hex(temp)+#247' ',
               songdata.instr_names[temp],1);

      If (header.ffver = 11) then
        For temp := 0 to $7f do
          Insert(songdata.pattern_names[temp][1]+
                 'PAT_'+byte2hex(temp)+'  '#247' ',
                 songdata.pattern_names[temp],1);
{$ENDIF}

      APACK_decompress(buf1,pattdata^[0]);
      For temp2 := 1 to 15 do
        If ((header.patts-1) DIV 8 > PRED(temp2)) then
          begin
            BlockReadF(f,buf1,header.b1len[temp2],temp);
            If NOT (temp = header.b1len[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2m_file_loader
              end;

            If (temp2*8+8 <= max_patterns) then
              APACK_decompress(buf1,pattdata^[temp2])
            else limit_exceeded := TRUE;
          end;
    end;

  If (header.ffver in [12,13,FFVER_A2M]) then
    begin
      crc := DWORD_NULL;
      BlockReadF(f,buf1,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header.b1len[0],temp);
      If NOT (temp = header.b1len[0]) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      For temp2 := 1 to 15 do
        If ((header.patts-1) DIV 8 > PRED(temp2)) then
          begin
            BlockReadF(f,buf1,header.b1len[temp2],temp);
            If NOT (temp = header.b1len[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2m_file_loader
              end;
            crc := Update32(buf1,temp,crc);
          end;

      crc := Update32(header.b0len,2,crc);
      For temp2 := 0 to 15 do
        crc := Update32(header.b1len[temp2],2,crc);

      If (crc <> header.crc32) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
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
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

      BlockReadF(f,buf1,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

{$IFNDEF ADT2PLAY}
      progress_num_steps := (header.patts-1) DIV 8 +2;
{$ENDIF}
      LZH_decompress(buf1,songdata,header.b0len);
{$IFNDEF ADT2PLAY}
      Inc(progress_step);
{$ENDIF}
      BlockReadF(f,buf1,header.b1len[0],temp);
      If NOT (temp = header.b1len[0]) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //a2m_file_loader
        end;

{$IFNDEF ADT2PLAY}
      For temp := 1 to 255 do
        Insert(songdata.instr_names[temp][1]+
               'iNS_'+byte2hex(temp)+#247' ',
               songdata.instr_names[temp],1);

      For temp := 0 to $7f do
        Insert(songdata.pattern_names[temp][1]+
               'PAT_'+byte2hex(temp)+'  '#247' ',
               songdata.pattern_names[temp],1);
{$ENDIF}

      LZH_decompress(buf1,pattdata^[0],header.b1len[0]);
{$IFNDEF ADT2PLAY}
      Inc(progress_step);
{$ENDIF}
      For temp2 := 1 to 15 do
        If ((header.patts-1) DIV 8 > PRED(temp2)) then
          begin
            BlockReadF(f,buf1,header.b1len[temp2],temp);
            If NOT (temp = header.b1len[temp2]) then
              begin
                CloseF(f);
{$IFNDEF ADT2PLAY}
                Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                       'LOADiNG STOPPED$',
                       '~O~KAY$',' A2M LOADER ',1);
{$ENDIF}
                _dbg_leave; EXIT; //a2m_file_loader
              end;

            If (temp2*8+8 <= max_patterns) then
              begin
                LZH_decompress(buf1,pattdata^[temp2],header.b1len[temp2]);
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
    1..4: load_flag := 1;
    else  load_flag := 2;
  end;
{$ELSE}
  temp := calc_max_speedup(songdata.tempo);
  If (songdata.macro_speedup > temp) then
     begin
       Dialog('DUE TO SYSTEM LiMiTATiONS, ~MACRO SPEEDUP~ VALUE IS ~CHANGED~$'+
              'SLOWDOWN: ~'+Num2str(songdata.macro_speedup,10)+'X -> '+Num2str(temp,10)+'X~$',
              '~O~KAY$',' A2M LOADER ',1);
       songdata.macro_speedup := temp;
     end;

  load_flag := 1;
{$ENDIF}

  _dbg_leave; //EXIT //a2m_file_loader
end;
