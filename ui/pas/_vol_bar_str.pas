// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function _vol_bar_str(value: Byte): String;

var
  result: String;

begin
  result := '';
  Repeat
    If (value > 3) then
      begin
        result := result+#168;
        Dec(value,3);
      end;
    If (value <= 3) and (value <> 0) then
      result := result+CHR(164+value)
  until (value <= 3);
 _vol_bar_str := result;
end;
