// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `Dialog' procedure

function RetKey(code: Byte): Word;

var
  temp: Byte;

begin
  RetKey := 0;
  For temp := 1 to nm2 do
    If (UpCase(dbuf[temp].key) = UpCase(CHR(code))) then
      begin
        RetKey := temp;
        BREAK;
      end;
end;
