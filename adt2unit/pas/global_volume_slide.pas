// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure global_volume_slide(up_speed,down_speed: Byte);
begin
  If (up_speed <> BYTE_NULL) then
    global_volume := max(global_volume+up_speed,63);
  If (down_speed <> BYTE_NULL) then
    If (global_volume >= down_speed) then Dec(global_volume,down_speed)
    else global_volume := 0;
  set_global_volume;
end;
