// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void SetTextDisp (uint16_t x, uint16_t y) {
  unsigned w;

  VGA_WaitDisplayDisabled ();

  if (program_screen_mode == 4 || program_screen_mode == 5)
    w = SCREEN_RES_X / scr_font_width;
  else
    w = MaxCol;

  VGA_SetStartAddress ((y / scr_font_height) * w + (x / scr_font_width));
  VGA_SetPresetRowScan (y % scr_font_height);
}
