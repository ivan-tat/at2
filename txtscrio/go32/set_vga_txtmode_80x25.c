// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void set_vga_txtmode_80x25 (void) {
  VGA_SetTextMode_80x25 (0);
  OnInitVideoMode (true);
}
