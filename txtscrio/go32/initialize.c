// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void initialize (void) {
#if !USE_FPC
  uint16_t orig_fs;
#endif // !USE_FPC

  VBIOS_get_video_mode (&v_mode, &DispPg);

#if !USE_FPC
  orig_fs = _fargetsel ();
  _farsetsel (_dos_ds);
#endif // !USE_FPC

  MaxCol = _farnspeekw (0x44A); // screen width in text columns
  MaxLn  = _farnspeekb (0x484) + 1; // EGA text rows - 1
#if !ADT2PLAY
  work_MaxCol = MaxCol;
  work_MaxLn  = MaxLn;
#endif // !ADT2PLAY

#if !USE_FPC
  _farsetsel (orig_fs);
#endif // !USE_FPC

  memsetw (screen_ptr, 0x0700, MAX_SCREEN_MEM_SIZE / 2);
#if !ADT2PLAY
  dosmemput (screen_ptr, MAX_SCREEN_MEM_SIZE, v_seg * 16 + v_ofs);
#endif // !ADT2PLAY
}
