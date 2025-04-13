// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void shift_text_screen (void)
{
  uint8_t xsize = SCREEN_RES_X / scr_font_width;
  uint8_t xshift = (xsize - MAX_COLUMNS) / 2;

  memset (ptr_temp_screen2, 0, SCREEN_MEM_SIZE);
  memcpy ((uint8_t *) ptr_temp_screen2 + xshift * 2, screen_ptr, MAX_ROWS * xsize * 2);
}
