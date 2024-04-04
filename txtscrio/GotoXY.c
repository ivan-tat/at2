// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void GotoXY (uint8_t x, uint8_t y) {
  virtual_cur_pos = x | (y << 8);
#if GO32
  VBIOS_set_cursor_pos (DispPg, x + GOTOXY_xshift - 1, y - 1);
#endif // GO32
}
