// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure update_recorded_channels;

var
  idx: Byte;

begin
  For idx := 1 to 20 do
    If channel_flag[idx] then opl3_record_channel[idx] := TRUE
    else opl3_record_channel[idx] := FALSE;
  For idx := SUCC(songdata.nm_tracks) to 20 do
    opl3_record_channel[idx] := FALSE;
  If percussion_mode then
    begin
      If NOT channel_flag[19] then opl3_record_channel[18] := FALSE;
      If NOT channel_flag[20] then opl3_record_channel[17] := FALSE;
    end;
end;
