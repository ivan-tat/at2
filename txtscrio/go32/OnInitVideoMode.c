// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void OnInitVideoMode (bool clear_screen) {
  MaxCol = v_cols;
  MaxLn = v_rows;
#if !ADT2PLAY
  work_MaxCol = v_cols;
  work_MaxLn = v_rows;
  virtual_cur_shape = v_curshape;
#endif // !ADT2PLAY

  if (clear_screen) {
    memsetw (screen_ptr, 0x0700, MAX_SCREEN_MEM_SIZE / 2);
#if !ADT2PLAY
    dosmemput (screen_ptr,
               (v_regen_size <= MAX_SCREEN_MEM_SIZE) ? v_regen_size
                                                     : MAX_SCREEN_MEM_SIZE,
                                                       v_seg * 16 + v_ofs);
#endif // !ADT2PLAY
  }
}
