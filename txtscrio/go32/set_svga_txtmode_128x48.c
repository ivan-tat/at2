// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void set_svga_txtmode_128x48 (void) {
  // set using VESA gfx mode 1024x768
  set_svga_txtmode (0x104, 128, 48);
}
