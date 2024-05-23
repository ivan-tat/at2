// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ void BDA_set_keyboard_buffer_tail (uint16_t offset) {
  _farnspokew (0x41C, offset); // Offset from 0x400 to keyboard buffer tail
}
