// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2unit.pas"

function hscroll_bar(x,y: Byte; size: Byte; len1,len2,pos: Word;
                     atr1,atr2: Byte): Byte;
var
  temp: Byte;

begin
  If (size > work_MaxCol-x) then size := work_MaxCol-x;
  If (size < 5) then size := 5;

  If (size-2-1 < 10) then temp := _partial(len1,len2,size-2-1)
  else temp := _partial(len1,len2,size-2-1-2);

  If (pos = temp) and NOT force_scrollbars then
    begin
      hscroll_bar := temp;
      EXIT; //hscroll_bar
    end;

  If (size < len1*4) and (len1 > 4) then
    begin
      pos := temp;
      show_str(x,y,#17+ExpStrL('',size-2,#176)+#16,atr1);
      If (size-2-1 < 10) then show_str(x+1+temp,y,#178,atr2)
      else show_str(x+1+temp,y,#178#178#178,atr2);
    end
  else show_Str(x,y,#17+ExpStrL('',size-2,#177)+#16,atr1);
  hscroll_bar := pos;

  //EXIT //hscroll_bar
end;
