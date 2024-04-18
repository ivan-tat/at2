// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ void VGA_wait_while_display_disabled (bool disabled) {
  // Input Status #1 Register (Read at 3BAh (mono) or 3DAh (color))
  // Bits 7-4            - unused
  // Bit  3   (VRetrace) - Vertical Retrace
  // Bits 2-1            - unused
  // Bit  0   (DD)       - Display Disabled
  //                       0 = not in horizontal or vertical retrace interval
  //                       1 = a horizontal or vertical retrace interval

  if (disabled)
    // Wait while ray is invisible
    // (in any of retrace intervals)
    while (inportb (VGA_STATUS_1_PORT) & 1);
  else
    // Wait while ray is explicitly visible
    // (not in in any of retrace intervals)
    while (!(inportb (VGA_STATUS_1_PORT) & 1));
}
