// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `Menu' procedure

procedure edit_contents(item: Word);

var
  item_str,temp: String;

begin
  _dbg_enter ({$I %FILE%}, 'Menu.edit_contents');

  is_setting.append_enabled := TRUE;
  is_setting.character_set  := [' '..'_','a'..'}',#128..#255]; // exclude ` and ~ characters
  is_environment.locate_pos := 1;

  item_str := pstr(item);
  If (mn_environment.edit_pos > 0) and (mn_environment.edit_pos < max-2) then
    temp := Copy(item_str,mn_environment.edit_pos+1,
                 Length(item_str)-mn_environment.edit_pos+1)
  else
    temp := CutStr(item_str);

  mn_environment.is_editing := TRUE;
  While (temp <> '') and (temp[Length(temp)] = ' ') do Delete(temp,Length(temp),1);
  temp := InputStr(temp,x+1+mn_environment.edit_pos,y+idx2,
               max-2-mn_environment.edit_pos+1,
               max-2-mn_environment.edit_pos+1,
               mn_setting.text2_attr,mn_setting.default_attr);
  mn_environment.is_editing := FALSE;

  HideCursor;
  If (is_environment.keystroke = kENTER) then
    begin
      If (mn_environment.edit_pos > 0) and (mn_environment.edit_pos < max-2) then
        temp := Copy(item_str,1,mn_environment.edit_pos)+temp
      else
        temp := CutStr(temp);
      Move(temp,pBYTE(mnu_data)[(item-1)*(len+1)],len+1);
    end;

  mn_environment.do_refresh := TRUE;
  refresh;

  _dbg_leave; //EXIT //Menu.edit_contents
end;
