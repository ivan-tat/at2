// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// From `MenuLib2' unit
// Local to `MenuLib2_Menu' procedure

procedure edit_contents(item: Word);

var
  temp: String;

begin
  _dbg_enter ({$I %FILE%}, 'MenuLib2_Menu.edit_contents');

  is_setting.append_enabled := TRUE;
  is_setting.character_set  := [#32..#255];
  is_environment.locate_pos := 1;

  If (MenuLib2_mn_environment.edit_pos > 0) and (MenuLib2_mn_environment.edit_pos < max-2) then
    temp := Copy(pstr(item),MenuLib2_mn_environment.edit_pos+1,
                      Length(pstr(item))-MenuLib2_mn_environment.edit_pos+1)
  else
    temp := CutStr(pstr(item));

  While (temp <> '') and (temp[Length(temp)] = ' ') do Delete(temp,Length(temp),1);
  temp := InputStr(temp,x+1+MenuLib2_mn_environment.edit_pos,y+idx2,
               max-2-MenuLib2_mn_environment.edit_pos+1,
               max-2-MenuLib2_mn_environment.edit_pos+1,
               MenuLib2_mn_setting.text2_attr,MenuLib2_mn_setting.default_attr);

  If (is_environment.keystroke = kENTER) then
    begin
      If (MenuLib2_mn_environment.edit_pos > 0) and (MenuLib2_mn_environment.edit_pos < max-2) then
        temp := Copy(pstr(item),1,MenuLib2_mn_environment.edit_pos)+temp
      else
        temp := CutStr(temp);
      Move(temp,pBYTE(mnu_data)[(item-1)*(len+1)],len+1);
    end;

  ShowCStr(MenuLib2_mn_environment.v_dest,x+1,y+idx2,
           ExpStrR(pstr(item),max+(Length(pstr(item)))-
           CStrLen(pstr(item)),' '),
           MenuLib2_mn_setting.text2_attr,MenuLib2_mn_setting.short2_attr);

  _dbg_leave; //EXIT //MenuLib2_Menu.edit_contents
end;
