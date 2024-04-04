// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void set_vga_txtmode_80x25 (void) {
  VBIOS_set_video_mode (3, 0);

  v_seg = 0xB800;
  v_ofs = 0;

  MaxCol = 80;
  MaxLn  = 25;

  memsetw (screen_ptr, 0x0700, MAX_SCREEN_MEM_SIZE / 2);
  dosmemput (screen_ptr, MAX_SCREEN_MEM_SIZE, v_seg * 16 + v_ofs);
}
