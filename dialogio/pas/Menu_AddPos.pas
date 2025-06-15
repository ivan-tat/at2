// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `Menu' procedure

procedure AddPos(var p: Word);

var
  temp: Word;

begin
  temp := p;
  If (temp < len2) and (temp < last) and
     mbuf[temp+page].use then
    Inc(temp)
  else If (temp < len2) and (temp < last) then
         begin
           Inc(temp);
           AddPos(temp);
         end
       else If (page+temp <= last) then
              Inc(page);
  If mbuf[temp+page-1].use then p := temp
  else If (temp+page-1 < last) then AddPos(temp);
end;
