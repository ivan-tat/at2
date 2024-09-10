// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void dump_VESA_buffer (ssize_t buffer_size) {
  ssize_t left_sz = buffer_size;
  const uint8_t *p = _FrameBuffer_mirror;
  uint8_t bank = 0;

  if (!_draw_screen_without_vsync)
    WaitRetrace ();

  while (left_sz > 0) {
    ssize_t copy_sz = (left_sz > 0x10000) ? 0x10000 : left_sz;

    VESA_SwitchBank (bank);
    dosmemput (p, copy_sz, 0xA0000);
    left_sz -= copy_sz;
    p += copy_sz;
    bank++;
  }
}
