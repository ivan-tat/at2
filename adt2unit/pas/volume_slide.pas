// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure volume_slide(chan,up_speed,down_speed: Byte);
begin
  If (up_speed <> 0) then slide_volume_up(chan,up_speed)
  else If (down_speed <> 0) then slide_volume_down(chan,down_speed);
end;
