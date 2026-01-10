// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext5.pas"

procedure bnk_file_loader;

const
  _perc_voice: array[1..5] of String[2] = ('BD','SD','TT','TC','HH');

var
  f: File;
  header: tBNK_HEADER;
  temp: Longint;
  index: Word;
  old_external_proc: procedure; cdecl;
  old_topic_len: Byte;
  old_cycle_moves: Boolean;
  xstart,ystart: Byte;
  nm_valid: Word;
  ysize: Byte;

{$I adt2ext5/pas/bnk_file_loader_restore.pas}

begin { bnk_file_loader }
  _dbg_enter ({$I %FILE%}, 'bnk_file_loader');

  {$i-}
  Assign(f,instdata_source);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' BNK LOADER ',1);
      _dbg_leave; EXIT; //bnk_file_loader
    end;

  BlockReadF(f,header,SizeOf(header),temp);
  If (temp <> SizeOf(header)) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' BNK LOADER ',1);
      _dbg_leave; EXIT; //bnk_file_loader
    end;

  If NOT ((header.fver_major = 1) and (header.fver_minor = 0)) then
    begin
      CloseF(f);
      Dialog('UNKNOWN FiLE FORMAT VERSiON$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' BNK LOADER ',1);
      _dbg_leave; EXIT; //bnk_file_loader
    end;

  If (header.signature <> bnk_id) or
     (header.total_entries < header.entries_used) then
    begin
      CloseF(f);
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' BNK LOADER ',1);
      _dbg_leave; EXIT; //bnk_file_loader
    end;

  ScreenMemCopy(screen_ptr,ptr_screen_backup);
  centered_frame_vdest := screen_ptr;
  dl_environment.context := ' ESC '#196#16' STOP ';
  centered_frame(xstart,ystart,43,3,' '+iCASE(NameOnly(instdata_source))+' ',
                 dialog_background+dialog_border,
                 dialog_background+dialog_title,
                 frame_double);
  ShowStr(screen_ptr,xstart+43-Length(dl_environment.context),ystart+3,
          dl_environment.context,
          dialog_background+dialog_border);
  dl_environment.context := '';

  bnk_queue[1] := bnk_header_str[1];
  bnk_queue[2] := bnk_header_str[2];
  bnk_queue[3] := bnk_header_str[3];

  progress_num_steps := 1;
  progress_step := 1;
  progress_value := max(header.total_entries,MAX_TIMBRES);
  progress_old_value := BYTE_NULL;
  progress_xstart := xstart+2;
  progress_ystart := ystart+2;

  ShowCStr(screen_ptr,xstart+2,ystart+1,
           'LOADiNG DATA FROM BANK FiLE...',
           dialog_background+dialog_text,
           dialog_background+dialog_hi_text);

  show_progress(0);
  nm_valid := 0;
  bnk_skip := 0;

  For index := 1 to max(header.total_entries,MAX_TIMBRES) do
    begin
      If (ticklooper = 0) then
        If keypressed and (index > 1) then
          begin
            fkey := GetKey;
            If (fkey = kESC) then
              begin
                Dec(index);
                BREAK;
              end;
          end;

      If (ticklooper = 0) then
        show_progress2(index,3);

      SeekF(f,header.name_offset+PRED(index)*SizeOf(name_record));
      If (IOresult <> 0) then
        begin
          CloseF(f);
          _restore;
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' BNK LOADER ',1);
          _dbg_leave; EXIT; //bnk_file_loader
        end;

      BlockReadF(f,name_record,SizeOf(name_record),temp);
      If (temp <> SizeOf(name_record)) then
        begin
          CloseF(f);
          _restore;
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' BNK LOADER ',1);
          _dbg_leave; EXIT; //bnk_file_loader
        end;

      SeekF(f,header.data_offset+name_record.data_index*SizeOf(data_record));
      If (IOresult <> 0) then
        begin
          CloseF(f);
          _restore;
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' BNK LOADER ',1);
          _dbg_leave; EXIT; //bnk_file_loader
        end;

      BlockReadF(f,data_record,SizeOf(data_record),temp);
      If (temp <> SizeOf(data_record)) then
        begin
          CloseF(f);
          _restore;
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' BNK LOADER ',1);
          _dbg_leave; EXIT; //bnk_file_loader
        end;

      import_instrument_from_data_record;
      If name_record.usage_flag then
        begin
          bnk_queue[3+index-bnk_skip] := '~ ~~';
          Inc(nm_valid);
        end
      else
        begin
          If (nm_valid = 0) then
            begin
              Inc(bnk_skip);
              CONTINUE;
            end;
          bnk_queue[3+index-bnk_skip] := ' ';
        end;

      bnk_queue[3+index-bnk_skip] := bnk_queue[3+index-bnk_skip]+
        ExpStrR(CutStr(asciiz_string(name_record.ins_name)),11,' ')+'~  ';

      With temp_instrument.fm_data do
        begin
          bnk_queue[3+index-bnk_skip] :=
            bnk_queue[3+index-bnk_skip]+byte2hex(AM_VIB_EG_carrier);
          bnk_queue[3+index-bnk_skip] :=
            bnk_queue[3+index-bnk_skip]+byte2hex(AM_VIB_EG_modulator)+' ';
          bnk_queue[3+index-bnk_skip] :=
            bnk_queue[3+index-bnk_skip]+byte2hex(KSL_VOLUM_carrier);
          bnk_queue[3+index-bnk_skip] :=
            bnk_queue[3+index-bnk_skip]+byte2hex(KSL_VOLUM_modulator)+' ';
          bnk_queue[3+index-bnk_skip] :=
            bnk_queue[3+index-bnk_skip]+byte2hex(ATTCK_DEC_carrier);
          bnk_queue[3+index-bnk_skip] :=
            bnk_queue[3+index-bnk_skip]+byte2hex(ATTCK_DEC_modulator)+' ';
          bnk_queue[3+index-bnk_skip] :=
            bnk_queue[3+index-bnk_skip]+byte2hex(SUSTN_REL_carrier);
          bnk_queue[3+index-bnk_skip] :=
            bnk_queue[3+index-bnk_skip]+byte2hex(SUSTN_REL_modulator)+' ';
          bnk_queue[3+index-bnk_skip] :=
            bnk_queue[3+index-bnk_skip]+byte2hex(WAVEFORM_carrier);
          bnk_queue[3+index-bnk_skip] :=
            bnk_queue[3+index-bnk_skip]+byte2hex(WAVEFORM_modulator)+' ';
          bnk_queue[3+index-bnk_skip] :=
            bnk_queue[3+index-bnk_skip]+byte2hex(FEEDBACK_FM)+'    ';
        end;

      Case data_record.mode of
        0: bnk_queue[3+index-bnk_skip] :=
             bnk_queue[3+index-bnk_skip]+'MELODiC';
        1: Case data_record.voice_num of
             6..10: bnk_queue[3+index-bnk_skip] :=
                      bnk_queue[3+index-bnk_skip]+'PERCUSSiON ('+
                      _perc_voice[data_record.voice_num-5]+')';
             else bnk_queue[3+index-bnk_skip] :=
                    bnk_queue[3+index-bnk_skip]+'PERCUSSiON (??)';
           end;
        else
          bnk_queue[3+index-bnk_skip] :=
            bnk_queue[3+index-bnk_skip]+'???';
      end;
    end;

  CloseF(f);
  show_progress(index);
  // delay for awhile to show progress bar
  {$IFDEF GO32V2}
  CRT.Delay(500);
  {$ELSE}
  SDL_Delay(200);
  {$ENDIF}
  _restore;
  If (nm_valid = 0) then
    begin
      _dbg_leave; EXIT; //bnk_file_loader
    end;

  If (index = header.total_entries) and
     (bnk_skip = 0) then
    mn_environment.context := '~[~'+Num2str(nm_valid,10)+'~/'+
                                    Num2str(index,10)+']~'
  else
    mn_environment.context := '['+Num2str(nm_valid,10)+']~['+
                                  Num2str(index,10)+'/'+
                                  Num2str(header.total_entries,10)+']~';

  old_external_proc := mn_environment.ext_proc;
  old_topic_len := mn_setting.topic_len;
  old_cycle_moves := mn_setting.cycle_moves;
  mn_environment.ext_proc := bnk_lister_external_proc;
  mn_setting.topic_len := 3;
  mn_setting.cycle_moves := FALSE;

  If is_default_screen_mode then ysize := 20
  else ysize := 30;

  keyboard_reset_buffer;
  If NOT _force_program_quit then
    index := Menu(bnk_queue,01,01,min(1,get_bank_position(instdata_source,nm_valid)),
                  72,ysize,nm_valid+3,' '+iCASE(NameOnly(instdata_source))+' ');

  add_bank_position(instdata_source,nm_valid,index+3);
  mn_environment.ext_proc := old_external_proc;
  mn_setting.topic_len := old_topic_len;
  mn_setting.cycle_moves := old_cycle_moves;

  If (mn_environment.keystroke = kENTER) then
    begin
      load_flag := 1;
      load_flag_alt := BYTE_NULL;
      bnk_file_loader_alt(bnk_skip+index);
      If (load_flag_alt <> BYTE_NULL) then
        begin
          songdata.instr_data[current_inst] := temp_instrument;
          songdata.instr_names[current_inst] :=
            Copy(songdata.instr_names[current_inst],1,9)+
            Copy(bnk_queue[3+index],5,8);
        end;
    end;

  _dbg_leave; //EXIT //bnk_file_loader
end;
