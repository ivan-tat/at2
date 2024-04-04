// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint8_t WhereY (void) {
#if GO32
  return (VBIOS_get_cursor_pos (DispPg) >> 8) + 1;
#else // !GO32
  return virtual_cur_pos >> 8;
#endif // !GO32
}
