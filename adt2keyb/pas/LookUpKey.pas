// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function LookUpKey(key: Word; var table; size: Byte): Boolean;

var
  idx: Byte;

begin
  LookUpKey := FALSE;
  If (size <> 0) then
    For idx := 0 to PRED(size) do
      If (pWord(@table)[idx] = key) then
        begin
          LookUpKey := TRUE;
          BREAK;
        end;
end;
