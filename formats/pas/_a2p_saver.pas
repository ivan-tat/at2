// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function _a2p_saver: Byte;

type
  tHEADER = Record
              ident: array[1..11] of Char;
              crc32: Longint;
              ffver: Byte;
              b0len: Longint;
            end;
const
  id = '_A2pattern_';

var
  f: File;
  header: tHEADER;
  temp,temp2: Longint;
  temp_str: String;

begin
  _dbg_enter ({$I %FILE%}, '_a2p_saver');

  _a2p_saver := 0;
  {$i-}
  Assign(f,songdata_source);
  ResetF(f);
  {$i+}
  If (IOresult = 0) and NOT quick_cmd then
    begin
      If (dl_environment.keystroke = kESC) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //_a2p_saver
        end;
        temp := Dialog('FiLE "'+iCASE(NameOnly(songdata_source))+
                       '" ALREADY EXiSTS iN DESTiNATiON DiRECTORY$',
                       '~O~VERWRiTE$~R~ENAME$~C~ANCEL$',' A2P SAVER ',1);

      If ((dl_environment.keystroke <> kESC) and (temp = 3)) or
          (dl_environment.keystroke = kESC) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //_a2p_saver
        end
      else If (dl_environment.keystroke <> kESC) and (temp = 2) then
             begin
               CloseF(f);
               _a2p_saver := BYTE_NULL;
               _dbg_leave; EXIT; //_a2p_saver
             end;
    end
  else If (IOresult <> 0) then
         begin
           CloseF(f);
           Dialog('ERROR WRiTiNG DATA - DiSK ERROR?$'+
                  'SAViNG STOPPED$',
                  '~O~KAY$',' A2P SAVER ',1);
           _dbg_leave; EXIT; //_a2p_saver
         end;
  {$i-}
  RewriteF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK ERROR?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2P SAVER ',1);
      _dbg_leave; EXIT; //_a2p_saver
    end;

  progress_num_steps := 0;
  header.crc32 := DWORD_NULL;
  header.ident := id;
  header.ffver := FFVER_A2P;

  If (pattern2use <> BYTE_NULL) then
    Move(pattdata^[pattern2use DIV 8][pattern2use MOD 8],
         buf2,
         PATTERN_SIZE)
  else
    Move(pattdata^[pattern_patt DIV 8][pattern_patt MOD 8],
         buf2,
         PATTERN_SIZE);

  If (pattern2use <> BYTE_NULL) then
    temp_str := Copy(songdata.pattern_names[pattern2use],12,30)
  else temp_str := Copy(songdata.pattern_names[pattern_patt],12,30);

  FillChar(temp_str[SUCC(Length(temp_str))],30-Length(temp_str),0);
  Move(temp_str,buf2[PATTERN_SIZE],Length(temp_str)+1);

  BlockWriteF(f,header,SizeOf(header),temp);
  If NOT (temp = SizeOf(header)) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2P SAVER ',1);
      _dbg_leave; EXIT; //_a2p_saver
    end;

  temp2 := PATTERN_SIZE+30+1;
  header.b0len := LZH_compress(buf2,buf1,temp2);

  BlockWriteF(f,buf1,header.b0len,temp);
  If NOT (temp = header.b0len) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2P SAVER ',1);
      _dbg_leave; EXIT; //_a2p_saver
    end;

  header.crc32 := Update32(buf1,header.b0len,header.crc32);
  header.crc32 := Update32(header.b0len,2,header.crc32);

  ResetF_RW(f);
  BlockWriteF(f,header,SizeOf(header),temp);
  If NOT (temp = SizeOf(header)) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2P SAVER ',1);
      _dbg_leave; EXIT; //_a2p_saver
    end;

  CloseF(f);

  _dbg_leave; //EXIT //_a2p_saver
end;
