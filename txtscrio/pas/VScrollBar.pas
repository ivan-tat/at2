// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "dialogio.pas"

function VScrollBar(dest: tSCREEN_MEM_PTR; x,y: Byte; size: Byte; len1,len2,pos: Word;
                    atr1,atr2: Byte): Word;
var
  temp: Word;

begin
  _dbg_enter ({$I %FILE%}, 'VScrollBar');

  If (size > work_MaxLn-y) then size := work_MaxLn-y;
  If (size < 5) then size := 5;

  If (size-2-1 < 10) then temp := _partial(len1,len2,size-2-1)
  else temp := _partial(len1,len2,size-2-1-2);

  If (pos = temp) then
    begin
      VScrollBar := temp;
      _dbg_leave; EXIT; //VScrollBar
    end;

  If (size < len1) then
    begin
      pos := temp;
      ShowVStr(dest,x,y,#30+ExpStrL('',size-2,#176)+#31,atr1);
      If (size-2-1 < 10) then ShowStr(dest,x,y+1+temp,#178,atr2)
      else ShowVStr(dest,x,y+1+temp,#178#178#178,atr2);
    end
  else ShowVCStr(dest,x,y,'~'#30'~'+ExpStrL('',size-2,#177)+'~'#31'~',atr2,atr1);
  VScrollBar := pos;

  _dbg_leave; //EXIT //VScrollBar
end;
