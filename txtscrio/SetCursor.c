// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void SetCursor (int32_t cursor) {
#if GO32
#if !ADT2PLAY
  virtual_cur_pos = (virtual_cur_pos & ~0xFFFF) | ((uint32_t) cursor >> 16);
#endif // !ADT2PLAY
#else // !GO32
  virtual_cur_pos = (uint32_t) cursor >> 16;
#endif // !GO32

  SetCursorShape (cursor & 0xFFFF);

#if GO32
  VBIOS_set_cursor_pos (v_page, ((uint32_t) cursor >> 16) & 0xFF,
                        (uint32_t) cursor >> 24);
#endif // GO32
}
