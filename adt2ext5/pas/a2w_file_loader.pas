// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure a2w_file_loader(loadFromFile: Boolean; loadMacros: Boolean; bankSelector: Boolean;
                          loadBankPossible: Boolean; updateCurInstr: Boolean);
type
  tOLD_HEADER = Record
                  ident: array[1..20] of Char;
                  crc32: Longint;
                  ffver: Byte;
                  b0len: Longint;
                  b1len: Longint;
                end;
type
  tHEADER = Record
              ident: array[1..20] of Char;
              crc32: Longint;
              ffver: Byte;
              b0len: Longint;
              b1len: Longint;
              b2len: Longint;
            end;
const
  id = '_A2insbank_w/macros_';

var
  f: File;
  a2w_instdata_source: String;
  header: tHEADER;
  header2: tOLD_HEADER;
  crc,temp: Longint;
  idx,index,nm_valid: Byte;
  idx1,idx2: Integer;
  temp_str: String;
  arpvib_arpeggio_table_bak: Byte;
  arpvib_vibrato_table_bak: Byte;
  browser_flag: Boolean;
  ysize: Byte;

  // backup of Menu settings / variables
  old_external_proc: procedure; cdecl;
  old_topic_len: Byte;
  old_cycle_moves: Boolean;
  old_topic_mask_chr: Set of Char;
  old_frame_enabled: Boolean;
  old_shadow_enabled: Boolean;
  old_winshade: Boolean;
  old_center_box: Boolean;
  old_show_scrollbar: Boolean;
  old_text_attr,
  old_text2_attr,
  old_short_attr,
  old_short2_attr,
  old_disbld_attr,
  old_contxt_attr,
  old_contxt2_attr,
  old_topic_attr,
  old_hi_topic_attr: Byte;

const
  new_keys: array[1..5] of Word = (kESC,kENTER,kF1,kTAB,kCtENTR);

var
  old_keys: array[1..50] of Word;

{$I adt2ext5/pas/_put_2op_instrument.pas}
{$I adt2ext5/pas/_put_4op_instrument.pas}

label _jmp1,_jmp1e,_jmp2,_jmp2e,_end;

begin
  _dbg_enter ({$I %FILE%}, 'a2w_file_loader');

  arpvib_arpeggio_table_bak := arpvib_arpeggio_table;
  arpvib_vibrato_table_bak := arpvib_vibrato_table;
  songdata_bak := songdata;
  temp_songdata := songdata_bak;
  update_current_inst := updateCurInstr;
  browser_flag := FALSE;
  progress_num_steps := 0;
  progress_step := 0;

  If NOT loadFromFile and bankSelector and
     NOT loadBankPossible then
    begin
      a2w_instdata_source := '';
      If loadMacros then
        begin
          arp_tab_selected := _macro_editor__pos[_arp_vib_mode] in [8..13];
          vib_tab_selected := _macro_editor__pos[_arp_vib_mode] in [14..20];
        end
      else
        begin
          arp_tab_selected := songdata.instr_macros[current_inst].arpeggio_table <> 0;
          vib_tab_selected := songdata.instr_macros[current_inst].vibrato_table <> 0;
        end;

        // init 4OP flags (no file bank)
      temp_songdata.ins_4op_flags := songdata.ins_4op_flags;
      FillChar(_4op_flag_column,SizeOf(_4op_flag_column),0);
      For temp := 1 to PRED(255) do
        If check_4op_flag_temp(temp) then
          begin
            If NOT (_4op_flag_column[temp] in _4op_flag_chars) then
              _4op_flag_column[temp] := _4op_flag_chr_beg;
            If NOT (_4op_flag_column[SUCC(temp)] in _4op_flag_chars) then
              _4op_flag_column[SUCC(temp)] := _4op_flag_chr_end;
          end;
      If loadMacros then
        GOTO _jmp1 // Arpeggio/Vibrato Macro Browser
      else GOTO _jmp2; // Instrument Macro Browser
    end
  else a2w_instdata_source := instdata_source;

  {$i-}
  Assign(f,a2w_instdata_source);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2W LOADER ',1);
      _dbg_leave; EXIT; //a2w_file_loader
    end;

  FillChar(buf1,SizeOf(buf1),0);
  BlockReadF(f,header,SizeOf(header),temp);

  If NOT ((temp = SizeOf(header)) and (header.ident = id)) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2W LOADER ',1);
      _dbg_leave; EXIT; //a2w_file_loader
    end;

  If NOT (header.ffver in [1..FFVER_A2W]) then
    begin
      CloseF(f);
      Dialog('UNKNOWN FiLE FORMAT VERSiON$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' A2W LOADER ',1);
      _dbg_leave; EXIT; //a2w_file_loader
    end;

  If (header.ffver = 1) then
    begin
      ResetF(f);
      BlockReadF(f,header2,SizeOf(header2),temp);
      If NOT ((temp = SizeOf(header2)) and (header2.ident = id)) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      crc := DWORD_NULL;
      BlockReadF(f,buf1,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header2.b1len,temp);
      If NOT (temp = header2.b1len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      crc := Update32(header2.b0len,2,crc);
      crc := Update32(header2.b1len,2,crc);

      If (crc <> header2.crc32) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      SeekF(f,SizeOf(header2));
      If (IOresult <> 0) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      BlockReadF(f,buf1,header2.b0len,temp);
      If NOT (temp = header2.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      For temp := 1 to 255 do
        temp_marks[temp] := temp_songdata.instr_names[temp][1];

      APACK_decompress(buf1,temp_songdata.instr_names);
      For temp := 1 to 255 do
        Insert(temp_marks[temp]+
               'iNS_'+byte2hex(temp)+#247' ',
               temp_songdata.instr_names[temp],1);

      BlockReadF(f,buf1,header2.b1len,temp);
      If NOT (temp = header2.b1len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      APACK_decompress(buf1,temp_songdata.macro_table);
      FillChar(temp_songdata.dis_fmreg_col,SizeOf(temp_songdata.dis_fmreg_col),FALSE);
    end;

  If (header.ffver = 2) then
    begin
      crc := DWORD_NULL;
      BlockReadF(f,buf1,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header.b1len,temp);
      If NOT (temp = header.b1len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header.b2len,temp);
      If NOT (temp = header.b2len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      crc := Update32(header.b0len,2,crc);
      crc := Update32(header.b1len,2,crc);
      crc := Update32(header.b2len,2,crc);

      If (crc <> header.crc32) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      SeekF(f,SizeOf(header));
      If (IOresult <> 0) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      BlockReadF(f,buf1,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      For temp := 1 to 255 do
        temp_marks[temp] := temp_songdata.instr_names[temp][1];

      APACK_decompress(buf1,temp_songdata.instr_names);
      For temp := 1 to 255 do
        Insert(temp_marks[temp]+
               'iNS_'+byte2hex(temp)+#247' ',
               temp_songdata.instr_names[temp],1);

      BlockReadF(f,buf1,header.b1len,temp);
      If NOT (temp = header.b1len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      APACK_decompress(buf1,temp_songdata.macro_table);
      BlockReadF(f,buf1,header.b2len,temp);
      If NOT (temp = header.b2len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      APACK_decompress(buf1,temp_songdata.dis_fmreg_col);
    end;

  If (header.ffver = FFVER_A2W) then
    begin
      crc := DWORD_NULL;
      BlockReadF(f,buf1,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header.b1len,temp);
      If NOT (temp = header.b1len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      BlockReadF(f,buf1,header.b2len,temp);
      If NOT (temp = header.b2len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      crc := Update32(buf1,temp,crc);
      crc := Update32(header.b0len,2,crc);
      crc := Update32(header.b1len,2,crc);
      crc := Update32(header.b2len,2,crc);

      If (crc <> header.crc32) then
        begin
          CloseF(f);
          Dialog('CRC FAiLED - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      SeekF(f,SizeOf(header));
      If (IOresult <> 0) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      BlockReadF(f,buf1,header.b0len,temp);
      If NOT (temp = header.b0len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      For temp := 1 to 255 do
        temp_marks[temp] := temp_songdata.instr_names[temp][1];

      progress_num_steps := 0;
      LZH_decompress(buf1,buf2,header.b0len);
      Move(buf2,temp_songdata.instr_names,SizeOf(songdata.instr_names)+
                                          SizeOf(songdata.instr_data)+
                                          SizeOf(songdata.instr_macros));
      Move(buf2[SizeOf(songdata.instr_names)+
                SizeOf(songdata.instr_data)+
                SizeOf(songdata.instr_macros)],temp_songdata.ins_4op_flags,
                SizeOf(songdata.ins_4op_flags));
      For temp := 1 to 255 do
        Insert(temp_marks[temp]+
               'iNS_'+byte2hex(temp)+#247' ',
               temp_songdata.instr_names[temp],1);

      BlockReadF(f,buf1,header.b1len,temp);
      If NOT (temp = header.b1len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      LZH_decompress(buf1,temp_songdata.macro_table,header.b1len);
      BlockReadF(f,buf1,header.b2len,temp);
      If NOT (temp = header.b2len) then
        begin
          CloseF(f);
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' A2W LOADER ',1);
          _dbg_leave; EXIT; //a2w_file_loader
        end;

      LZH_decompress(buf1,temp_songdata.dis_fmreg_col,header.b2len);
    end;

  CloseF(f);

  // init 4OP flags (file bank)
  FillChar(_4op_flag_column,SizeOf(_4op_flag_column),0);
  For temp := 1 to PRED(255) do
    If check_4op_flag_temp(temp) then
      begin
        If NOT (_4op_flag_column[temp] in _4op_flag_chars) then
          _4op_flag_column[temp] := _4op_flag_chr_beg;
        If NOT (_4op_flag_column[SUCC(temp)] in _4op_flag_chars) then
          _4op_flag_column[SUCC(temp)] := _4op_flag_chr_end;
      end;

  If loadMacros then
    begin
_jmp1:
      ScreenMemCopy(screen_ptr,ptr_screen_backup);
      nm_valid := count_macros;
      If (nm_valid = 0) then nm_valid := 1;

      If NOT bankSelector then
        begin
          mn_environment.keystroke := kCtENTR;
          arp_tab_selected := TRUE;
          vib_tab_selected := TRUE;
          GOTO _jmp1e;
        end;

      window_xsize := 73;
      If is_default_screen_mode then window_ysize := max(nm_valid+5,15)+10
      else window_ysize := max(nm_valid+5,20)+10;
      If (a2w_instdata_source <> '') theN temp_str := ' '+iCASE(NameOnly(a2w_instdata_source))+' '#16' A/V MACROS '
      else temp_str := ' ARPEGGiO/ViBRATO MACRO BROWSER ';

      If update_current_inst then temp_str := temp_str + '(iNS_  ) '
      else temp_str := temp_str + '[iNS_  ] ';
      a2w_institle_pos := (window_xsize DIV 2)+(Length(temp_str) DIV 2)-3;

      ScreenMemCopy(screen_ptr,ptr_temp_screen);
      centered_frame_vdest := ptr_temp_screen;
      centered_frame(xstart,ystart,window_xsize,window_ysize,
                     temp_str,
                     macro_background+macro_border,
                     macro_background+macro_title,
                     frame_double);

      ShowStr(centered_frame_vdest,xstart+1,ystart+window_ysize-10+1,
              av_browser_str[6],
              macro_background+macro_topic2);

      ShowStr(centered_frame_vdest,xstart+1,ystart+window_ysize-2,
              ExpStrR('',72,#205),
              macro_background+macro_topic2);

      context_str := ' ~[~'+Num2str(nm_valid,10)+'~/255]~';
      xstart_arp := xstart+3;
      ystart_arp := ystart+window_ysize-10+1;
      xstart_vib := xstart+38;
      ystart_vib := ystart+window_ysize-10+1;
      scrollbar_xstart := xstart+window_xsize;
      scrollbar_ystart := ystart+1;
      scrollbar_size := window_ysize-10+1;
      macro_table_size := nm_valid;

      If (a2w_instdata_source <> '') then temp_str := a2w_instdata_source
      else temp_str := '?internal_instrument_data';

      arpeggio_table_pos := min(1,get_bank_position(temp_str+'?macro_av?arp_pos',nm_valid));
      vibrato_table_pos := min(1,get_bank_position(temp_str+'?macro_av?vib_pos',nm_valid));

      a2w_queue_m[1] := av_browser_str[1];
      a2w_queue_m[2] := av_browser_str[2];
      a2w_queue_m[3] := av_browser_str[3];
      a2w_queue_m[4] := av_browser_str[4];
      a2w_queue_m[5] := av_browser_str[5];

      For idx := 1 to nm_valid do
        begin
          a2w_queue_m[5+idx] := ' ~'+byte2hex(idx)+'~ '#186;
          If Empty(temp_songdata.macro_table[idx].arpeggio,SizeOf(tARPEGGIO_TABLE)) then
            a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '#250#250'  '#179+
                                                     ' '#250#250'  '#179+
                                                     ' '#250#250'  '#179+
                                                     ' '#250#250'  '#179+
                                                     ' '#250#250'   '#186
          else
            With temp_songdata.macro_table[idx].arpeggio do
              begin
                If (length > 0) then
                  a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '+byte2hex(length)+'  '#179
                else a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '#249#249'  '#179;
                If (speed > 0) then
                  a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '+byte2hex(speed)+'  '#179
                else a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '#249#249'  '#179;
                If (loop_begin > 0) then
                  a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '+byte2hex(loop_begin)+'  '#179
                else a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '#249#249'  '#179;
                If (loop_length > 0) then
                  a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '+byte2hex(loop_length)+'  '#179
                else a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '#249#249'  '#179;
                If (keyoff_pos > 0) then
                  a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '+byte2hex(keyoff_pos)+'   '#186
                else a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '#249#249'   '#186;
              end;

          If Empty(temp_songdata.macro_table[idx].vibrato,SizeOf(tViBRATO_TABLE)) then
            a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '#250#250'  '#179+
                                                     ' '#250#250'  '#179+
                                                     ' '#250#250'  '#179+
                                                     ' '#250#250'  '#179+
                                                     ' '#250#250'  '#179+
                                                     ' '#250#250
          else
            With temp_songdata.macro_table[idx].vibrato do
              begin
                If (length > 0) then
                  a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '+byte2hex(length)+'  '#179
                else a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '#249#249'  '#179;
                If (speed > 0) then
                  a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '+byte2hex(speed)+'  '#179
                else a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '#249#249'  '#179;
                If (delay > 0) then
                  a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '+byte2hex(delay)+'  '#179
                else a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '#249#249'  '#179;
                If (loop_begin > 0) then
                  a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '+byte2hex(loop_begin)+'  '#179
                else a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '#249#249'  '#179;
                If (loop_length > 0) then
                  a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '+byte2hex(loop_length)+'  '#179
                else a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '#249#249'  '#179;
                If (keyoff_pos > 0) then
                  a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '+byte2hex(keyoff_pos)+'   '#186
                else a2w_queue_m[5+idx] := a2w_queue_m[5+idx]+' '#249#249;
              end;
        end;

      Move(mn_setting.terminate_keys,old_keys,SizeOf(old_keys));
      old_external_proc := mn_environment.ext_proc;
      old_topic_len := mn_setting.topic_len;
      old_cycle_moves := mn_setting.cycle_moves;
      old_topic_mask_chr := mn_setting.topic_mask_chr;
      old_frame_enabled := mn_setting.frame_enabled;
      old_shadow_enabled := mn_setting.shadow_enabled;
      old_winshade := mn_environment.winshade;
      old_center_box := mn_setting.center_box;
      old_show_scrollbar := mn_setting.show_scrollbar;
      old_text_attr := mn_setting.text_attr;
      old_text2_attr := mn_setting.text2_attr;
      old_short_attr := mn_setting.short_attr;
      old_short2_attr := mn_setting.short2_attr;
      old_disbld_attr := mn_setting.disbld_attr;
      old_contxt_attr := mn_setting.contxt_attr;
      old_contxt2_attr := mn_setting.contxt2_attr;
      old_topic_attr := mn_setting.topic_attr;
      old_hi_topic_attr := mn_setting.hi_topic_attr;

      Move(new_keys,mn_setting.terminate_keys,SizeOf(new_keys));
      mn_setting.terminate_keys[4] := 0; // TAB possible only in FM-Register bank browser
      If NOT loadBankPossible then
        mn_setting.terminate_keys[5] := 0; // ^ENTER possible only in Arpeggio/Vibrato Macro Editor

      mn_environment.ext_proc := a2w_macro_lister_external_proc;
      mn_setting.topic_len := 5;
      mn_setting.cycle_moves := FALSE;
      mn_setting.topic_mask_chr := [#179,#186];
      mn_setting.frame_enabled := FALSE;
      mn_setting.shadow_enabled := FALSE;
      mn_environment.winshade := FALSE;
      mn_setting.center_box := FALSE;
      mn_setting.show_scrollbar := FALSE;
      mn_environment.unpolite := FALSE;
      mn_environment.preview := TRUE;
      mn_environment.v_dest := ptr_temp_screen;
      mn_setting.text_attr := macro_background+macro_item;
      mn_setting.text2_attr := macro_sel_itm_bck+macro_sel_itm;
      mn_setting.short_attr := macro_background+macro_short;
      mn_setting.short2_attr := macro_sel_itm_bck+macro_sel_short;
      mn_setting.disbld_attr := macro_background+macro_item_dis;
      mn_setting.contxt_attr := macro_background+macro_context;
      mn_setting.contxt2_attr := macro_background+macro_context_dis;
      mn_setting.topic_attr := macro_background+macro_topic2;
      mn_setting.hi_topic_attr := macro_background+macro_hi_topic;

      If (a2w_instdata_source <> '') then temp_str := a2w_instdata_source
      else temp_str := '?internal_instrument_data';

      If is_default_screen_mode then ysize := 15
      else ysize := 20;

      If (program_screen_mode in [0,3,4,5]) then
        Menu(a2w_queue_m,xstart,ystart,
             min(1,get_bank_position(temp_str+'?macro_av?pos',nm_valid)),
             72,max(nm_valid+5,ysize),nm_valid+5,'')
      else Menu(a2w_queue_m,xstart,ystart,
                    min(1,get_bank_position(temp_str+'?macro_av?pos',nm_valid)),
                72,max(nm_valid+5,20),nm_valid+5,'');

      move_to_screen_data := ptr_temp_screen;
      move_to_screen_area[1] := xstart;
      move_to_screen_area[2] := ystart;
      move_to_screen_area[3] := xstart+window_xsize+2+1;
      move_to_screen_area[4] := ystart+window_ysize+1;
      move2screen_alt;

      mn_environment.unpolite := FALSE;
      mn_environment.preview := FALSE;
      mn_environment.v_dest := screen_ptr;
      centered_frame_vdest := mn_environment.v_dest;

      If is_default_screen_mode then ysize := 15
      else ysize := 20;

      keyboard_reset_buffer;
      If NOT _force_program_quit then
        If (program_screen_mode in [0,3,4,5]) then
          index := Menu(a2w_queue_m,xstart,ystart,
                        min(1,get_bank_position(temp_str+'?macro_av?pos',nm_valid)),
                        72,max(nm_valid+5,ysize),nm_valid+5,'')
        else index := Menu(a2w_queue_m,xstart,ystart,
                           min(1,get_bank_position(temp_str+'?macro_av?pos',nm_valid)),
                           72,max(nm_valid+5,20),nm_valid+5,'');

      add_bank_position(temp_str+'?macro_av?pos',nm_valid,index+5);
      add_bank_position(temp_str+'?macro_av?arp_pos',nm_valid,arpeggio_table_pos);
      add_bank_position(temp_str+'?macro_av?vib_pos',nm_valid,vibrato_table_pos);

      Move(old_keys,mn_setting.terminate_keys,SizeOf(old_keys));
      mn_environment.ext_proc := old_external_proc;
      mn_setting.topic_len := old_topic_len;
      mn_setting.cycle_moves := old_cycle_moves;
      mn_setting.topic_mask_chr := old_topic_mask_chr;
      mn_setting.frame_enabled := old_frame_enabled;
      mn_setting.shadow_enabled := old_shadow_enabled;
      mn_environment.winshade := old_winshade;
      mn_setting.center_box := old_center_box;
      mn_setting.show_scrollbar := old_show_scrollbar;
      mn_setting.text_attr := old_text_attr;
      mn_setting.text2_attr := old_text2_attr;
      mn_setting.short_attr := old_short_attr;
      mn_setting.short2_attr := old_short2_attr;
      mn_setting.disbld_attr := old_disbld_attr;
      mn_setting.contxt_attr := old_contxt_attr;
      mn_setting.contxt2_attr := old_contxt2_attr;
      mn_setting.topic_attr := old_topic_attr;
      mn_setting.hi_topic_attr := old_hi_topic_attr;

      move_to_screen_data := ptr_screen_backup;
      move_to_screen_area[1] := xstart;
      move_to_screen_area[2] := ystart;
      move_to_screen_area[3] := xstart+window_xsize+2+1;
      move_to_screen_area[4] := ystart+window_ysize+1;
      move2screen;

      If (mn_environment.keystroke = kF1) then
        begin
          HELP('macro_browser_av');
          If NOT _force_program_quit then GOTO _jmp1;
        end;

      If NOT update_current_inst and (mn_environment.keystroke = kENTER) then
            begin
          songdata := songdata_bak;
                  temp_songdata := songdata_bak;
                  mn_environment.keystroke := kESC;
                end;

          If NOT loadMacros and (mn_environment.keystroke = kESC) then
            begin
          load_flag := BYTE_NULL;
          load_flag_alt := BYTE_NULL;
          GOTO _jmp2;
        end;
_jmp1e:
      If (mn_environment.keystroke = kENTER) or
         (loadBankPossible and (mn_environment.keystroke = kCtENTR)) then
        begin
          If (mn_environment.keystroke = kENTER) then
            begin
              If loadMacros or _arp_vib_loader then
                begin
                  arpvib_arpeggio_table := arpvib_arpeggio_table_bak;
                  arpvib_vibrato_table := arpvib_vibrato_table_bak;
                  If arp_tab_selected then
                    songdata.macro_table[arpvib_arpeggio_table].arpeggio := temp_songdata.macro_table[index].arpeggio;
                  If vib_tab_selected then
                    songdata.macro_table[arpvib_vibrato_table].vibrato := temp_songdata.macro_table[index].vibrato;
                end
              else
                begin
                  idx1 := -1;
                  idx2 := -1;
                  If arp_tab_selected then
                    idx1 := get_free_arpeggio_table_idx(temp_songdata.macro_table[index].arpeggio);
                  If vib_tab_selected then
                    idx2 := get_free_vibrato_table_idx(temp_songdata.macro_table[index].vibrato);

                  temp_str := '';
                  If (idx1 = 0) then
                    If (idx2 = 0) then
                      temp_str := '~ARPEGGiO/ViBRATO'
                    else temp_str := '~ARPEGGiO'
                  else If (idx2 = 0) then
                         temp_str := '~ViBRATO';

                  If NOT (temp_str <> '') then
                    begin
                      If (idx1 > 0) then
                        begin
                          songdata.macro_table[idx1].arpeggio := temp_songdata.macro_table[index].arpeggio;
                          songdata.instr_macros[current_inst].arpeggio_table := idx1;
                        end;
                      If (idx2 > 0) then
                        begin
                          songdata.macro_table[idx2].vibrato := temp_songdata.macro_table[index].vibrato;
                          songdata.instr_macros[current_inst].vibrato_table := idx2;
                        end;
                    end
                  else begin
                         Dialog('RELATED '+temp_str+' DATA~ WAS NOT LOADED!$'+
                                'FREE SOME SPACE iN MACRO TABLES AND ~REPEAT THiS ACTiON~$',
                                '~O~K$',' A2W LOADER ',1);
                         GOTO _end;
                       end;
                end;
              load_flag := 1;
              load_flag_alt := BYTE_NULL;
            end
          else
            begin
              temp_str := '';
              If arp_tab_selected then temp_str := 'ARPEGGiO';
              If vib_tab_selected then
                If (temp_str <> '') then temp_str := temp_str+'/ViBRATO'
                else temp_str := 'ViBRATO';
              If NOT (NOT arp_tab_selected and NOT vib_tab_selected) or (nm_valid < 2) then
                begin
                  If bankSelector then
                    index := Dialog('ALL UNSAVED '+temp_str+' MACRO DATA WiLL BE LOST$'+
                                    'DO YOU WiSH TO CONTiNUE?$',
                                    '~Y~UP$~N~OPE$',' A2W LOADER ',1)
                  else begin
                         index := 1;
                         dl_environment.keystroke := kENTER;
                       end;
                  If (dl_environment.keystroke <> kESC) and (index = 1) then
                    begin
                      For idx := 1 to 255 do
                        If NOT (idx > nm_valid) then
                          begin
                            If arp_tab_selected then
                              songdata.macro_table[idx].arpeggio := temp_songdata.macro_table[idx].arpeggio;
                            If vib_tab_selected then
                              songdata.macro_table[idx].vibrato := temp_songdata.macro_table[idx].vibrato;
                          end
                        else begin
                               FillChar(songdata.macro_table[idx].arpeggio,SizeOf(songdata.macro_table[idx].arpeggio),0);
                               FillChar(songdata.macro_table[idx].vibrato,SizeOf(songdata.macro_table[idx].vibrato),0);
                             end;
                      load_flag := 1;
                      load_flag_alt := BYTE_NULL;
                    end;
                end;
            end;
        end;
    end
  else
    begin
_jmp2:
      browser_flag := FALSE;
      ScreenMemCopy(screen_ptr,ptr_screen_backup);

      a2w_queue[1] := a2w_header_str[1];
      a2w_queue[2] := a2w_header_str[2];
      a2w_queue[3] := a2w_header_str[3];
      a2w_queue_more[1] := a2w_header_hires1_str[1];
      a2w_queue_more[2] := a2w_header_hires1_str[2];
      a2w_queue_more[3] := a2w_header_hires1_str[3];
      a2w_queue_more2[1] := a2w_header_hires2_str[1];
      a2w_queue_more2[2] := a2w_header_hires2_str[2];
      a2w_queue_more2[3] := a2w_header_hires2_str[3];

      nm_valid := count_instruments;
      If (nm_valid = 0) then nm_valid := 1;

      If NOT bankSelector then
        begin
          mn_environment.keystroke := kCtENTR;
          GOTO _jmp2e;
        end;

      context_str := '';
      context_str2 := '';
      If (a2w_instdata_source = '') then nm_valid := 255
      else begin
             context_str := ' ~[~'+Num2str(nm_valid,10)+'~/255]~';
             If NOT loadBankPossible then context_str2 := ' ~[BANK]~'
             else context_str2 := ' [BANK]';
           end;

      For idx := 1 to nm_valid do
        begin
          a2w_queue[3+idx] := '~ '+ExpStrR(Copy(temp_songdata.instr_names[idx],2,8)+'~'+
                              Copy(temp_songdata.instr_names[idx],10,32),44,' ');
          a2w_queue_more[3+idx] := a2w_queue[3+idx];

          With temp_songdata.instr_data[idx].fm_data do
            begin
              a2w_queue_more[3+idx] := a2w_queue_more[3+idx]+
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

          a2w_queue_more2[3+idx] := a2w_queue_more[3+idx]+temp_str+'   ';
          If (temp_songdata.instr_data[idx].fine_tune > 0) then
            temp_str := '+'+ExpStrR(Num2str(temp_songdata.instr_data[idx].fine_tune,16),5,' ')
          else If (temp_songdata.instr_data[idx].fine_tune < 0) then
                 temp_str := '-'+ExpStrR(Num2str(0-temp_songdata.instr_data[idx].fine_tune,16),5,' ')
               else temp_str := ExpStrR('',6,' ');

          a2w_queue_more2[3+idx] := a2w_queue_more2[3+idx]+temp_str+'   ';
          Case temp_songdata.instr_data[idx].perc_voice of
            0..5: temp_str := perc_voice_str[temp_songdata.instr_data[idx].perc_voice];
            else  temp_str := ExpStrL('',7,' ');
          end;

          a2w_queue[3+idx] := a2w_queue[3+idx]+temp_str;
          a2w_queue_more[3+idx] := a2w_queue_more[3+idx]+temp_str;
          a2w_queue_more2[3+idx] := a2w_queue_more2[3+idx]+temp_str;

          If (temp_songdata.instr_macros[idx].length <> 0) then temp_str := ' MACRO:FM'
          else temp_str := ' ';

          With temp_songdata.macro_table[
               temp_songdata.instr_macros[idx].arpeggio_table].arpeggio do
            If (temp_songdata.instr_macros[idx].arpeggio_table <> 0) then
              If (temp_str <> ' ') then temp_str := temp_str+'+ARP'
              else temp_str := temp_str+'MACRO:ARP';

          With temp_songdata.macro_table[
               temp_songdata.instr_macros[idx].vibrato_table].vibrato do
            If (temp_songdata.instr_macros[idx].vibrato_table <> 0) then
              If (temp_str <> ' ') then temp_str := temp_str+'+ViB'
              else temp_str := temp_str+'MACRO:ViB';

          a2w_queue[3+idx] := a2w_queue[3+idx]+'  '+ExpStrR(temp_str,18,' ');
          a2w_queue_more[3+idx] := a2w_queue_more[3+idx]+'  '+ExpStrR(temp_str,18,' ');
          a2w_queue_more2[3+idx] := a2w_queue_more2[3+idx]+'  '+ExpStrR(temp_str,18,' ');
        end;

      If (a2w_instdata_source <> '') then temp_str := a2w_instdata_source
      else temp_str := '?internal_instrument_data';

      idx := min(1,get_bank_position(temp_str+'?macro?pos',nm_valid));
      fmreg_page := min(1,get_bank_position(temp_str+'?macro?fmreg_page',nm_valid));
      fmreg_hpos := min(1,get_bank_position(temp_str+'?macro?fmreg_hpos',nm_valid));
      fmreg_vpos := min(1,get_bank_position(temp_str+'?macro?fmreg_vpos',nm_valid));
      fmreg_left_margin := min(1,get_bank_position(temp_str+'?macro?fmreg_left_margin',nm_valid));
      fmreg_cursor_pos := min(1,get_bank_position(temp_str+'?macro?fmreg_cursor_pos',nm_valid));

      If (a2w_instdata_source <> '') then temp_str := ' '+iCASE(NameOnly(a2w_instdata_source))+' '
      else begin
             If updateCurInstr then add_bank_position('?internal_instrument_data?pos',255,current_inst);
             temp_str := ' iNSTRUMENT MACRO BROWSER ';
           end;

      Case program_screen_mode of
        0,
        3: begin
             window_xsize := 73;
             If is_default_screen_mode then
               window_ysize := max(nm_valid+3,15)+10
             else window_ysize := max(nm_valid+3,20)+10;
           end;
        4,
        5: begin
             window_xsize := 73;
             window_ysize := max(nm_valid+3,20)+10;
           end;
        1: begin
             window_xsize := 103;
             window_ysize := max(nm_valid+3,20)+10;
           end;
        2: begin
             window_xsize := 122;
             window_ysize := max(nm_valid+3,20)+10;
           end;
      end;

      xstart := ((work_MaxCol-window_xsize) DIV 2);
      ystart := ((work_MaxLn-window_ysize) DIV 2)+1;
      fmreg_scrlbar_items := nm_valid;
      fmreg_scrlbar_size := window_ysize-10-2;
      If NOT (program_screen_mode in [0,3,4,5]) then _fmreg_add_prev_size := 10
      else _fmreg_add_prev_size := 0;

      If update_current_inst then
        begin
          temp_str := temp_str + '(iNS_  ) ';
          idx := current_inst;
        end
      else temp_str := temp_str + '[iNS_  ] ';
      a2w_institle_pos := (window_xsize DIV 2)+(Length(temp_str) DIV 2)-3;

      ScreenMemCopy(screen_ptr,ptr_temp_screen);
      centered_frame_vdest := ptr_temp_screen;
      centered_frame(xstart,ystart,window_xsize,window_ysize,
                     temp_str,
                     macro_background+macro_border,
                     macro_background+macro_title,
                     frame_double);

      ShowStr(centered_frame_vdest,xstart+1,ystart+window_ysize-10+1,
              ExpStrR('',window_xsize-1,#205),
              macro_background+macro_topic2);
      ShowStr(centered_frame_vdest,xstart+1,ystart+window_ysize-2,
              ExpStrR('',window_xsize-1,#205),
              macro_background+macro_topic2);
      ShowVStr(centered_frame_vdest,xstart+42+_fmreg_add_prev_size,ystart+window_ysize-10+1,
               #203#186#186#186#186#186#186#202,
               macro_background+macro_topic2);

      Move(mn_setting.terminate_keys,old_keys,SizeOf(old_keys));
      old_external_proc := mn_environment.ext_proc;
      old_topic_len := mn_setting.topic_len;
      old_cycle_moves := mn_setting.cycle_moves;
      old_topic_mask_chr := mn_setting.topic_mask_chr;
      old_frame_enabled := mn_setting.frame_enabled;
      old_shadow_enabled := mn_setting.shadow_enabled;
      old_winshade := mn_environment.winshade;
      old_center_box := mn_setting.center_box;
      old_show_scrollbar := mn_setting.show_scrollbar;
      old_text_attr := mn_setting.text_attr;
      old_text2_attr := mn_setting.text2_attr;
      old_short_attr := mn_setting.short_attr;
      old_short2_attr := mn_setting.short2_attr;
      old_disbld_attr := mn_setting.disbld_attr;
      old_contxt_attr := mn_setting.contxt_attr;
      old_contxt2_attr := mn_setting.contxt2_attr;
      old_topic_attr := mn_setting.topic_attr;
      old_hi_topic_attr := mn_setting.hi_topic_attr;

      Move(new_keys,mn_setting.terminate_keys,SizeOf(new_keys));
      If NOT update_current_inst then
        mn_setting.terminate_keys[4] := 0; // TAB not possible when called from Macro Browser
      If NOT loadBankPossible then
        mn_setting.terminate_keys[5] := 0; // ^ENTER possible only in Instrument Control

      mn_environment.ext_proc := a2w_lister_external_proc;
      mn_setting.topic_len := 3;
      mn_setting.cycle_moves := FALSE;
      mn_setting.frame_enabled := FALSE;
      mn_setting.shadow_enabled := FALSE;
      mn_environment.winshade := FALSE;
      mn_setting.center_box := FALSE;
      mn_setting.show_scrollbar := FALSE;
      mn_environment.unpolite := FALSE;
      mn_environment.preview := TRUE;
      mn_environment.v_dest := ptr_temp_screen;
      mn_setting.text_attr := macro_background+macro_item;
      mn_setting.text2_attr := macro_sel_itm_bck+macro_sel_itm;
      mn_setting.short_attr := macro_background+macro_short;
      mn_setting.short2_attr := macro_sel_itm_bck+macro_sel_short;
      mn_setting.disbld_attr := macro_background+macro_item_dis;
      mn_setting.contxt_attr := macro_background+macro_context;
      mn_setting.contxt2_attr := macro_background+macro_context_dis;
      mn_setting.topic_attr := macro_background+macro_topic2;
      mn_setting.hi_topic_attr := macro_background+macro_hi_topic;

      If is_default_screen_mode then ysize := 15
      else ysize := 20;
      _a2w_lister_count := max(nm_valid+3,ysize)-3;

      If NOT _force_program_quit then
        If (program_screen_mode in [0,3,4,5]) then
          index := Menu(a2w_queue,xstart,ystart,idx+3,72,max(nm_valid+3,ysize),nm_valid+3,temp_str)
        else If (program_screen_mode = 1) then
               index := Menu(a2w_queue_more,xstart,ystart,idx+3,102,max(nm_valid+3,20),nm_valid+3,temp_str)
             else index := Menu(a2w_queue_more2,xstart,ystart,idx+3,121,max(nm_valid+3,20),nm_valid+3,temp_str);

      move_to_screen_data := ptr_temp_screen;
      move_to_screen_area[1] := xstart;
      move_to_screen_area[2] := ystart;
      move_to_screen_area[3] := xstart+window_xsize+2+1;
      move_to_screen_area[4] := ystart+window_ysize+1;
      move2screen_alt;

      mn_environment.unpolite := FALSE;
      mn_environment.preview := FALSE;
      mn_environment.v_dest := screen_ptr;
      centered_frame_vdest := mn_environment.v_dest;

      keyboard_reset_buffer;
      If NOT _force_program_quit then
        If (program_screen_mode in [0,3,4,5]) then
          index := Menu(a2w_queue,xstart,ystart,idx+3,72,max(nm_valid+3,ysize),nm_valid+3,temp_str)
        else If (program_screen_mode = 1) then
               index := Menu(a2w_queue_more,xstart,ystart,idx+3,102,max(nm_valid+3,20),nm_valid+3,temp_str)
             else index := Menu(a2w_queue_more2,xstart,ystart,idx+3,121,max(nm_valid+3,20),nm_valid+3,temp_str);

      If (a2w_instdata_source <> '') then temp_str := a2w_instdata_source
      else temp_str := '?internal_instrument_data';

      add_bank_position(temp_str+'?macro?pos',nm_valid,index);
      add_bank_position(temp_str+'?macro?fmreg_page',nm_valid,fmreg_page);
      add_bank_position(temp_str+'?macro?fmreg_hpos',nm_valid,fmreg_hpos);
      add_bank_position(temp_str+'?macro?fmreg_vpos',nm_valid,fmreg_vpos);
      add_bank_position(temp_str+'?macro?fmreg_left_margin',nm_valid,fmreg_left_margin);
      add_bank_position(temp_str+'?macro?fmreg_cursor_pos',nm_valid,fmreg_cursor_pos);

      Move(old_keys,mn_setting.terminate_keys,SizeOf(old_keys));
      mn_environment.ext_proc := old_external_proc;
      mn_setting.topic_len := old_topic_len;
      mn_setting.cycle_moves := old_cycle_moves;
      mn_setting.topic_mask_chr := old_topic_mask_chr;
      mn_setting.frame_enabled := old_frame_enabled;
      mn_setting.shadow_enabled := old_shadow_enabled;
      mn_environment.winshade := old_winshade;
      mn_setting.center_box := old_center_box;
      mn_setting.show_scrollbar := old_show_scrollbar;
      mn_setting.text_attr := old_text_attr;
      mn_setting.text2_attr := old_text2_attr;
      mn_setting.short_attr := old_short_attr;
      mn_setting.short2_attr := old_short2_attr;
      mn_setting.disbld_attr := old_disbld_attr;
      mn_setting.contxt_attr := old_contxt_attr;
      mn_setting.contxt2_attr := old_contxt2_attr;
      mn_setting.topic_attr := old_topic_attr;
      mn_setting.hi_topic_attr := old_hi_topic_attr;

      move_to_screen_data := ptr_screen_backup;
      move_to_screen_area[1] := xstart;
      move_to_screen_area[2] := ystart;
      move_to_screen_area[3] := xstart+window_xsize+2+1;
      move_to_screen_area[4] := ystart+window_ysize+1;
      move2screen;

      centered_frame_vdest := screen_ptr;
      If (mn_environment.keystroke = kESC) then
        GOTO _end;

      If (mn_environment.keystroke = kTAB) then
        begin
          arp_tab_selected := songdata.instr_macros[current_inst].arpeggio_table <> 0;
          vib_tab_selected := songdata.instr_macros[current_inst].vibrato_table <> 0;
          loadMacros := FALSE;
          browser_flag := TRUE;
          GOTO _jmp1;
        end;

      If (mn_environment.keystroke = kF1) then
        begin
          HELP('macro_browser');
          If NOT _force_program_quit then GOTO _jmp2;
        end;
_jmp2e:
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
                    _put_4op_instrument;
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
                         _put_4op_instrument;
                         set_4op_flag(_4op_idx11);
                       end;
                   end;
              // put 2op instrument
              If NOT _4op_ins_flag then
                _put_2op_instrument;

              load_flag := 1;
              load_flag_alt := BYTE_NULL;
            end
          else
            begin
              If bankSelector then
                index := Dialog('ALL UNSAVED iNSTRUMENT AND MACRO DATA WiLL BE LOST$'+
                                'DO YOU WiSH TO CONTiNUE?$',
                                '~Y~UP$~N~OPE$',' A2W LOADER ',1)
              else begin
                     index := 1;
                     dl_environment.keystroke := kENTER;
                   end;

              If (dl_environment.keystroke <> kESC) and (index = 1) then
                begin
                  temp_str := '';
                  For idx := 1 to 255 do
                    If NOT (idx > nm_valid) then
                      begin
                        songdata.instr_data[idx] := temp_songdata.instr_data[idx];
                        songdata.instr_macros[idx] := temp_songdata.instr_macros[idx];
                        songdata.dis_fmreg_col[idx] := temp_songdata.dis_fmreg_col[idx];
                        songdata.instr_names[idx] := Copy(songdata.instr_names[idx],1,9)+
                                                     Copy(temp_songdata.instr_names[idx],10,32);

                        idx1 := -1;
                        idx2 := -1;
                        If (songdata.instr_macros[idx].arpeggio_table <> 0) then
                          idx1 := get_free_arpeggio_table_idx(temp_songdata.macro_table[
                                                              songdata.instr_macros[idx].arpeggio_table].arpeggio);
                        If (songdata.instr_macros[idx].vibrato_table <> 0) then
                          idx2 := get_free_vibrato_table_idx(temp_songdata.macro_table[
                                                             songdata.instr_macros[idx].vibrato_table].vibrato);
                        If (temp_str = '') then
                          If (idx1 = 0) then
                            If (idx2 = 0) then
                              temp_str := '~ARPEGGiO/ViBRATO'
                            else temp_str := '~ARPEGGiO'
                          else If (idx2 = 0) then
                                 temp_str := '~ViBRATO';

                        If (idx1 > 0) then
                          begin
                            songdata.macro_table[idx1].arpeggio :=
                            temp_songdata.macro_table[songdata.instr_macros[idx].arpeggio_table].arpeggio;
                            songdata.instr_macros[idx].arpeggio_table := idx1;
                          end;
                        If (idx2 > 0) then
                          begin
                            songdata.macro_table[idx2].vibrato :=
                            temp_songdata.macro_table[songdata.instr_macros[idx].vibrato_table].vibrato;
                            songdata.instr_macros[idx].vibrato_table := idx2;
                          end;
                      end
                    else begin
                           FillChar(songdata.instr_data[idx],SizeOf(songdata.instr_data[idx]),0);
                           FillChar(songdata.instr_macros[idx],SizeOf(songdata.instr_macros[idx]),0);
                           FillChar(songdata.dis_fmreg_col[idx],SizeOf(songdata.dis_fmreg_col[idx]),0);
                           songdata.instr_names[idx] := Copy(songdata.instr_names[idx],1,9);
                         end;

                  songdata.ins_4op_flags := temp_songdata.ins_4op_flags;
                  If (temp_str <> '') then
                    Dialog('RELATED '+temp_str+' DATA~ WAS NOT LOADED!$'+
                           'FREE SOME SPACE iN MACRO TABLES AND ~REPEAT THiS ACTiON~$',
                           '~O~K$',' A2W LOADER ',1);
                  load_flag := 1;
                  load_flag_alt := BYTE_NULL;
                end;
            end;
        end;
      end;
_end:
  If browser_flag then GOTO _jmp2;
  arpvib_arpeggio_table := arpvib_arpeggio_table_bak;
  arpvib_vibrato_table := arpvib_vibrato_table_bak;
{$IFDEF GO32V2}
  keyboard_reset_buffer_alt;
{$ENDIF}

  _dbg_leave; //EXIT //a2w_file_loader
end;
