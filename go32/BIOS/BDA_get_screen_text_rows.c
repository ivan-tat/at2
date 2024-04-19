// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Return value: 1-256
static __inline__ uint16_t BDA_get_screen_text_rows (void) {
  return _farnspeekb (0x484) + 1; // Text rows on the screen - 1 (EGA+)
}
