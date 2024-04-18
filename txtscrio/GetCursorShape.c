// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint16_t GetCursorShape (void) {
#if GO32
  return VGA_GetCursorShape ();
#else // !GO32
  return virtual_cur_shape;
#endif // !GO32
}
