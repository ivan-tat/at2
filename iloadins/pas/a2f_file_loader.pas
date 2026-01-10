// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext3.pas"

procedure a2f_file_loader;

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
  ins_4op: Word;
  temp_ins,temp_ins2: tADTRACK2_INS;
  temp_str,temp_str2: String;
  temp_macro: tREGISTER_TABLE;
  temp_macro2: tREGISTER_TABLE;
  temp_dis_fmreg_col: tDIS_FMREG_COL;
  temp_dis_fmreg_col2: tDIS_FMREG_COL;
  _4op_ins_flag: Boolean;
  _4op_ins_idx: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'a2f_file_loader');

  {$i-}
  Assign(f,instdata_source);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2F LOADER ',1);
      _dbg_leave; EXIT; //a2f_file_loader
    end;

  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and (header.ident = id)) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2F LOADER ',1);
      _dbg_leave; EXIT; //a2f_file_loader
    end;

  If NOT (header.ffver in [1..FFVER_A2F]) then
    begin
      CloseF(f);
      Dialog('UNKNOWN FiLE FORMAT VERSiON$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2F LOADER ',1);
      _dbg_leave; EXIT; //a2f_file_loader
    end;

  If (header.ffver = 1) then
    begin
      BlockReadF(f,buf2,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2F LOADER ',1);
          _dbg_leave; EXIT; //a2f_file_loader
        end;

      crc := DWORD_NULL;
      crc := Update32(header.b0len,1,crc);
      crc := Update32(buf2,header.b0len,crc);

      If (crc <> header.crc32) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2F LOADER ',1);
          _dbg_leave; EXIT; //a2f_file_loader
        end;

      APACK_decompress(buf2,buf3);
      Move(buf3,
           songdata.instr_data[current_inst],
           SizeOf(songdata.instr_data[current_inst]));
      Move(buf3[SizeOf(songdata.instr_data[current_inst])],
           temp_str,
           buf3[SizeOf(songdata.instr_data[current_inst])]+1);

      If (temp_str = '') then temp_str := Lower(NameOnly(instdata_source));
      songdata.instr_names[current_inst] :=
        Copy(songdata.instr_names[current_inst],1,9)+truncate_string(temp_str);

      Move(buf3[SizeOf(songdata.instr_data[current_inst])+
                buf3[SizeOf(songdata.instr_data[current_inst])]+1],
           songdata.instr_macros[current_inst],
           SizeOf(songdata.instr_macros[current_inst]));

      Move(buf3[SizeOf(songdata.instr_data[current_inst])+
                buf3[SizeOf(songdata.instr_data[current_inst])]+1+
                SizeOf(songdata.instr_macros[current_inst])],
           songdata.dis_fmreg_col[current_inst],
           SizeOf(songdata.dis_fmreg_col[current_inst]));
    end;

  If (header.ffver = FFVER_A2F) then
    begin
      BlockReadF(f,buf2,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2F LOADER ',1);
          _dbg_leave; EXIT; //a2f_file_loader
        end;

      crc := DWORD_NULL;
      crc := Update32(header.b0len,1,crc);
      crc := Update32(buf2,header.b0len,crc);

      If (crc <> header.crc32) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2F LOADER ',1);
          _dbg_leave; EXIT; //a2f_file_loader
        end;

      _4op_ins_flag := FALSE;
      progress_num_steps := 0;
      temp := 0;
      temp2 := LZH_decompress(buf2,buf3,header.b0len);
      Move(buf3[temp],temp_ins,SizeOf(temp_ins));
      Inc(temp,SizeOf(temp_ins)); // instrument data
      Move(buf3[temp],temp_str,SUCC(buf3[temp]));
      Inc(temp,SUCC(buf3[temp])); // instrument name
      Move(buf3[temp],temp_macro,SizeOf(temp_macro));
      Inc(temp,SizeOf(temp_macro)); // FM-macro data
      Move(buf3[temp],temp_dis_fmreg_col,SizeOf(temp_dis_fmreg_col));
      Inc(temp,SizeOf(temp_dis_fmreg_col)); // disabled FM-macro column data
      If (temp < temp2) then // more data present => 4op instrument
        begin
          _4op_ins_flag := TRUE;
          Move(buf3[temp],temp_ins2,SizeOf(temp_ins2));
          Inc(temp,SizeOf(temp_ins2));
          Move(buf3[temp],temp_str2,SUCC(buf3[temp]));
          Inc(temp,SUCC(buf3[temp]));
          Move(buf3[temp],temp_macro2,SizeOf(temp_macro2));
          Inc(temp,SizeOf(temp_macro2));
          Move(buf3[temp],temp_dis_fmreg_col2,SizeOf(temp_dis_fmreg_col2));
        end;

      If NOT _4op_ins_flag then
        begin
          ins_4op := check_4op_to_test;
          If (ins_4op <> 0) then
            begin
              reset_4op_flag(HI(ins_4op));
              update_4op_flag_marks;
            end;
          Move(temp_ins,songdata.instr_data[current_inst],SizeOf(temp_ins));
          Move(temp_macro,songdata.instr_macros[current_inst],SizeOf(temp_macro));
          Move(temp_dis_fmreg_col,songdata.dis_fmreg_col[current_inst],SizeOf(temp_dis_fmreg_col));
          If (temp_str = '') then temp_str := Lower(NameOnly(instdata_source));
          songdata.instr_names[current_inst] :=
            Copy(songdata.instr_names[current_inst],1,9)+truncate_string(temp_str);
        end
      else
        begin
          _4op_ins_idx := current_inst;
          set_4op_flag(current_inst);
          update_4op_flag_marks;
          If (_4op_ins_idx = 255) then Dec(_4op_ins_idx);
          // 4OP 1/2
          Move(temp_ins,songdata.instr_data[_4op_ins_idx],SizeOf(temp_ins));
          Move(temp_macro,songdata.instr_macros[_4op_ins_idx],SizeOf(temp_macro));
          Move(temp_dis_fmreg_col,songdata.dis_fmreg_col[_4op_ins_idx],SizeOf(temp_dis_fmreg_col));
          If (temp_str = '') then temp_str := Lower(NameOnly(instdata_source))+' [4OP 1/2]';
          songdata.instr_names[_4op_ins_idx] :=
            Copy(songdata.instr_names[_4op_ins_idx],1,9)+truncate_string(temp_str);
          // 4OP 2/2
          Move(temp_ins2,songdata.instr_data[SUCC(_4op_ins_idx)],SizeOf(temp_ins2));
          Move(temp_macro2,songdata.instr_macros[SUCC(_4op_ins_idx)],SizeOf(temp_macro2));
          Move(temp_dis_fmreg_col2,songdata.dis_fmreg_col[SUCC(_4op_ins_idx)],SizeOf(temp_dis_fmreg_col2));
          If (temp_str2 = '') then temp_str2 := Lower(NameOnly(instdata_source))+' [4OP 2/2]';
          songdata.instr_names[SUCC(_4op_ins_idx)] :=
            Copy(songdata.instr_names[SUCC(_4op_ins_idx)],1,9)+truncate_string(temp_str2);
        end;
    end;

  CloseF(f);
  load_flag := 1;

  _dbg_leave; //EXIT //a2f_file_loader
end;
