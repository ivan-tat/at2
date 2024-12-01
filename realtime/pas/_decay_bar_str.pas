// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function _decay_bar_str(value: Byte): String;

var
  result: String;

begin
  result := '';
  Repeat
    If (value > 15) then
      begin
        result := result+#219;
        Dec(value,15);
      end;
    If (value <= 15) and (value <> 0) then
      result := result+CHR(224+value)
  until (value <= 15);
  _decay_bar_str := FlipStr(result);
end;
