// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
/*static*/ void volume_slide (uint8_t chan, uint8_t up_speed, uint8_t down_speed)
{
  chan--;

  if (up_speed)
    slide_volume_up (chan + 1, up_speed);
  else
    if (down_speed)
      slide_volume_down (chan + 1, down_speed);
}
