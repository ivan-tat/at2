// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function _patts_marked: Byte;

var
  temp,
  result: Byte;

begin
  result := 0;
  For temp := 0 to $7f do
    If (songdata.pattern_names[temp][1] = #16) then
      Inc(result);
  _patts_marked := result;
end;
