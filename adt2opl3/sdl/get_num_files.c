// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ uint8_t get_num_files (void) {
  uint8_t result = 18, i;

  for (i = 0; i < 18; i++)
    if (!opl3_record_channel[i])
      result--;

  return result ? result : 1;
}
