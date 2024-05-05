// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void GotoXY (uint8_t x, uint8_t y) {
#if GO32
#if ADT2PLAY
  VBIOS_set_cursor_pos (v_page, x - 1, y - 1);
#else // !ADT2PLAY
  virtual_cur_pos = x | (y << 8);

  VBIOS_set_cursor_pos (v_page, x + GOTOXY_xshift - 1, y - 1);
#endif // !ADT2PLAY
#else // !GO32
  virtual_cur_pos = x | (y << 8);
#endif // !GO32
}
