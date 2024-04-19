// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ uint8_t BDA_get_screen_character_height (void) {
  return _farnspeekb (0x485); // Point height of character matrix (EGA+)
}
