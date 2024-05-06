// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_MakeTextMode (uint8_t font, uint8_t cols, uint8_t rows,
                       uint16_t curshape) {

  uint16_t orig_fs;
  uint16_t ca, cd; // Controller's Address and Data ports
  bool iflag;

  orig_fs = _fargetsel ();
  _farsetsel (_dos_ds);

  ca = BDA_get_active_6845_CRTC_port (); // 0x3B4 (mono) or 0x3D4 (color)
  cd = ca + 1;

  iflag = disable ();

  /*** Reprogram CRT Controller ***/

  // Clear write protection of CRTC registers 0-7
  outportb (ca, 0x11); // Vertical Retrace End Register
  // Bit  7   - Protect: CRTC Registers 0-7 Protect Enable (0 = disable)
  // Bit  6   - Bandwidth: Memory Refresh Bandwidth
  // Bits 5-4 - unused
  // Bits 3-0 - Vertical Retrace End
  outportb (cd, inportb (cd) & 0x7F);

  // Set maximum scan line to character height
  outportb (ca, 9); // Maximum Scan Line Register
  // Bit  7   - SD: Scan Doubling
  // Bit  6   - LC9: Line Compare (bit 9)
  // Bit  5   - SVB9: Start Vertical Blanking (bit 9)
  // Bits 4-0 - Maximum Scan Line (character height - 1)
  outportb (cd, (inportb (cd) & ~0x1F) | ((font - 1) & 0x1F));

  // Set cursor start scan line
  outportw (ca, 0x0A | (curshape & 0x1F00)); // Cursor Start Register
  // Bits 7-6 - unused
  // Bit  5   - Cursor Disable: 0 = enable, 1 = disable
  // Bits 4-0 - Cursor Scan Line Start: 0-31

  // Set cursor end scan line
  outportw (ca, 0x0B | ((curshape & 0x1F) << 8)); // Cursor End Register
  // Bit  7   - unused
  // Bits 6-5 - Cursor Skew: different behavior (better set to 0)
  // Bits 4-0 - Cursor Scan Line End: 0-31

  // Set word mode
  outportb (ca, 0x17); // Mode control register
  // Bit 7 - SE: Sync Enable
  // Bit 6 - Word/Byte: Word/Byte Mode Select (0 = word, 1 = byte)
  // Bit 5 - AW: Address Wrap Select
  // Bit 4 - unused
  // Bit 3 - DIV2: Divide Memory Address clock by 2
  // Bit 2 - SLDIV: Divide Scan Line clock by 2
  // Bit 1 - MAP14: Map Display Address 14
  // Bit 0 - MAP13: Map Display Address 13
  outportb (cd, inportb (cd) & ~0x40);

  // Enable write protection of CRTC registers 0-7
  outportb (ca, 0x11); // Vertical Retrace End Register
  // Bit  7   - Protect: CRTC Registers 0-7 Protect Enable (0 = disable)
  // Bit  6   - Bandwidth: Memory Refresh Bandwidth
  // Bits 5-4 - unused
  // Bits 3-0 - Vertical Retrace End
  outportb (cd, inportb (cd) | 0x80);

  /*** Reprogram Sequencer ***/

  // Make planes 2 and 3 write protected
  outportw (VGA_SEQ_ADDR_PORT, 0x0302); // Map Mask Register
  // Bits 7-4 - unused
  // Bits 3-0 - Memory Plane 3-0 Write Enable (0 = disable)

  // Set odd/even mode, reset chain 4, more than 64 KiB
  outportw (VGA_SEQ_ADDR_PORT, 0x0204); // Sequencer Memory Mode Register
  // Bits 7-4 - unused
  // Bit  3   - Chain 4: Chain 4 Enable (0 = disable)
  // Bit  2   - O/E Dis.: Odd/Even Host Memory Write Adressing Disable
  //            (0 = odd/even addressing, 1 = normal addressing)
  // Bit  1   - Ext. Mem: Extended Memory (1 = enables 64-256 KiB VRAM)
  // Bit  0   - unused

  /*** Reprogram Graphics Controller ***/

  // Set write mode 0, read mode 0, odd/even addressing
  outportw (VGA_GRAPH_ADDR_PORT, 0x1005); // Graphics Mode Register
  // Bit  7   - unused
  // Bit  6   - Shift256: 256-Color Shift Mode
  // Bit  5   - Shift Reg.: Shift Register Interleave Mode
  // Bit  4   - Host O/E: Host Odd/Even Memory Read Addressing Enable
  //            (1 = odd/even addressing)
  // Bit  3   - Read Mode
  // Bit  2   - unused
  // Bits 1-0 - Write Mode (0-3)

  // Set memory map to 0xB8000h-0xBFFFF (32 KiB), set text mode, set odd/even
  outportb (VGA_GRAPH_ADDR_PORT, 6); // Miscellaneous Graphics Register
  // Bits 7-4 - unused
  // Bits 3-2 - Memory Map Select
  //            00 = 0xA0000-0xBFFFF (128 KiB region)
  //            01 = 0xA0000-0xAFFFF (64 KiB region)
  //            10 = 0xB0000-0xB7FFF (32 KiB region)
  //            11 = 0xB8000-0xBFFFF (32 KiB region)
  // Bit 1    - Chain O/E: Chain Odd/Even Enable
  // Bit 0    - Alpha Dis.: Alphanumeric Mode Disable
  outportb (VGA_GRAPH_DATA_PORT,
            (inportb (VGA_GRAPH_DATA_PORT) & 0xF0) | 0x0E);

  /*** Reprogram Attribute Controller ***/

  // Reset
  // Miscellaneous Output Register:
  // Bit  7   - VSYNCP: Vertical Sync Polarity
  // Bit  6   - HSYNCP: Horizontal Sync Polarity
  // Bit  5   - O/E Page: Odd/Even Page Select
  // Bit  4   - unused
  // Bits 3-2 - Clock Select
  // Bit  1   - RAM En.: RAM Enable
  // Bit  0   - I/OAS: Input/Output Address Select
  //            (0 = 0x3BX CRTC port, 1 = 0x3DX CRTC port)
  inportb ((inportb (VGA_MISC_READ_PORT) & 1) ? VGA_STATUS_1_PORT
                                              : (VGA_STATUS_1_PORT - 32));

  // Attribute Address Register:
  // Bits 7-6 - unused
  // Bit  5   - PAS: Palette Address Source
  //            0 = load color values to the registers in the internal palette
  //            1 = normal operation of the attribute controller
  // Bits 4-0 - Attribute Address
  outportb (VGA_ATTR_WRITE_PORT, 0x10); // Select mode register

  // Attribute Mode Control Register (Index 10h)
  // Bit 7 (P54S)  - Palette Bits 5-4 Select
  // Bit 6 (8BIT)  - 8-bit Color Enable
  // Bit 5 (PPM)   - Pixel Panning Mode:
  //                 0 = normal operation
  //                 1 = bottom part is displayed as if `Pixel Shift Count'
  //                     and `Byte Panning' fields are set to 0
  // Bit 4         - unused
  // Bit 3 (BLINK) - Blink Enable
  // Bit 2 (LGE)   - Line Graphics Enable
  // Bit 1 (MONO)  - Monochrome Emulation
  // Bit 0 (ATGE)  - Attribute Controller Graphics Enable
  outportb (VGA_ATTR_WRITE_PORT, 0x00); // Set text mode [bit 0=0]

  outportb (VGA_ATTR_WRITE_PORT, 0x20); // Turn screen on again

  /*** Update internal state ***/

  v_font = font;
  v_cols = cols;
  v_rows = rows;
  v_page = 0;
  v_regen_size = cols * rows * 2;
  v_ofs = 0;
  v_seg = VGA_SEG_B800;
  v_curpos = 0;
  v_curshape = curshape & 0x1F1F;

  /*** Update BIOS Data Area ***/

  BDA_set_screen_character_height (v_font);
  BDA_set_screen_text_columns (v_cols);
  BDA_set_screen_text_rows (v_rows);
  BDA_set_active_video_page (0);
  BDA_set_video_regen_buffer_size (v_regen_size);
  BDA_set_video_page_offset (v_ofs);
  BDA_set_cursor_position (0, 1, 0); // To force update by VBIOS later
  BDA_set_cursor_shape (v_curshape & 0xFF, v_curshape >> 8);

  if (iflag)
    enable ();

  _farsetsel (orig_fs);

  VBIOS_set_cursor_pos (0, 0, 0); // Finally set it
}
