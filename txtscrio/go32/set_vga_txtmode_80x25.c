// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void set_vga_txtmode_80x25 (void) {
  VGA_SetTextMode_80x25 (0);

  MaxCol = v_cols;
  MaxLn = v_rows;

  memsetw (screen_ptr, 0x0700, MAX_SCREEN_MEM_SIZE / 2);
  dosmemput (screen_ptr,
             v_regen_size <= MAX_SCREEN_MEM_SIZE ? v_regen_size : MAX_SCREEN_MEM_SIZE,
             v_seg * 16 + v_ofs);
}
