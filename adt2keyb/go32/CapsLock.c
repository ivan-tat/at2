// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool CapsLock (void) {
  uint16_t orig_fs = _fargetsel ();
  bool r;

  _farsetsel (_dos_ds);
  r = (BDA_get_keyboard_flag_byte_0 () & 0x40) != 0;
  _farsetsel (orig_fs);

  return r;
}
