// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2unit.pas"

function vscroll_bar(x,y: Byte; size: Byte; len1,len2,pos: Word;
                     atr1,atr2: Byte): Byte;
var
  temp: Byte;

begin
  If (size > work_MaxLn-y) then size := work_MaxLn-y;
  If (size < 5) then size := 5;

  If (size-2-1 < 10) then temp := _partial(len1,len2,size-2-1)
  else temp := _partial(len1,len2,size-2-1-2);

  If (pos = temp) and NOT force_scrollbars then
    begin
      vscroll_bar := temp;
      EXIT; //vscroll_bar
    end;

  If (size < len1*4) and (len1 > 5) then
    begin
      pos := temp;
      show_vstr(x,y,#30+ExpStrL('',size-2,#176)+#31,atr1);
      If (size-2-1 < 10) then show_str(x,y+1+temp,#178,atr2)
      else show_vstr(x,y+1+temp,#178#178#178,atr2);
    end
  else show_vstr(x,y,#30+ExpStrL('',size-2,#177)+#31,atr1);
  vscroll_bar := pos;

  //EXIT //vscroll_bar
end;
