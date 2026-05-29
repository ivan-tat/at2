// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function _a2b_saver: Byte;

const
  id = '_A2insbank_';

type
  tHEADER = Record
              ident: array[1..11] of Char;
              crc32: Longint;
              ffver: Byte;
              b0len: Longint;
            end;
var
  f: File;
  header: tHEADER;
  temp,temp2,temp3: Longint;
  crc: Longint;
  temp_marks: array[1..255] of Char;

begin
  _dbg_enter ({$I %FILE%}, '_a2b_saver');

  _a2b_saver := 0;
  {$i-}
  Assign(f,instdata_source);
  ResetF(f);
  {$i+}
  If (IOresult = 0) and NOT quick_cmd then
    begin
      If (dl_environment.keystroke = kESC) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //_a2b_saver
        end;
        temp := Dialog('FiLE "'+iCASE(NameOnly(instdata_source))+
                       '" ALREADY EXiSTS iN DESTiNATiON DiRECTORY$',
                       '~O~VERWRiTE$~R~ENAME$~C~ANCEL$',' A2B SAVER ',1);

      If ((dl_environment.keystroke <> kESC) and (temp = 3)) or
          (dl_environment.keystroke = kESC) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //_a2b_saver
        end
      else If (dl_environment.keystroke <> kESC) and (temp = 2) then
             begin
               CloseF(f);
               _a2b_saver := BYTE_NULL;
               _dbg_leave; EXIT; //_a2b_saver
             end;
    end
  else If (IOresult <> 0) then
         begin
           CloseF(f);
           Dialog('ERROR WRiTiNG DATA - DiSK ERROR?$'+
                  'SAViNG STOPPED$',
                  '~O~KAY$',' A2B SAVER ',1);
           _dbg_leave; EXIT; //_a2b_saver
         end;
  {$i-}
  RewriteF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK ERROR?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2B SAVER ',1);
      _dbg_leave; EXIT; //_a2b_saver
    end;

  progress_num_steps := 0;
  header.ident := id;
  header.ffver := FFVER_A2B;

  BlockWriteF(f,header,SizeOf(header),temp);
  If NOT (temp = SizeOf(header)) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2B SAVER ',1);
      _dbg_leave; EXIT; //_a2b_saver
    end;

  For temp := 1 to 255 do
    begin
      temp_marks[temp] := songdata.instr_names[temp][1];
      Delete(songdata.instr_names[temp],1,9);
      FillChar(songdata.instr_names[temp][SUCC(Length(songdata.instr_names[temp]))],
               32-Length(songdata.instr_names[temp]),0);
    end;

  temp3 := SizeOf(songdata.instr_names)+SizeOf(songdata.instr_data);
  Move(songdata.instr_names,buf2,temp3);
  Move(songdata.ins_4op_flags,buf2[temp3],SizeOf(songdata.ins_4op_flags));
  Inc(temp3,SizeOf(songdata.ins_4op_flags));
  temp2 := LZH_compress(buf2,buf1,temp3);

  For temp := 1 to 255 do
    Insert(temp_marks[temp]+
           'iNS_'+byte2hex(temp)+#247' ',
           songdata.instr_names[temp],1);

  BlockWriteF(f,buf1,temp2,temp);
  If NOT (temp = temp2) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2B SAVER ',1);
      _dbg_leave; EXIT; //_a2b_saver
    end;

  header.b0len := temp;
  crc := DWORD_NULL;
  crc := Update32(header.b0len,2,crc);
  crc := Update32(buf1,header.b0len,crc);
  header.crc32 := crc;
  ResetF_RW(f);

  BlockWriteF(f,header,SizeOf(header),temp);
  If NOT (temp = SizeOf(header)) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2B SAVER ',1);
      _dbg_leave; EXIT; //_a2b_saver
    end;

  CloseF(f);

  _dbg_leave; //EXIT //_a2b_saver
end;
