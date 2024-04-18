// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ void VGA_wait_while_vertical_retrace (bool active) {
  // Input Status #1 Register (Read at 3BAh (mono) or 3DAh (color))
  // Bits 7-4            - unused
  // Bit  3   (VRetrace) - Vertical Retrace:
  //                       0 = not in vertical retrace interval
  //                       1 = the display is in a vertical retrace interval
  // Bits 2-1            - unused
  // Bit  0   (DD)       - DD

  if (active)
    // Wait while ray is invisible (vertically off-screen)
    while (inportb (VGA_STATUS_1_PORT) & 8);
  else
    // Wait while ray is potentially visible (vertically on-screen)
    while (!(inportb (VGA_STATUS_1_PORT) & 8));
}
