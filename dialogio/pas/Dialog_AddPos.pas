// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `Dialog' procedure

procedure AddPos(var p: Word);

var
  temp: Word;

begin
  temp := p;
  If (temp < nm2) and dbuf[temp+1].use then Inc(temp)
  else If (temp < nm2) then begin Inc(temp); AddPos(temp); end;
  If dbuf[temp].use then p := temp;
end;
