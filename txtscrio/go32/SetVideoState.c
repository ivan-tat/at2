// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void SetVideoState (VGAVideoState_t *vs, bool restore_screen) {
  VGA_RestoreVideoState (vs, restore_screen);

  MaxCol = v_cols;
  MaxLn = v_rows;
#if !ADT2PLAY
  virtual_cur_shape = v_curshape;
#endif // !ADT2PLAY
}
