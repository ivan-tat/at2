// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `Menu' procedure

procedure SubPos(var p: Word);

var
  temp: Word;

begin
  temp := p;
  If (temp > 1) and mbuf[temp+page-2].use then
    Dec(temp)
  else If (temp > 1) then
         begin
           Dec(temp);
           SubPos(temp);
         end
       else If (page > first) then
              Dec(page);
  If mbuf[temp+page-1].use then p := temp
  else If (temp+page-1 > first) then SubPos(temp);
end;
