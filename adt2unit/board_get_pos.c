// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// octave: 0..7
// Returns 0..28 or -1 if failed
int8_t board_get_pos (uint8_t octave, ExtKeyCode fkey)
{
  uint_least8_t board_pos;
  uint_least16_t note;
  uint8_t sc = fkey >> 8;

  for (board_pos = 0, note = 12 * octave; board_pos < 29; board_pos++, note++)
    if (board_scancodes[board_pos] == sc)
      return note <= 12*8 ? board_pos : -1;

  return -1;
}
