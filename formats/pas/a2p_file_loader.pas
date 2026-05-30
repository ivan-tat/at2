// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure a2p_file_loader;

type
  tOLD_HEADER = Record
                  ident: array[1..11] of Char;
                  crc32: Longint;
                  ffver: Byte;
                  b0len: Word;
                end;
type
  tHEADER = Record
              ident: array[1..11] of Char;
              crc32: Longint;
              ffver: Byte;
              b0len: Longint;
            end;
type
  tNEW_HEADER = Record
                  ident: array[1..11] of Char;
                  crc32: Longint;
                  ffver: Byte;
                  b0len: Longint;
                end;
const
  id = '_A2pattern_';

var
  f: File;
  header: tOLD_HEADER;
  header2: tHEADER;
  temp: Longint;
  line,chan: Byte;
  temp_str: String;
  crc: Longint;
  _pattern: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'a2p_file_loader');

  If (pattern2use <> BYTE_NULL) then _pattern := pattern2use
  else _pattern := pattern_patt;

  {$i-}
  Assign(f,songdata_source);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2P LOADER ',1);
      _dbg_leave; EXIT; //a2p_file_loader
    end;

  FillChar(buf1,SizeOf(buf1),0);
  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and (header.ident = id)) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2P LOADER ',1);
      _dbg_leave; EXIT; //a2p_file_loader
    end;

  If NOT (header.ffver in [1..FFVER_A2P]) then
    begin
      CloseF(f);
      Dialog('UNKNOWN FiLE FORMAT VERSiON$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2P LOADER ',1);
      _dbg_leave; EXIT; //a2p_file_loader
    end;

  If (play_status <> isStopped) then
    begin
      fade_out_playback(FALSE);
      stop_playing;
    end;

  init_old_songdata;
  If (header.ffver in [1..4]) then
    begin
      FillChar(adsr_carrier,SizeOf(adsr_carrier),BYTE(FALSE));
      crc := DWORD_NULL;
      BlockReadF(f,buf1,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2P LOADER ',1);
          _dbg_leave; EXIT; //a2p_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      crc := Update32(header.b0len,2,crc);

      If (crc <> header.crc32) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2P LOADER ',1);
          _dbg_leave; EXIT; //a2p_file_loader
        end;

      Case header.ffver of
        4: Move(buf1,old_hash_buffer,header.b0len);
        3: LZSS_decompress(buf1,old_hash_buffer,header.b0len);
        2: LZW_decompress(buf1,old_hash_buffer);
        1: SIXPACK_decompress(buf1,old_hash_buffer,header.b0len);
      end;

      If (pattern2use <> BYTE_NULL) and (_patts_marked <> 0) then
        For temp := 0 to PRED(max_patterns) do
          begin
            If (songdata.pattern_names[temp][1] <> ' ') then
              begin
                For line := 0 to $3f do
                  For chan := 1 to 9 do
                    import_old_a2m_event1(temp,line,chan,old_hash_buffer[0][line][chan],FALSE);
                songdata.pattern_names[temp] :=
                  Copy(songdata.pattern_names[temp],1,11)+
                  Lower(NameOnly(songdata_source));
              end;
          end
      else
        begin
          For line := 0 to $3f do
            For chan := 1 to 9 do
              import_old_a2m_event1(_pattern,line,chan,old_hash_buffer[0][line][chan],FALSE);
          songdata.pattern_names[_pattern] :=
            Copy(songdata.pattern_names[_pattern],1,11)+
            Lower(NameOnly(songdata_source));
        end;
    end;

  If (header.ffver in [5..8]) then
    begin
      crc := DWORD_NULL;
      BlockReadF(f,buf1,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2P LOADER ',1);
          _dbg_leave; EXIT; //a2p_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      crc := Update32(header.b0len,2,crc);

      If (crc <> header.crc32) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2P LOADER ',1);
          _dbg_leave; EXIT; //a2p_file_loader
        end;

      Case header.ffver of
        8: Move(buf1,hash_buffer[0],header.b0len);
        7: LZSS_decompress(buf1,hash_buffer[0],header.b0len);
        6: LZW_decompress(buf1,hash_buffer[0]);
        5: SIXPACK_decompress(buf1,hash_buffer[0],header.b0len);
      end;

      If (pattern2use <> BYTE_NULL) and (_patts_marked <> 0) then
        For temp := 0 to PRED(max_patterns) do
          begin
            If (songdata.pattern_names[temp][1] <> ' ') then
              begin
                For line := 0 to $3f do
                   For chan := 1 to 18 do
                     import_old_a2m_event2(temp,line,chan,hash_buffer[0][chan][line]);
                songdata.pattern_names[temp] :=
                  Copy(songdata.pattern_names[temp],1,11)+
                  Lower(NameOnly(songdata_source));
              end;
          end
      else
        begin
          For line := 0 to $3f do
             For chan := 1 to 18 do
               import_old_a2m_event2(_pattern,line,chan,hash_buffer[0][chan][line]);
          songdata.pattern_names[_pattern] :=
            Copy(songdata.pattern_names[_pattern],1,11)+
            Lower(NameOnly(songdata_source));
        end;
    end;

  If (header.ffver = 9) then
    begin
      ResetF(f);
      BlockReadF(f,header2,SizeOf(header2),temp);
      If NOT ((temp = SizeOf(header2)) and (header2.ident = id)) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2P LOADER ',1);
          _dbg_leave; EXIT; //a2p_file_loader
        end;

      crc := DWORD_NULL;
      BlockReadF(f,buf1,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2P LOADER ',1);
          _dbg_leave; EXIT; //a2p_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      crc := Update32(header2.b0len,2,crc);

      If (crc <> header2.crc32) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2P LOADER ',1);
          _dbg_leave; EXIT; //a2p_file_loader
        end;

      If (pattern2use <> BYTE_NULL) and (_patts_marked <> 0) then
        For temp := 0 to PRED(max_patterns) do
          begin
            If (songdata.pattern_names[temp][1] <> ' ') then
              APACK_decompress(buf1,pattdata^[temp DIV 8][temp MOD 8]);
            songdata.pattern_names[temp] :=
              Copy(songdata.pattern_names[temp],1,11)+
              Lower(NameOnly(songdata_source));
          end
      else begin
             APACK_decompress(buf1,pattdata^[_pattern DIV 8][_pattern MOD 8]);
             songdata.pattern_names[_pattern] :=
               Copy(songdata.pattern_names[_pattern],1,11)+
               Lower(NameOnly(songdata_source));
           end;
    end;

  If (header.ffver = 10) then
    begin
      ResetF(f);
      BlockReadF(f,header2,SizeOf(header2),temp);
      If NOT ((temp = SizeOf(header2)) and (header2.ident = id)) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2P LOADER ',1);
          _dbg_leave; EXIT; //a2p_file_loader
        end;

      crc := DWORD_NULL;
      BlockReadF(f,buf1,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2P LOADER ',1);
          _dbg_leave; EXIT; //a2p_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      crc := Update32(header2.b0len,2,crc);

      If (crc <> header2.crc32) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2P LOADER ',1);
          _dbg_leave; EXIT; //a2p_file_loader
        end;

      If (pattern2use <> BYTE_NULL) and (_patts_marked <> 0) then
        For temp := 0 to PRED(max_patterns) do
          begin
            If (songdata.pattern_names[temp][1] <> ' ') then
              begin
                APACK_decompress(buf1,buf2);
                Move(buf2,pattdata^[temp DIV 8][temp MOD 8],PATTERN_SIZE);
                Move(buf2[PATTERN_SIZE],temp_str,buf2[PATTERN_SIZE]+1);

                If (temp_str <> '') then
                  songdata.pattern_names[temp] :=
                    Copy(songdata.pattern_names[temp],1,11)+
                    temp_str
                else
                  songdata.pattern_names[temp] :=
                    Copy(songdata.pattern_names[temp],1,11)+
                    Lower(NameOnly(songdata_source));
              end;
          end
      else begin
             APACK_decompress(buf1,buf2);
             Move(buf2,pattdata^[_pattern DIV 8][_pattern MOD 8],PATTERN_SIZE);
             Move(buf2[PATTERN_SIZE],temp_str,buf2[PATTERN_SIZE]+1);

             If (temp_str <> '') then
               songdata.pattern_names[_pattern] :=
                 Copy(songdata.pattern_names[_pattern],1,11)+
                 temp_str
             else
               songdata.pattern_names[_pattern] :=
                 Copy(songdata.pattern_names[_pattern],1,11)+
                 Lower(NameOnly(songdata_source));
           end;
    end;

  If (header.ffver = FFVER_A2P) then
    begin
      ResetF(f);
      BlockReadF(f,header2,SizeOf(header2),temp);
      If NOT ((temp = SizeOf(header2)) and (header2.ident = id)) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2P LOADER ',1);
          _dbg_leave; EXIT; //a2p_file_loader
        end;

      crc := DWORD_NULL;
      BlockReadF(f,buf1,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2P LOADER ',1);
          _dbg_leave; EXIT; //a2p_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      crc := Update32(header2.b0len,2,crc);

      If (crc <> header2.crc32) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2P LOADER ',1);
          _dbg_leave; EXIT; //a2p_file_loader
        end;

      progress_num_steps := 0;
      If (pattern2use <> BYTE_NULL) and (_patts_marked <> 0) then
        For temp := 0 to PRED(max_patterns) do
          begin
            If (songdata.pattern_names[temp][1] <> ' ') then
              begin
                LZH_decompress(buf1,buf2,header2.b0len);
                Move(buf2,pattdata^[temp DIV 8][temp MOD 8],PATTERN_SIZE);
                Move(buf2[PATTERN_SIZE],temp_str,buf2[PATTERN_SIZE]+1);

                If (temp_str <> '') then
                  songdata.pattern_names[temp] :=
                    Copy(songdata.pattern_names[temp],1,11)+
                    temp_str
                else
                  songdata.pattern_names[temp] :=
                    Copy(songdata.pattern_names[temp],1,11)+
                    Lower(NameOnly(songdata_source));
              end;
          end
      else begin
             LZH_decompress(buf1,buf2,header2.b0len);
             Move(buf2,pattdata^[_pattern DIV 8][_pattern MOD 8],PATTERN_SIZE);
             Move(buf2[PATTERN_SIZE],temp_str,buf2[PATTERN_SIZE]+1);

             If (temp_str <> '') then
               songdata.pattern_names[_pattern] :=
                 Copy(songdata.pattern_names[_pattern],1,11)+
                 temp_str
             else
               songdata.pattern_names[_pattern] :=
                 Copy(songdata.pattern_names[_pattern],1,11)+
                 Lower(NameOnly(songdata_source));
           end;
    end;

  CloseF(f);
  load_flag := 1;

  _dbg_leave; //EXIT //a2p_file_loader
end;
