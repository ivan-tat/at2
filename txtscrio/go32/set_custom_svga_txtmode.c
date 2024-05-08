// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void set_custom_svga_txtmode (void) {
  VGA_MakeCustomTextModeEx (svga_txtmode_regs,
                            sizeof (svga_txtmode_regs) / sizeof (svga_txtmode_regs[0]),
                            svga_txtmode_cols, svga_txtmode_rows);
  OnInitVideoMode (true);
}
