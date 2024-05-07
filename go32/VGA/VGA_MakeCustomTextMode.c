// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_MakeCustomTextMode (const VGACustomTextMode_t *mode) {
  bool iflag;
  uint16_t orig_fs;
  uint16_t ca, cd; // CRTC Address/Data ports (0x3B4 (mono) or 0x3D4 (color))
  uint16_t s1; // Input Status #1 port (0x3BA (mono) or 0x3DA (color))
  int i;

  iflag = disable ();

  // Miscellaneous Output Register:
  // Bit  7 (VSYNCP)   - Vertical Sync Polarity
  // Bit  6 (HSYNCP)   - Horizontal Sync Polarity
  // Bit  5 (O/E Page) - Odd/Even Page Select
  // Bit  4            - unused
  // Bits 3-2          - Clock Select
  // Bit  1 (RAM En.)  - RAM Enable
  // Bit  0 (I/OAS)    - Input/Output Address Select
  //                     (0 = 0x3BX CRTC port, 1 = 0x3DX CRTC port)
  ca = (inportb (VGA_MISC_READ_PORT) & 1) ? VGA_CRTC_ADDR_PORT
                                          : VGA_CRTC_ADDR_PORT - 32;
  //cd = ca + 1;
  s1 = ca + 6;

  // Reset
  inportb (s1);

  // Disable video output
  outportb (VGA_ATTR_WRITE_PORT, 0); // Attribute Address Register
  // Bits 7-6 - unused
  // Bit  5   - Palette Address Source:
  //            0 = load color values to the registers in the internal palette
  //            1 = normal operation of the attribute controller
  // Bits 4-0 - Attribute Address

  /*** Sequencer ***/

  // Allow sequencer to change clock mode
  outportw (VGA_SEQ_ADDR_PORT, 0x0100); // Reset Register
  // Bits 7-2    - unused
  // Bit  1 (SR) - Sychnronous Reset
  // Bit  0 (AR) - Asynchronous Reset

  for (i = 0; i < 4; i++)
    outportw (VGA_SEQ_ADDR_PORT, (i + 1) | (mode->Seq[i] << 8));

  /*** Miscellaneous Output ***/

  outportb (VGA_MISC_WRITE_PORT, mode->Misc); // Miscellaneous Output Register
  // Bit  7 (VSYNCP)   - Vertical Sync Polarity
  // Bit  6 (HSYNCP)   - Horizontal Sync Polarity
  // Bit  5 (O/E Page) - Odd/Even Page Select
  // Bit  4            - unused
  // Bits 3-2          - Clock Select:
  //                     00 = 25 Mhz clock (used for 320/640 pixel wide modes)
  //                     01 = 28 Mhz clock (used for 360/720 pixel wide modes)
  //                     10, 11 = undefined (possible external clock)
  // Bit  1 (RAM En.)  - RAM Enable
  // Bit  0 (I/OAS)    - Input/Output Address Select
  //                     (0 = 0x3BX CRTC port, 1 = 0x3DX CRTC port)

  /*** Sequencer ***/

  // Enable normal operation of sequencer
  outportw (VGA_SEQ_ADDR_PORT, 0x0300); // Reset Register
  // Bits 7-2    - unused
  // Bit  1 (SR) - Sychnronous Reset
  // Bit  0 (AR) - Asynchronous Reset

  // Miscellaneous Output Register:
  // Bit  7 (VSYNCP)   - Vertical Sync Polarity
  // Bit  6 (HSYNCP)   - Horizontal Sync Polarity
  // Bit  5 (O/E Page) - Odd/Even Page Select
  // Bit  4            - unused
  // Bits 3-2          - Clock Select
  // Bit  1 (RAM En.)  - RAM Enable
  // Bit  0 (I/OAS)    - Input/Output Address Select
  //                     (0 = 0x3BX CRTC port, 1 = 0x3DX CRTC port)
  ca = (inportb (VGA_MISC_READ_PORT) & 1) ? VGA_CRTC_ADDR_PORT
                                          : VGA_CRTC_ADDR_PORT - 32;
  cd = ca + 1;
  s1 = ca + 6;

  orig_fs = _fargetsel ();
  _farsetsel (_dos_ds);

  BDA_set_active_6845_CRTC_port (ca);

  /*** CRTC ***/

  // Clear write protection of CRTC registers 0-7
  outportb (ca, 0x11); // Vertical Retrace End Register
  // Bit  7 (Protect)   - CRTC Registers 0-7 Protect Enable (0 = disable)
  // Bit  6 (Bandwidth) - Memory Refresh Bandwidth
  // Bits 5-4           - unused
  // Bits 3-0           - Vertical Retrace End
  outportb (cd, inportb (cd) & 0x7F);

  for (i = 0; i < 25; i++)
    outportw (ca, i | (mode->CRTC[i] << 8));

  // Reset
  inportb (s1);

  /*** Attribute Controller ***/

  for (i = 0; i < 20; i++) {
    outportb (VGA_ATTR_WRITE_PORT, i);
    outportb (VGA_ATTR_WRITE_PORT, mode->Attr[i]);
  }

  /*** Graphics Controller ***/

  for (i = 0; i < 9; i++)
    outportw (VGA_GRAPH_ADDR_PORT, i | (mode->Graph[i] << 8));

  // Enable video output
  outportb (VGA_ATTR_WRITE_PORT, 0x20); // Attribute Address Register
  // Bits 7-6 - unused
  // Bit  5   - Palette Address Source:
  //            0 = load color values to the registers in the internal palette
  //            1 = normal operation of the attribute controller
  // Bits 4-0 - Attribute Address

  /*** Update internal state ***/

  v_mode = 3; // standard 80x25
  v_font = mode->font_height;
  v_cols = mode->columns;
  v_rows = mode->rows;
  v_page = 0;
  v_regen_size = mode->regen_size;
  v_ofs = 0;
  v_seg = VGA_SEG_B800;
  v_curpos = 0;
  v_curshape = (mode->CRTC[0x0B] + (mode->CRTC[0x0A] << 8)) & 0x1F1F;

  /*** Update BIOS Data Area ***/

  BDA_set_active_video_mode (v_mode);
  BDA_set_screen_character_height (v_font);
  BDA_set_screen_text_columns (v_cols);
  BDA_set_screen_text_rows (v_rows);
  BDA_set_active_video_page (v_page);
  BDA_set_video_regen_buffer_size (v_regen_size);
  BDA_set_video_page_offset (v_ofs);
  for (i = 0; i < 8; i++)
    BDA_set_cursor_position (i, 0, 0);
  BDA_set_cursor_shape (v_curshape >> 8, v_curshape & 0xFF);

  if (iflag)
    enable ();

  _farsetsel (orig_fs);

  switch (mode->font_height) {
  case 8:
    VBIOS_load_ROM_font_8x8 (0, 0);
    break;
  case 14:
    VBIOS_load_ROM_font_8x14 (0, 0);
    break;
  case 16:
    VBIOS_load_ROM_font_8x16 (0, 0);
    break;
  default:
    break;
  }
}
