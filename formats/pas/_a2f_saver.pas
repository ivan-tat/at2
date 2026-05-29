// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function _a2f_saver: Byte;

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
  temp,temp2,temp3: Longint;
  ins_4op: Word;
  temp_str: String;

begin
  _dbg_enter ({$I %FILE%}, '_a2f_saver');

  _a2f_saver := 0;
  {$i-}
  Assign(f,instdata_source);
  ResetF(f);
  {$i+}
  If (IOresult = 0) and NOT quick_cmd then
    begin
      If (dl_environment.keystroke = kESC) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //_a2f_saver
        end;
        temp := Dialog('FiLE "'+iCASE(NameOnly(instdata_source))+
                       '" ALREADY EXiSTS iN DESTiNATiON DiRECTORY$',
                       '~O~VERWRiTE$~R~ENAME$~C~ANCEL$',' A2F SAVER ',1);

      If ((dl_environment.keystroke <> kESC) and (temp = 3)) or
          (dl_environment.keystroke = kESC) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //_a2f_saver
        end
      else If (dl_environment.keystroke <> kESC) and (temp = 2) then
             begin
               CloseF(f);
               _a2f_saver := BYTE_NULL;
               _dbg_leave; EXIT; //_a2f_saver
             end;
    end
  else If (IOresult <> 0) then
         begin
           CloseF(f);
           Dialog('ERROR WRiTiNG DATA - DiSK ERROR?$'+
                  'SAViNG STOPPED$',
                  '~O~KAY$',' A2F SAVER ',1);
           _dbg_leave; EXIT; //_a2f_saver
         end;
  {$i-}
  RewriteF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK ERROR?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2F SAVER ',1);
      _dbg_leave; EXIT; //_a2f_saver
    end;

  progress_num_steps := 0;
  header.ident := id;
  header.ffver := FFVER_A2F;

  BlockWriteF(f,header,SizeOf(header),temp);
  If NOT (temp = SizeOf(header)) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2F SAVER ',1);
      _dbg_leave; EXIT; //_a2f_saver
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
      temp2 := 0;
      Move(songdata.instr_macros[HI(ins_4op)],buf3[temp2],
           SizeOf(songdata.instr_macros[HI(ins_4op)]));
      Inc(temp2,SizeOf(songdata.instr_macros[HI(ins_4op)]));
      tREGISTER_TABLE(Addr(buf3[temp2])^).arpeggio_table := 0;
      tREGISTER_TABLE(Addr(buf3[temp2])^).vibrato_table := 0;
      Move(songdata.dis_fmreg_col[HI(ins_4op)],
           buf3[temp2],
           SizeOf(songdata.dis_fmreg_col[HI(ins_4op)]));
      Inc(temp2,SizeOf(songdata.dis_fmreg_col[HI(ins_4op)]));
      Move(buf3,buf2[temp3],temp2);
      Inc(temp3,temp2);
      // 4OP 2/2
      Move(songdata.instr_data[LO(ins_4op)],buf2[temp3],
           SizeOf(songdata.instr_data[LO(ins_4op)]));
      Inc(temp3,SizeOf(songdata.instr_data[LO(ins_4op)]));
      temp_str := Copy(songdata.instr_names[LO(ins_4op)],10,32);
      Move(temp_str,buf2[temp3],SUCC(Length(temp_str)));
      Inc(temp3,SUCC(Length(temp_str)));
      temp2 := 0;
      Move(songdata.instr_macros[LO(ins_4op)],buf3[temp2],
           SizeOf(songdata.instr_macros[LO(ins_4op)]));
      Inc(temp2,SizeOf(songdata.instr_macros[LO(ins_4op)]));
      tREGISTER_TABLE(Addr(buf3[temp2])^).arpeggio_table := 0;
      tREGISTER_TABLE(Addr(buf3[temp2])^).vibrato_table := 0;
      Move(songdata.dis_fmreg_col[LO(ins_4op)],
           buf3[temp2],
           SizeOf(songdata.dis_fmreg_col[LO(ins_4op)]));
      Inc(temp2,SizeOf(songdata.dis_fmreg_col[LO(ins_4op)]));
      Move(buf3,buf2[temp3],temp2);
      Inc(temp3,temp2);
    end
  else begin
         Move(songdata.instr_data[current_inst],buf2[temp3],
              SizeOf(songdata.instr_data[current_inst]));
         Inc(temp3,SizeOf(songdata.instr_data[current_inst]));
         temp_str := Copy(songdata.instr_names[current_inst],10,32);
         Move(temp_str,buf2[temp3],SUCC(Length(temp_str)));
         Inc(temp3,SUCC(Length(temp_str)));
         temp2 := 0;
         Move(songdata.instr_macros[current_inst],buf3[temp2],
              SizeOf(songdata.instr_macros[current_inst]));
         Inc(temp2,SizeOf(songdata.instr_macros[current_inst]));
         tREGISTER_TABLE(Addr(buf3[temp2])^).arpeggio_table := 0;
         tREGISTER_TABLE(Addr(buf3[temp2])^).vibrato_table := 0;
         Move(songdata.dis_fmreg_col[current_inst],
              buf3[temp2],
              SizeOf(songdata.dis_fmreg_col[current_inst]));
         Inc(temp2,SizeOf(songdata.dis_fmreg_col[current_inst]));
         Move(buf3,buf2[temp3],temp2);
         Inc(temp3,temp2);
       end;

  temp2 := LZH_compress(buf2,buf3,temp3);
  BlockWriteF(f,buf3,temp2,temp);
  If NOT (temp = temp2) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2F SAVER ',1);
      _dbg_leave; EXIT; //_a2f_saver
    end;

  header.b0len := temp;
  header.crc32 := DWORD_NULL;
  header.crc32 := Update32(header.b0len,1,header.crc32);
  header.crc32 := Update32(buf3,header.b0len,header.crc32);
  ResetF_RW(f);

  BlockWriteF(f,header,SizeOf(header),temp);
  If NOT (temp = SizeOf(header)) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2F SAVER ',1);
      _dbg_leave; EXIT; //_a2f_saver
    end;

  CloseF(f);

  _dbg_leave; //EXIT //_a2f_saver
end;
