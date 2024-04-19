// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void set_svga_txtmode (uint16_t mode, uint8_t cols, uint8_t rows) {
#if !USE_FPC
  uint16_t orig_fs;
#endif // !USE_FPC
  uint16_t CRTC_addr, CRTC_data;

  VESA_set_video_mode (mode);

  // rerogram CRT controller

#if !USE_FPC
  orig_fs = _fargetsel ();
  _farsetsel (_dos_ds);
#endif // !USE_FPC

  CRTC_addr = BDA_get_active_6845_CRTC_port ();
  CRTC_data = CRTC_addr + 1;

  disable ();

  // clear write protection for CRTC register 0-7
  outportb (CRTC_addr, 0x11);
  // vertical retrace end register bit 7 reset
  outportb (CRTC_data, inportb (CRTC_data) & 0x7F);

  outportb (CRTC_addr, 9);
  // clear bits 0-4, set max scan line to 15
  outportb (CRTC_data, (inportb (CRTC_data) & 0xE0) | 0x0F);

  outportw (CRTC_addr, 0x0E0A);
  outportw (CRTC_addr, 0x0F0B);

  // mode control register
  outportb (CRTC_addr, 0x17);
  // set byte mode
  outportb (CRTC_data, inportb (CRTC_data) & ~0x40);

  // restore write protection for CRTC register 0-7
  outportb (CRTC_addr, 0x11);
  outportb (CRTC_data, inportb (CRTC_data) | 0x80);

  // write sequencer: make planes 2+3 write protected
  outportw (VGA_SEQ_ADDR_PORT, 0x0302);

  // set odd/even mode, reset chain 4, more than 64 kB
  outportw (VGA_SEQ_ADDR_PORT, 0x0204);

  // write graphics controller

  // set write mode 0, read mode 0, odd/even addressing
  outportw (VGA_GRAPH_ADDR_PORT, 0x1005);

  // set 0xB800 as base, set text mode, set odd/even
  outportb (VGA_GRAPH_ADDR_PORT, 6);
  outportb (VGA_GRAPH_DATA_PORT,
            (inportb (VGA_GRAPH_DATA_PORT) & 0xF0) | 0x0E);

  // write attribute controller

  // reset attribute controller
  inportb ((inportb (VGA_MISC_READ_PORT) & 1) ? VGA_STATUS_1_PORT
                                              : (VGA_STATUS_1_PORT - 32));

  outportb (VGA_ATTR_WRITE_PORT, 0x10); // select mode register
  outportb (VGA_ATTR_WRITE_PORT, 0x00); // set text mode [bit 0=0]
  outportb (VGA_ATTR_WRITE_PORT, 0x20); // turn screen on again

  enable ();

  MaxCol = cols;
  MaxLn  = rows;

  BDA_set_screen_text_columns (MaxCol);
  BDA_set_screen_text_rows (MaxLn);

#if !USE_FPC
  _farsetsel (orig_fs);
#endif // !USE_FPC

  initialize ();
  dosmemput (screen_ptr, MAX_SCREEN_MEM_SIZE, v_seg * 16 + v_ofs);
}
