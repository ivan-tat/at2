// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure FILE_save(ext: String);

var
  quit_flag: Boolean;
  temp_str:  String;
  temp,mpos: Byte;
  old_songdata_source,
  old_instdata_source: String;

label _jmp1,_jmp2;

begin
  _dbg_enter ({$I %FILE%}, 'FILE_save');

  old_songdata_source := songdata_source;
  old_instdata_source := instdata_source;

  If (songdata_source <> '') then
    songdata_source := iCase_filename(PathOnly(songdata_source))+
                             Lower_filename(BaseNameOnly(songdata_source))+'.'+ext;

  If (instdata_source <> '') then
    instdata_source := iCase_filename(PathOnly(instdata_source))+
                             Lower_filename(BaseNameOnly(instdata_source))+'.'+ext;
_jmp1:
  If quick_cmd then
    If ((Lower_filename(ext) = 'a2m') or (Lower_filename(ext) = 'a2t')) and
        (songdata_source <> '') then GOTO _jmp2;

  Repeat
    is_setting.append_enabled    := TRUE;
    is_setting.character_set     := [#32..#255];
    dl_setting.center_text       := FALSE;
    dl_setting.terminate_keys[3] := kTAB;
    is_setting.terminate_keys[3] := kTAB;
    is_environment.locate_pos    := 1;
    dl_environment.context       := ' TAB '#196#16' FiLE SELECTOR ';

    If (Lower_filename(ext) = 'a2i') then
      begin
        If NOT alt_ins_name then
          begin
            If (a2i_default_path = '') then dl_environment.input_str := instdata_source
            else dl_environment.input_str := iCase_filename(a2i_default_path)+NameOnly(instdata_source);
          end
        else dl_environment.input_str := iCase_filename(a2i_default_path)+
               'instr'+ExpStrL(Num2str(current_inst,10),3,'0')+'.a2i';
      end;

    If (Lower_filename(ext) = 'a2f') then
      begin
        If NOT alt_ins_name then
          begin
            If (a2f_default_path = '') then dl_environment.input_str := instdata_source
            else dl_environment.input_str := iCase_filename(a2f_default_path)+NameOnly(instdata_source);
          end
        else dl_environment.input_str := iCase_filename(a2f_default_path)+
               'instr'+ExpStrL(Num2str(current_inst,10),3,'0')+'.a2f';
      end;

    If (Lower_filename(ext) = 'a2b') then
      If (a2b_default_path = '') then dl_environment.input_str := instdata_source
      else dl_environment.input_str := iCase_filename(a2b_default_path)+NameOnly(instdata_source);

    If (Lower_filename(ext) = 'a2w') then
      If (a2w_default_path = '') then dl_environment.input_str := instdata_source
      else dl_environment.input_str := iCase_filename(a2w_default_path)+NameOnly(instdata_source);

    If (Lower_filename(ext) = 'a2m') then
      If (a2m_default_path = '') then dl_environment.input_str := songdata_source
      else dl_environment.input_str := iCase_filename(a2m_default_path)+NameOnly(songdata_source);

    If (Lower_filename(ext) = 'a2t') then
      If (a2t_default_path = '') then dl_environment.input_str := songdata_source
      else dl_environment.input_str := iCase_filename(a2t_default_path)+NameOnly(songdata_source);

    If (Lower_filename(ext) = 'a2p') then
      If (a2p_default_path = '') then dl_environment.input_str := songdata_source
      else dl_environment.input_str := iCase_filename(a2p_default_path)+NameOnly(songdata_source);

    Dialog('{PATH}[FiLENAME] EXTENSiON iS SET TO "'+iCase_filename(ext)+'"$',
           '%string_input%255$50'+
           '$'+Num2str(dialog_input_bckg+dialog_input,16)+
           '$'+Num2str(dialog_def_bckg+dialog_def,16)+
           '$',' SAVE FiLE ',0);

    dl_setting.terminate_keys[3] := 0;
    is_setting.terminate_keys[3] := 0;
    dl_setting.center_text       := TRUE;
    dl_environment.context       := '';

    If (dl_environment.keystroke = kESC) or
       ((dl_environment.keystroke <> kTAB) and
       (BaseNameOnly(dl_environment.input_str) = '')) then
      begin
        songdata_source := old_songdata_source;
        instdata_source := old_instdata_source;
        _dbg_leave; EXIT; //FILE_save
      end;

    If (dl_environment.keystroke = kENTER) then
      begin
        If (Lower_filename(ext) = 'a2m') then
          songdata_source := iCase_filename(PathOnly(dl_environment.input_str))+
                             Lower_filename(BaseNameOnly(dl_environment.input_str))+
                             '.a2m';

        If (Lower_filename(ext) = 'a2t') then
          songdata_source := iCase_filename(PathOnly(dl_environment.input_str))+
                             Lower_filename(BaseNameOnly(dl_environment.input_str))+
                             '.a2t';

        If (Lower_filename(ext) = 'a2p') then
          songdata_source := iCase_filename(PathOnly(dl_environment.input_str))+
                             Lower_filename(BaseNameOnly(dl_environment.input_str))+
                             '.a2p';

        If (Lower_filename(ext) = 'a2i') then
          instdata_source := iCase_filename(PathOnly(dl_environment.input_str))+
                             Lower_filename(BaseNameOnly(dl_environment.input_str))+
                             '.a2i';

        If (Lower_filename(ext) = 'a2f') then
          instdata_source := iCase_filename(PathOnly(dl_environment.input_str))+
                             Lower_filename(BaseNameOnly(dl_environment.input_str))+
                             '.a2f';

        If (Lower_filename(ext) = 'a2b') then
          instdata_source := iCase_filename(PathOnly(dl_environment.input_str))+
                             Lower_filename(BaseNameOnly(dl_environment.input_str))+
                             '.a2b';

        If (Lower_filename(ext) = 'a2w') then
          instdata_source := iCase_filename(PathOnly(dl_environment.input_str))+
                             Lower_filename(BaseNameOnly(dl_environment.input_str))+
                             '.a2w';
      end;

    quit_flag := TRUE;
    If (dl_environment.keystroke = kTAB) then
      begin
        If (Lower_filename(ext) <> 'a2i') and (Lower_filename(ext) <> 'a2f') and
           (Lower_filename(ext) <> 'a2b') and (Lower_filename(ext) <> 'a2w') then mpos := 3
        else mpos := 4;

        fs_environment.last_file := last_file[mpos];
        fs_environment.last_dir  := last_dir[mpos];

        temp_str := Fselect('*.'+ext+'$');

        last_file[mpos] := fs_environment.last_file;
        last_dir[mpos]  := fs_environment.last_dir;

        If (mn_environment.keystroke = kESC) then quit_flag := FALSE
        else begin
               If (Lower_filename(ext) = 'a2m') then
                 songdata_source := iCase_filename(PathOnly(temp_str))+
                                    Lower_filename(BaseNameOnly(temp_str))+'.a2m';

               If (Lower_filename(ext) = 'a2t') then
                 songdata_source := iCase_filename(PathOnly(temp_str))+
                                    Lower_filename(BaseNameOnly(temp_str))+'.a2t';

               If (Lower_filename(ext) = 'a2p') then
                 songdata_source := iCase_filename(PathOnly(temp_str))+
                                    Lower_filename(BaseNameOnly(temp_str))+'.a2p';

               If (Lower_filename(ext) = 'a2i') then
                 instdata_source := iCase_filename(PathOnly(temp_str))+
                                    Lower_filename(BaseNameOnly(temp_str))+'.a2i';

               If (Lower_filename(ext) = 'a2f') then
                 instdata_source := iCase_filename(PathOnly(temp_str))+
                                    Lower_filename(BaseNameOnly(temp_str))+'.a2f';

               If (Lower_filename(ext) = 'a2b') then
                 instdata_source := iCase_filename(PathOnly(temp_str))+
                                    Lower_filename(BaseNameOnly(temp_str))+'.a2b';

               If (Lower_filename(ext) = 'a2w') then
                 instdata_source := iCase_filename(PathOnly(temp_str))+
                                    Lower_filename(BaseNameOnly(temp_str))+'.a2w';
             end;
      end;
  until quit_flag;

  If (dl_environment.keystroke = kESC) then
    begin
      songdata_source := old_songdata_source;
      instdata_source := old_instdata_source;
      _dbg_leave; EXIT; //FILE_save
    end;

_jmp2:
 If (Lower_filename(ext) = 'a2i') or (Lower_filename(ext) = 'a2f') or
    (Lower_filename(ext) = 'a2b') or (Lower_filename(ext) = 'a2w') then
    temp_str := instdata_source;
  If (Lower_filename(ext) = 'a2m') or (Lower_filename(ext) = 'a2t') or (Lower_filename(ext) = 'a2p') then
    temp_str := songdata_source;

  If (Lower_filename(ext) = 'a2m') then temp := _a2m_saver;
  If (Lower_filename(ext) = 'a2t') then temp := _a2t_saver;
  If (Lower_filename(ext) = 'a2i') then temp := _a2i_saver;
  If (Lower_filename(ext) = 'a2f') then temp := _a2f_saver;
  If (Lower_filename(ext) = 'a2p') then temp := _a2p_saver;
  If (Lower_filename(ext) = 'a2b') then temp := _a2b_saver;
  If (Lower_filename(ext) = 'a2w') then temp := _a2w_saver;

  If (temp = BYTE_NULL) then GOTO _jmp1;

  _dbg_leave; //EXIT //FILE_save
end;
