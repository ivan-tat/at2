// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void set_svga_txtmode (uint16_t mode, uint8_t font, uint8_t cols,
                              uint8_t rows, uint16_t curshape) {

  VESA_set_video_mode (mode);
  VGA_MakeTextMode (font, cols, rows, curshape);
  OnInitVideoMode (true);
}
