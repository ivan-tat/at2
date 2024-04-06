// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void ResetMode (void) {
#if !USE_FPC
  uint16_t orig_fs;
#endif // !USE_FPC

  VBIOS_set_video_mode (v_mode, DispPg);

  v_seg = 0xB800;
  v_ofs = 0;

#if !USE_FPC
  orig_fs = _fargetsel ();
  _farsetsel (_dos_ds);
#endif // !USE_FPC

  MaxCol = _farnspeekw (0x44A); // screen width in text columns
  MaxLn  = _farnspeekb (0x484) + 1; // EGA text rows - 1

#if !USE_FPC
  _farsetsel (orig_fs);
#endif // !USE_FPC

  memsetw (screen_ptr, 0x0700, MAX_SCREEN_MEM_SIZE / 2);
  dosmemput (screen_ptr, MAX_SCREEN_MEM_SIZE, v_seg * 16 + v_ofs);
}
