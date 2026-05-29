// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure import_old_flags;

var
  temp: Byte;

begin
  If (songdata.common_flag OR 2 = songdata.common_flag) then
    For temp := 1 to 20 do
      songdata.lock_flags[temp] := songdata.lock_flags[temp] OR $10;

  If (songdata.common_flag OR 4 = songdata.common_flag) then
    For temp := 1 to 20 do
      songdata.lock_flags[temp] := songdata.lock_flags[temp] OR $20;

  If (songdata.common_flag OR $20 = songdata.common_flag) then
    For temp := 1 to 20 do
      songdata.lock_flags[temp] := songdata.lock_flags[temp] AND NOT 3;
end;
