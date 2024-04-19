// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ void BDA_set_screen_character_height (uint8_t height) {
  _farnspokeb (0x485, height); // Point height of character matrix (EGA+)
}
