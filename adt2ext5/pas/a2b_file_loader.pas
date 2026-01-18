// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext5.pas"

procedure a2b_file_loader(bankSelector: Boolean; loadBankPossible: Boolean);

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
const
  id = '_A2insbank_';

var
  f: File;
  header: tOLD_HEADER;
  header2: tHEADER;
  crc,temp: Longint;
  old_external_proc: procedure; cdecl;
  old_topic_len: Byte;
  old_cycle_moves: Boolean;
  idx,index,nm_valid: Byte;
  temp_str: String;
  ysize: Byte;

const
  new_keys: array[1..3] of Word = (kESC,kENTER,kCtENTR);

var
  old_keys: array[1..3] of Word;

begin
  _dbg_enter ({$I %FILE%}, 'a2b_file_loader');

  progress_num_steps := 0;
  progress_step := 0;
 {$i-}
  Assign(f,instdata_source);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2B LOADER ',1);
      _dbg_leave; EXIT; //a2b_file_loader
    end;

  temp_songdata := songdata;
  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and (header.ident = id)) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2B LOADER ',1);
      _dbg_leave; EXIT; //a2b_file_loader
    end;

  If NOT (header.ffver in [1..FFVER_A2B]) then
    begin
      CloseF(f);
      Dialog('UNKNOWN FiLE FORMAT VERSiON$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2B LOADER ',1);
      _dbg_leave; EXIT; //a2b_file_loader
    end;

  init_old_songdata;
  If (header.ffver in [1..4]) then
    begin
      FillChar(buf1,SizeOf(buf1),0);
      BlockReadF(f,buf1,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2B LOADER ',1);
          _dbg_leave; EXIT; //a2b_file_loader
        end;

      crc := DWORD_NULL;
      crc := Update32(header.b0len,2,crc);
      crc := Update32(buf1,header.b0len,crc);

      If (crc <> header.crc32) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2B LOADER ',1);
          _dbg_leave; EXIT; //a2b_file_loader
        end;

      Case header.ffver of
        4: Move(buf1,old_songdata.instr_names,header.b0len);
        3: LZSS_decompress(buf1,old_songdata.instr_names,header.b0len);
        2: LZW_decompress(buf1,old_songdata.instr_names);
        1: SIXPACK_decompress(buf1,old_songdata.instr_names,header.b0len);
      end;

      For temp := 1 to 250 do
        old_songdata.instr_data[temp].panning := 0;
      import_old_instruments(Addr(old_songdata),Addr(temp_songdata),1,250);
    end;

  If (header.ffver in [5..8]) then
    begin
      FillChar(buf1,SizeOf(buf1),0);
      BlockReadF(f,buf1,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2B LOADER ',1);
          _dbg_leave; EXIT; //a2b_file_loader
        end;

      crc := DWORD_NULL;
      crc := Update32(header.b0len,2,crc);
      crc := Update32(buf1,header.b0len,crc);

      If (crc <> header.crc32) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2B LOADER ',1);
          _dbg_leave; EXIT; //a2b_file_loader
        end;

      Case header.ffver of
        8: Move(buf1,old_songdata.instr_names,header.b0len);
        7: LZSS_decompress(buf1,old_songdata.instr_names,header.b0len);
        6: LZW_decompress(buf1,old_songdata.instr_names);
        5: SIXPACK_decompress(buf1,old_songdata.instr_names,header.b0len);
      end;
      import_old_instruments(Addr(old_songdata),Addr(temp_songdata),1,250);
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
                 '~O~KAY$',' A2B LOADER ',1);
          _dbg_leave; EXIT; //a2b_file_loader
        end;

      FillChar(buf1,SizeOf(buf1),0);
      BlockReadF(f,buf1,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2B LOADER ',1);
          _dbg_leave; EXIT; //a2b_file_loader
        end;

      crc := DWORD_NULL;
      crc := Update32(header2.b0len,2,crc);
      crc := Update32(buf1,header2.b0len,crc);

      If (crc <> header2.crc32) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2B LOADER ',1);
          _dbg_leave; EXIT; //a2b_file_loader
        end;

      For temp := 1 to 255 do
        temp_marks[temp] := temp_songdata.instr_names[temp][1];

      APACK_decompress(buf1,temp_songdata.instr_names);
      For temp := 1 to 255 do
        Insert(temp_marks[temp]+
               'iNS_'+byte2hex(temp)+#247' ',
               temp_songdata.instr_names[temp],1);
    end;

  If (header.ffver = FFVER_A2B) then
    begin
      ResetF(f);
      BlockReadF(f,header2,SizeOf(header2),temp);
      If NOT ((temp = SizeOf(header2)) and (header2.ident = id)) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2B LOADER ',1);
          _dbg_leave; EXIT; //a2b_file_loader
        end;

      FillChar(buf1,SizeOf(buf1),0);
      BlockReadF(f,buf1,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2B LOADER ',1);
          _dbg_leave; EXIT; //a2b_file_loader
        end;

      crc := DWORD_NULL;
      crc := Update32(header2.b0len,2,crc);
      crc := Update32(buf1,header2.b0len,crc);

      If (crc <> header2.crc32) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2B LOADER ',1);
          _dbg_leave; EXIT; //a2b_file_loader
        end;

      For temp := 1 to 255 do
        temp_marks[temp] := temp_songdata.instr_names[temp][1];

      progress_num_steps := 0;
      LZH_decompress(buf1,buf2,header2.b0len);
      Move(buf2,temp_songdata.instr_names,SizeOf(songdata.instr_names)+
                                          SizeOf(songdata.instr_data));
      Move(buf2[SizeOf(songdata.instr_names)+
                SizeOf(songdata.instr_data)],temp_songdata.ins_4op_flags,
                SizeOf(songdata.ins_4op_flags));
      For temp := 1 to 255 do
        Insert(temp_marks[temp]+
               'iNS_'+byte2hex(temp)+#247' ',
               temp_songdata.instr_names[temp],1);
    end;

  FillChar(temp_songdata.dis_fmreg_col,SizeOf(temp_songdata.dis_fmreg_col),FALSE);
  CloseF(f);

  If NOT bankSelector then
    begin
      songdata.instr_names := temp_songdata.instr_names;
      songdata.instr_data := temp_songdata.instr_data;
      load_flag := 1;
      _dbg_leave; EXIT; //a2b_file_loader
    end;

  // init 4OP flags
  FillChar(_4op_flag_column,SizeOf(_4op_flag_column),0);
  For temp := 1 to PRED(255) do
    If check_4op_flag_temp(temp) then
      begin
        If NOT (_4op_flag_column[temp] in _4op_flag_chars) then
          _4op_flag_column[temp] := _4op_flag_chr_beg;
        If NOT (_4op_flag_column[SUCC(temp)] in _4op_flag_chars) then
          _4op_flag_column[SUCC(temp)] := _4op_flag_chr_end;
      end;

  a2b_queue[1] := a2b_header_str[1];
  a2b_queue[2] := a2b_header_str[2];
  a2b_queue[3] := a2b_header_str[3];
  a2b_queue_more[1] := a2b_header_hires_str[1];
  a2b_queue_more[2] := a2b_header_hires_str[2];
  a2b_queue_more[3] := a2b_header_hires_str[3];

  nm_valid := count_instruments;
  If (nm_valid = 0) then nm_valid := 1;

  For idx := 1 to nm_valid do
    begin
      a2b_queue[3+idx] := '~'+ExpStrR(Copy(temp_songdata.instr_names[idx],1,9)+'~'+
                          Copy(temp_songdata.instr_names[idx],10,32),45,' ');
      a2b_queue_more[3+idx] := a2b_queue[3+idx];

      With temp_songdata.instr_data[idx].fm_data do
        begin
          a2b_queue_more[3+idx] := a2b_queue_more[3+idx]+
            byte2hex(AM_VIB_EG_carrier)+
            byte2hex(AM_VIB_EG_modulator)+' '+
            byte2hex(KSL_VOLUM_carrier)+
            byte2hex(KSL_VOLUM_modulator)+' '+
            byte2hex(ATTCK_DEC_carrier)+
            byte2hex(ATTCK_DEC_modulator)+' '+
            byte2hex(SUSTN_REL_carrier)+
            byte2hex(SUSTN_REL_modulator)+' '+
            byte2hex(WAVEFORM_carrier)+
            byte2hex(WAVEFORM_modulator)+' '+
            byte2hex(FEEDBACK_FM)+'   ';
        end;

      Case temp_songdata.instr_data[idx].panning of
        0..2: temp_str := ins_pan_str2[temp_songdata.instr_data[idx].panning];
        else  temp_str := ExpStrL('',7,#250);
      end;

      a2b_queue[3+idx] := a2b_queue[3+idx]+temp_str+'   ';
          a2b_queue_more[3+idx] := a2b_queue_more[3+idx]+temp_str+'   ';
      If (temp_songdata.instr_data[idx].fine_tune > 0) then
        temp_str := '+'+ExpStrR(Num2str(temp_songdata.instr_data[idx].fine_tune,16),5,' ')
      else If (temp_songdata.instr_data[idx].fine_tune < 0) then
             temp_str := '-'+ExpStrR(Num2str(0-temp_songdata.instr_data[idx].fine_tune,16),5,' ')
           else temp_str := ExpStrR('',6,' ');

      a2b_queue[3+idx] := a2b_queue[3+idx]+temp_str+'   ';
      a2b_queue_more[3+idx] := a2b_queue_more[3+idx]+temp_str+'   ';

      Case temp_songdata.instr_data[idx].perc_voice of
        0..5: temp_str := perc_voice_str[temp_songdata.instr_data[idx].perc_voice];
        else  temp_str := ExpStrL('',7,' ');
      end;

      a2b_queue[3+idx] := a2b_queue[3+idx]+temp_str;
      a2b_queue_more[3+idx] := a2b_queue_more[3+idx]+temp_str;
    end;

  Move(mn_setting.terminate_keys,old_keys,SizeOf(old_keys));
  old_external_proc := mn_environment.ext_proc;
  old_topic_len := mn_setting.topic_len;
  old_cycle_moves := mn_setting.cycle_moves;

  Move(new_keys,mn_setting.terminate_keys,SizeOf(new_keys));
  mn_environment.ext_proc := a2b_lister_external_proc;
  mn_setting.topic_len := 3;
  mn_setting.cycle_moves := FALSE;

  If loadBankPossible then
    mn_environment.context := ' ~[~'+Num2str(nm_valid,10)+'~/255]~ ^ENTER '#196#16' LOAD COMPLETE BANK '
  else mn_environment.context := '~[~'+Num2str(nm_valid,10)+'~/255]~';

  keyboard_reset_buffer;
  If is_default_screen_mode then ysize := 20
  else ysize := 30;
  _a2b_lister_count := ysize-3;

  If NOT _force_program_quit then
    If (program_screen_mode in [0,3,4,5]) then
      index := Menu(a2b_queue,01,01,min(1,get_bank_position(instdata_source,nm_valid)),
                    74,ysize,nm_valid+3,' '+iCASE(NameOnly(instdata_source))+' ')
    else index := Menu(a2b_queue_more,01,01,min(1,get_bank_position(instdata_source,nm_valid)),
                       104,30,nm_valid+3,' '+iCASE(NameOnly(instdata_source))+' ');

  add_bank_position(instdata_source,nm_valid,index+3);
  Move(old_keys,mn_setting.terminate_keys,SizeOf(old_keys));
  mn_environment.ext_proc := old_external_proc;
  mn_setting.topic_len := old_topic_len;
  mn_setting.cycle_moves := old_cycle_moves;

  If (mn_environment.keystroke = kENTER) or
     (loadBankPossible and (mn_environment.keystroke = kCtENTR)) then
    begin
      If (mn_environment.keystroke = kENTER) then
        begin
          If shift_pressed then
            begin
              // put 4op instrument (alternate)
              _4op_ins_flag := FALSE;
              If (_4op_flag_column[index] = _4op_flag_chr_beg) then
                If check_4op_flag(current_inst) then
                  begin
                    _4op_ins_flag := TRUE;
                    _4op_idx11 := current_inst;
                    _4op_idx12 := SUCC(current_inst);
                    _4op_idx21 := index;
                    _4op_idx22 := SUCC(index);
                  end
                else If check_4op_flag(PRED(current_inst)) then
                       begin
                         _4op_ins_flag := TRUE;
                         _4op_idx11 := PRED(min(current_inst,2));
                         _4op_idx12 := min(current_inst,2);
                         _4op_idx21 := index;
                         _4op_idx22 := SUCC(index);
                       end
                     else
              else If (_4op_flag_column[index] = _4op_flag_chr_end) then
                     If check_4op_flag(current_inst) then
                       begin
                         _4op_ins_flag := TRUE;
                         _4op_idx11 := current_inst;
                         _4op_idx12 := SUCC(current_inst);
                         _4op_idx21 := PRED(index);
                         _4op_idx22 := index;
                       end
                     else If check_4op_flag(PRED(current_inst)) then
                            begin
                              _4op_ins_flag := TRUE;
                              _4op_idx11 := PRED(min(current_inst,2));
                              _4op_idx12 := min(current_inst,2);
                              _4op_idx21 := PRED(index);
                              _4op_idx22 := index;
                            end;
              If _4op_ins_flag then
                begin
                  songdata.instr_data[_4op_idx11] := temp_songdata.instr_data[_4op_idx21];
                  songdata.instr_names[_4op_idx11] := Copy(songdata.instr_names[_4op_idx11],1,9)+
                                                      Copy(temp_songdata.instr_names[_4op_idx21],10,32);
                  songdata.instr_data[_4op_idx12] := temp_songdata.instr_data[_4op_idx22];
                  songdata.instr_names[_4op_idx12] := Copy(songdata.instr_names[_4op_idx12],1,9)+
                                                      Copy(temp_songdata.instr_names[_4op_idx22],10,32);
                end;
            end
          else begin
                 // put 4op instrument (force)
                 _4op_ins_flag := FALSE;
                 If (_4op_flag_column[index] = _4op_flag_chr_beg) then
                   begin
                     _4op_ins_flag := TRUE;
                     _4op_idx11 := current_inst;
                     _4op_idx12 := SUCC(current_inst);
                     _4op_idx21 := index;
                     _4op_idx22 := SUCC(index);
                   end
                 else If (_4op_flag_column[index] = _4op_flag_chr_end) then
                        begin
                          _4op_ins_flag := TRUE;
                          _4op_idx11 := PRED(min(current_inst,2));
                          _4op_idx12 := min(current_inst,2);
                          _4op_idx21 := PRED(index);
                          _4op_idx22 := index;
                        end;
                 If _4op_ins_flag then
                   begin
                     songdata.instr_data[_4op_idx11] := temp_songdata.instr_data[_4op_idx21];
                     songdata.instr_names[_4op_idx11] := Copy(songdata.instr_names[_4op_idx11],1,9)+
                                                         Copy(temp_songdata.instr_names[_4op_idx21],10,32);
                     songdata.instr_data[_4op_idx12] := temp_songdata.instr_data[_4op_idx22];
                     songdata.instr_names[_4op_idx12] := Copy(songdata.instr_names[_4op_idx12],1,9)+
                                                         Copy(temp_songdata.instr_names[_4op_idx22],10,32);
                     set_4op_flag(_4op_idx11);
                   end;
               end;
          // put 2op instrument
          If NOT _4op_ins_flag then
            begin
              songdata.instr_data[current_inst] := temp_songdata.instr_data[index];
              songdata.instr_names[current_inst] := Copy(songdata.instr_names[current_inst],1,9)+
                                                    Copy(temp_songdata.instr_names[index],10,32);
            end;
        end
      else
        begin
          songdata.instr_data := temp_songdata.instr_data;
          songdata.ins_4op_flags := temp_songdata.ins_4op_flags;
          For idx := 1 to 255 do
            songdata.instr_names[idx] := Copy(songdata.instr_names[idx],1,9)+
                                         Copy(temp_songdata.instr_names[idx],10,32);
        end;
      load_flag := 1;
      load_flag_alt := BYTE_NULL;
    end;
  keyboard_reset_buffer;

  _dbg_leave; //EXIT //a2b_file_loader
end;
