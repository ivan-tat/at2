// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void set_svga_txtmode_100x38 (void) {
  // set using VESA gfx mode 800x600
  set_svga_txtmode (0x102, 100, 38);
}
