// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void update_recorded_channels (void) {
  uint8_t i;

  for (i = 0; i < 20; i++)
    opl3_record_channel[i] = channel_flag[i] ? true : false;

  for (i = songdata.nm_tracks; i < 20; i++)
    opl3_record_channel[i] = false;

  if (percussion_mode) {
    if (!channel_flag[18])
      opl3_record_channel[17] = false;
    if (!channel_flag[19])
      opl3_record_channel[16] = false;
  }
}
