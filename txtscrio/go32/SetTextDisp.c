// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void SetTextDisp (uint16_t x, uint16_t y) {
  uint_least8_t maxcol_val;
  uint_least16_t t;

  while (inportb (VGA_STATUS_1_PORT) & 1) { }
  while (!(inportb (VGA_STATUS_1_PORT) & 1)) { }

  if (program_screen_mode != 4 && program_screen_mode != 5)
    maxcol_val = MaxCol;
  else
    maxcol_val = SCREEN_RES_X / scr_font_width;

  t = (y / scr_font_height) * maxcol_val + (x / scr_font_width);

  outportb (VGA_CRTC_ADDR_PORT, 0x0C);
  outportb (VGA_CRTC_DATA_PORT, (t >> 8) & 0xFF);

  outportb (VGA_CRTC_ADDR_PORT, 0x0D);
  outportb (VGA_CRTC_DATA_PORT, t & 0xFF);

  outportb (VGA_CRTC_ADDR_PORT, 0x08);
  outportb (VGA_CRTC_DATA_PORT,
            (inportb (VGA_CRTC_DATA_PORT) & 0xE0) | (y & 0x0F));
}
