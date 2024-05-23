// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ uint16_t BDA_get_keyboard_buffer_head (void) {
  return _farnspeekw (0x41A); // Offset from 0x400 to keyboard buffer head
}
