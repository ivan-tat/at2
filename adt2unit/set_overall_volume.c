// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void set_overall_volume (uint8_t level)
{
  overall_volume = max (level, 63);
  set_global_volume ();
}
