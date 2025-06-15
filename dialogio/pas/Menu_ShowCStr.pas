// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `refresh' procedure

procedure ShowCStr_clone(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String;
                         atr1,atr2,atr3,atr4: Byte);
var
  temp,
  len,len2: Byte;
  highlighted: Boolean;

begin
  _dbg_enter ({$I %FILE%}, 'refresh.ShowCStr_clone');

  If NOT (mn_setting.fixed_len <> 0) then
    begin
      ShowC3Str(dest,x,y,str,atr1,atr2,atr1 AND $0f0+mn_setting.topic_attr AND $0f);
      _dbg_leave; EXIT; //refresh.ShowCStr_clone
    end;

  highlighted := FALSE;
  len := 0;
  len2 := 0;
  For temp := 1 to Length(str) do
    If (str[temp] = '~') then highlighted := NOT highlighted
    else begin
           If (temp >= mn_environment.fixed_start) and
              (len < mn_setting.fixed_len) then
             begin
               If NOT highlighted then ShowStr(dest,x+len2,y,str[temp],atr1)
               else ShowStr(dest,x+len2,y,str[temp],atr2);
               Inc(len);
               Inc(len2);
             end
           else
             begin
               If NOT highlighted then ShowStr(dest,x+len2,y,str[temp],atr3)
               else ShowStr(dest,x+len2,y,str[temp],atr4);
               If (temp >= mn_environment.fixed_start) then Inc(len);
               Inc(len2);
             end
         end;

  _dbg_leave; //EXIT //refresh.ShowCStr_clone
end;
