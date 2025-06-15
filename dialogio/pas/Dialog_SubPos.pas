// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `Dialog' procedure

procedure SubPos(var p: Word);

var
  temp: Word;

begin
  temp := p;
  If (temp > 1) and dbuf[temp-1].use then Dec(temp)
  else If (temp > 1) then begin Dec(temp); SubPos(temp); end;
  If dbuf[temp].use then p := temp;
end;
