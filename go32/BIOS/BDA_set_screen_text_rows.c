// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// rows: 1-256
static __inline__ void BDA_set_screen_text_rows (uint16_t rows) {
  _farnspokeb (0x484, rows - 1); // Text rows on the screen - 1 (EGA+)
}
