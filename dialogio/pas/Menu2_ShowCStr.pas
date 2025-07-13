// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// From `MenuLib2' unit
// Local to `refresh' procedure

procedure ShowCStr_clone(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String;
                         atr1,atr2,atr3,atr4: Byte);
var
  temp,len: Byte;
  highlighted: Boolean;

begin
  _dbg_enter ({$I %FILE%}, 'refresh.ShowCStr_clone');

  If NOT (MenuLib2_mn_setting.fixed_len <> 0) then
    begin
      ShowCStr(dest,x,y,str,atr1,atr2);
      _dbg_leave; EXIT; //refresh.ShowCStr_clone
    end;

  highlighted := FALSE;
  len := 0;
  For temp := 1 to Length(str) do
    If (str[temp] = '~') then highlighted := NOT highlighted
    else begin
           If (len < MenuLib2_mn_setting.fixed_len) then
             begin
               If NOT highlighted then ShowStr(dest,x+len,y,str[temp],atr1)
               else ShowStr(dest,x+len,y,str[temp],atr2);
               Inc(len);
             end
           else
             begin
               If NOT highlighted then ShowStr(dest,x+len,y,str[temp],atr3)
               else ShowStr(dest,x+len,y,str[temp],atr4);
               Inc(len);
             end
         end;

  _dbg_leave; //EXIT //refresh.ShowCStr_clone
end;
