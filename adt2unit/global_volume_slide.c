// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ void global_volume_slide (uint8_t up_speed, uint8_t down_speed)
{
  if (up_speed != BYTE_NULL)
    global_volume = max (global_volume + up_speed, 63);

  if (down_speed != BYTE_NULL)
  {
    if (global_volume >= down_speed)
      global_volume -= down_speed;
    else
      global_volume = 0;
  }

  set_global_volume ();
}
