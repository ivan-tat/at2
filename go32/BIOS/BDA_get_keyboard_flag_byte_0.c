// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ uint8_t BDA_get_keyboard_flag_byte_0 (void) {
  return _farnspeekb (0x417); // Keyboard flag byte 0
  // Bit 0: Right SHIFT key depressed
  // Bit 1: Left SHIFT key depressed
  // Bit 2: CTRL key depressed
  // Bit 3: ALT key depressed
  // Bit 4: Scroll-Lock is active
  // Bit 5: Num-Lock is active
  // Bit 6: Caps-Lock is active
  // Bit 7: Insert is active
}
