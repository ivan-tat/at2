// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// page: 0-7
static __inline__ void BDA_set_cursor_position (uint8_t page, uint8_t x,
                                                uint8_t y) {

  _farnspokew (0x450 + page * 2, x + (y << 8));
}
