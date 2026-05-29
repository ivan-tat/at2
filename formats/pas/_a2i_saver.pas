// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function _a2i_saver: Byte;

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
  header: tHEADER;
  temp,temp2,temp3: Longint;
  ins_4op: Word;
  crc: Word;
  temp_str: String;

begin
  _dbg_enter ({$I %FILE%}, '_a2i_saver');

  _a2i_saver := 0;
  {$i-}
  Assign(f,instdata_source);
  ResetF(f);
  {$i+}
  If (IOresult = 0) and NOT quick_cmd then
    begin
      If (dl_environment.keystroke = kESC) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //_a2i_saver
        end;
        temp := Dialog('FiLE "'+iCASE(NameOnly(instdata_source))+
                       '" ALREADY EXiSTS iN DESTiNATiON DiRECTORY$',
                       '~O~VERWRiTE$~R~ENAME$~C~ANCEL$',' A2i SAVER ',1);

      If ((dl_environment.keystroke <> kESC) and (temp = 3)) or
          (dl_environment.keystroke = kESC) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //_a2i_saver
        end
      else If (dl_environment.keystroke <> kESC) and (temp = 2) then
             begin
               CloseF(f);
               _a2i_saver := BYTE_NULL;
               _dbg_leave; EXIT; //_a2i_saver
             end;
    end
  else If (IOresult <> 0) then
         begin
           CloseF(f);
           Dialog('ERROR WRiTiNG DATA - DiSK ERROR?$'+
                  'SAViNG STOPPED$',
                  '~O~KAY$',' A2i SAVER ',1);
           _dbg_leave; EXIT; //_a2i_saver
         end;
  {$i-}
  RewriteF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK ERROR?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2i SAVER ',1);
      _dbg_leave; EXIT; //_a2i_saver
    end;

  progress_num_steps := 0;
  header.ident := id;
  header.ffver := FFVER_A2I;

  BlockWriteF(f,header,SizeOf(header),temp);
  If NOT (temp = SizeOf(header)) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2i SAVER ',1);
      _dbg_leave; EXIT; //_a2i_saver
    end;

  temp3 := 0;
  ins_4op := check_4op_to_test;
  If (ins_4op <> 0) then
    begin
      // 4OP 1/2
      Move(songdata.instr_data[HI(ins_4op)],buf2[temp3],
           SizeOf(songdata.instr_data[HI(ins_4op)]));
      Inc(temp3,SizeOf(songdata.instr_data[HI(ins_4op)]));
      temp_str := Copy(songdata.instr_names[HI(ins_4op)],10,32);
      Move(temp_str,buf2[temp3],SUCC(Length(temp_str)));
      Inc(temp3,SUCC(Length(temp_str)));
      // 4OP 2/2
      Move(songdata.instr_data[LO(ins_4op)],buf2[temp3],
           SizeOf(songdata.instr_data[LO(ins_4op)]));
      Inc(temp3,SizeOf(songdata.instr_data[LO(ins_4op)]));
      temp_str := Copy(songdata.instr_names[LO(ins_4op)],10,32);
      Move(temp_str,buf2[temp3],SUCC(Length(temp_str)));
      Inc(temp3,SUCC(Length(temp_str)));
    end
  else begin
         Move(songdata.instr_data[current_inst],buf2[temp3],
              SizeOf(songdata.instr_data[current_inst]));
         Inc(temp3,SizeOf(songdata.instr_data[current_inst]));
         temp_str := Copy(songdata.instr_names[current_inst],10,32);
         Move(temp_str,buf2[temp3],SUCC(Length(temp_str)));
         Inc(temp3,SUCC(Length(temp_str)));
       end;

  temp2 := LZH_compress(buf2,buf3,temp3);
  BlockWriteF(f,buf3,temp2,temp);
  If NOT (temp = temp2) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2i SAVER ',1);
      _dbg_leave; EXIT; //_a2i_saver
    end;

  header.b0len := temp;
  crc := WORD_NULL;
  crc := Update16(header.b0len,1,crc);
  crc := Update16(buf3,header.b0len,crc);
  header.crc16 := crc;
  ResetF_RW(f);

  BlockWriteF(f,header,SizeOf(header),temp);
  If NOT (temp = SizeOf(header)) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2i SAVER ',1);
      _dbg_leave; EXIT; //_a2i_saver
    end;

  CloseF(f);

  _dbg_leave; //EXIT //_a2i_saver
end;
