// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint8_t calc_pattern_pos (uint8_t pattern) {
  int16_t index;
  uint8_t jump_count = 0, pattern_pos = BYTE_NULL;

  DBG_ENTER ("calc_pattern_pos");

  index = calc_following_order (0);
  while ((index != -1) && (jump_count < 0x7F)) {
    if (songdata.pattern_order[index] != pattern) {
      if (index < 0x7F) {
        index = calc_following_order (index + 1);
        jump_count++;
      } else
        break;
    } else {
      pattern_pos = index;
      break;
    }
  }

  return pattern_pos;
}
