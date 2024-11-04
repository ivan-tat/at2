// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "dialogio.pas"

function HScrollBar(dest: tSCREEN_MEM_PTR; x,y: Byte; size: Byte; len1,len2,pos: Word;
                    atr1,atr2: Byte): Word;
var
  temp: Word;

begin
  _dbg_enter ({$I %FILE%}, 'HScrollBar');

  If (size > work_MaxCol-x) then size := work_MaxCol-x;
  If (size < 5) then size := 5;

  If (size-2-1 < 10) then temp := _partial(len1,len2,size-2-1)
  else temp := _partial(len1,len2,size-2-1-2);

  If (pos = temp) then
    begin
      HScrollBar := temp;
      _dbg_leave; EXIT; //HScrollBar
    end;

  If (size < len1) then
    begin
      pos := temp;
      ShowStr(dest,x,y,#17+ExpStrL('',size-2,#176)+#16,atr1);
      If (size-2-1 < 10) then ShowStr(dest,x+1+temp,y,#178,atr2)
      else ShowStr(dest,x+1+temp,y,#178#178#178,atr2);
    end
  else ShowCStr(dest,x,y,'~'#17'~'+ExpStrL('',size-2,#177)+'~'#16'~',atr2,atr1);
  HScrollBar := pos;

  _dbg_leave; //EXIT //HScrollBar
end;
