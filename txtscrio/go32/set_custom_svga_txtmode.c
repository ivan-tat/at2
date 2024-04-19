// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void set_custom_svga_txtmode (void) {
#if !USE_FPC
  uint16_t orig_fs;
#endif // !USE_FPC

  LoadVgaRegisters (&svga_txtmode_regs);

  MaxCol = svga_txtmode_cols;
  MaxLn  = svga_txtmode_rows;

#if !USE_FPC
  orig_fs = _fargetsel ();
  _farsetsel (_dos_ds);
#endif // !USE_FPC

  BDA_set_screen_text_columns (MaxCol);
  BDA_set_screen_text_rows (MaxLn);

#if !USE_FPC
  _farsetsel (orig_fs);
#endif // !USE_FPC

  initialize ();
  dosmemput (screen_ptr, MAX_SCREEN_MEM_SIZE, v_seg * 16 + v_ofs);
}
