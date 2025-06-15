// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `Dialog' procedure

function CurrentKey(pos: Byte): Byte;

var
  idx,temp: Byte;

begin
  temp := 0;
  For idx := 1 to nm2 do
    If (pos in [dbuf[idx].pos,dbuf[idx].pos+CStrLen(dbuf[idx].str)-1]) then
      temp := idx;
  CurrentKey := temp;
end;
