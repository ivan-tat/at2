// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void keyboard_reset_buffer_alt (void) {
  uint16_t orig_fs;

  DBG_ENTER ("keyboard_reset_buffer_alt");

  orig_fs = _fargetsel ();
  _farsetsel (_dos_ds);
  if (BDA_get_keyboard_buffer_tail ()
  -   BDA_get_keyboard_buffer_head () > 5)
    BDA_set_keyboard_buffer_tail (BDA_get_keyboard_buffer_head ());
  _farsetsel (orig_fs);
}
