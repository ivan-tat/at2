// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext3.pas"

procedure a2i_file_loader;

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
  ins_4op: Word;
  crc: Word;
  temp_ins,temp_ins2: tADTRACK2_INS;
  temp_str,temp_str2: String;
  _4op_ins_flag: Boolean;
  _4op_ins_idx: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'a2i_file_loader');

  {$i-}
  Assign(f,instdata_source);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2i LOADER ',1);
      _dbg_leave; EXIT; //a2i_file_loader
    end;

  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and (header.ident = id)) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2i LOADER ',1);
      _dbg_leave; EXIT; //a2i_file_loader
    end;

  If NOT (header.ffver in [1..FFVER_A2I]) then
    begin
      CloseF(f);
      Dialog('UNKNOWN FiLE FORMAT VERSiON$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2i LOADER ',1);
      _dbg_leave; EXIT; //a2i_file_loader
    end;

  init_old_songdata;
  If (header.ffver in [1..4]) then
    begin
      BlockReadF(f,buf2,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2i LOADER ',1);
          _dbg_leave; EXIT; //a2i_file_loader
        end;

      crc := WORD_NULL;
      crc := Update16(header.b0len,1,crc);
      crc := Update16(buf2,header.b0len,crc);

      If (crc <> header.crc16) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2i LOADER ',1);
          _dbg_leave; EXIT; //a2i_file_loader
        end;

      Case header.ffver of
        4: Move(buf2,buf3,header.b0len);
        3: LZSS_decompress(buf2,buf3,header.b0len);
        2: LZW_decompress(buf2,buf3);
        1: SIXPACK_decompress(buf2,buf3,header.b0len);
      end;

      Move(buf3,
           old_songdata.instr_data[1],
           SizeOf(old_songdata.instr_data[1]));
      Move(buf3[SizeOf(old_songdata.instr_data[1])],
           temp_str,
           buf3[SizeOf(old_songdata.instr_data[1])]+1);

      old_songdata.instr_data[1].panning := 0;
      If (temp_str = '') then temp_str := Lower(NameOnly(instdata_source));
      old_songdata.instr_names[1] :=
        Copy(old_songdata.instr_names[1],1,9)+truncate_string(temp_str);
      import_single_old_instrument(Addr(old_songdata),current_inst,1);
    end;

  If (header.ffver in [5..8]) then
    begin
      BlockReadF(f,buf2,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2i LOADER ',1);
          _dbg_leave; EXIT; //a2i_file_loader
        end;

      crc := WORD_NULL;
      crc := Update16(header.b0len,1,crc);
      crc := Update16(buf2,header.b0len,crc);

      If (crc <> header.crc16) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2i LOADER ',1);
          _dbg_leave; EXIT; //a2i_file_loader
        end;

      Case header.ffver of
        8: Move(buf2,buf3,header.b0len);
        7: LZSS_decompress(buf2,buf3,header.b0len);
        6: LZW_decompress(buf2,buf3);
        5: SIXPACK_decompress(buf2,buf3,header.b0len);
      end;

      Move(buf3,
           old_songdata.instr_data[1],
           SizeOf(old_songdata.instr_data[1]));
      Move(buf3[SizeOf(old_songdata.instr_data[1])],
           temp_str,
           buf3[SizeOf(old_songdata.instr_data[1])]+1);

      If (temp_str = '') then temp_str := Lower(NameOnly(instdata_source));
      old_songdata.instr_names[1] :=
        Copy(old_songdata.instr_names[1],1,9)+truncate_string(temp_str);
      import_single_old_instrument(Addr(old_songdata),current_inst,1);
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
                 '~O~KAY$',' A2i LOADER ',1);
          _dbg_leave; EXIT; //a2i_file_loader
        end;

      BlockReadF(f,buf2,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2i LOADER ',1);
          _dbg_leave; EXIT; //a2i_file_loader
        end;

      crc := WORD_NULL;
      crc := Update16(header2.b0len,1,crc);
      crc := Update16(buf2,header2.b0len,crc);

      If (crc <> header2.crc16) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2i LOADER ',1);
          _dbg_leave; EXIT; //a2i_file_loader
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
    end;

  If (header.ffver = FFVER_A2I) then
    begin
      ResetF(f);
      BlockReadF(f,header2,SizeOf(header2),temp);
      If NOT ((temp = SizeOf(header2)) and (header2.ident = id)) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2i LOADER ',1);
          _dbg_leave; EXIT; //a2i_file_loader
        end;

      BlockReadF(f,buf2,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2i LOADER ',1);
          _dbg_leave; EXIT; //a2i_file_loader
        end;

      crc := WORD_NULL;
      crc := Update16(header2.b0len,1,crc);
      crc := Update16(buf2,header2.b0len,crc);

      If (crc <> header2.crc16) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2i LOADER ',1);
          _dbg_leave; EXIT; //a2i_file_loader
        end;

      _4op_ins_flag := FALSE;
      progress_num_steps := 0;
      temp := 0;
      temp2 := LZH_decompress(buf2,buf3,header2.b0len);
      Move(buf3[temp],temp_ins,SizeOf(temp_ins));
      Inc(temp,SizeOf(temp_ins)); // instrument data
      Move(buf3[temp],temp_str,SUCC(buf3[temp]));
      Inc(temp,SUCC(buf3[temp])); // instrument name
      If (temp < temp2) then // more data present => 4op instrument
        begin
          _4op_ins_flag := TRUE;
          Move(buf3[temp],temp_ins2,SizeOf(temp_ins2));
          Inc(temp,SizeOf(temp_ins2));
          Move(buf3[temp],temp_str2,SUCC(buf3[temp]));
          Inc(temp,SUCC(buf3[temp]));
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
          If (temp_str = '') then temp_str := Lower(NameOnly(instdata_source))+' [4OP 1/2]';
          songdata.instr_names[_4op_ins_idx] :=
            Copy(songdata.instr_names[_4op_ins_idx],1,9)+truncate_string(temp_str);
          // 4OP 2/2
          Move(temp_ins2,songdata.instr_data[SUCC(_4op_ins_idx)],SizeOf(temp_ins2));
          If (temp_str2 = '') then temp_str2 := Lower(NameOnly(instdata_source))+' [4OP 2/2]';
          songdata.instr_names[SUCC(_4op_ins_idx)] :=
            Copy(songdata.instr_names[SUCC(_4op_ins_idx)],1,9)+truncate_string(temp_str2);
        end;
    end;

  CloseF(f);
  load_flag := 1;

  _dbg_leave; //EXIT //a2i_file_loader
end;
