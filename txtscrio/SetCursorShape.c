// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void SetCursorShape (uint16_t shape) {
#if GO32
#if !ADT2PLAY
  virtual_cur_shape = shape;
#endif // !ADT2PLAY
  VGA_SetCursorShape (shape);
#else // !GO32
  virtual_cur_shape = shape;
#endif // !GO32
}
