// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_MakeCustomTextModeEx (const VGARegister_t *regs, size_t count,
                               uint8_t columns, uint8_t rows) {
  bool iflag;

  iflag = disable ();

  // Clear write protection of CRTC registers 0-7
  outportb (VGA_CRTC_ADDR_PORT, 0x11); // Vertical Retrace End Register
  // Bit 7 (Protect)   - CRTC Registers 0-7 Protect Enable (0 = disable)
  // Bit 6 (Bandwidth) - Memory Refresh Bandwidth
  // Bits 5-4          - unused
  // Bits 3-0          - Vertical Retrace End
  outportb (VGA_CRTC_DATA_PORT, inportb (VGA_CRTC_DATA_PORT) & 0x7F);

  while (count) {
    switch (regs->port) {
    case VGA_ATTR_WRITE_PORT:
      inportb (VGA_STATUS_1_PORT);
      outportb (VGA_ATTR_WRITE_PORT, regs->idx | 0x20);
      outportb (VGA_ATTR_WRITE_PORT, regs->val);
      break;
    case VGA_MISC_WRITE_PORT:
    case VGA_ENABLE_PORT:
      outportb (regs->port, regs->val);
      break;
    case VGA_SEQ_ADDR_PORT:
    case VGA_GRAPH_ADDR_PORT:
    case VGA_CRTC_ADDR_PORT:
    default:
      outportb (regs->port, regs->idx);
      outportb (regs->port + 1, regs->val);
      break;
    }
    regs++;
    count--;
  }

  /*** Update internal state ***/

  v_mode = 3; // standard 80x25
  VGA_GetFontHeight ();
  v_cols = columns;
  v_rows = rows;
  v_page = 0;
  v_regen_size = (v_cols * v_rows * 2 + 0xFF) & ~0xFF;
  v_ofs = v_regen_size * v_page;
  v_seg = VGA_SEG_B800;
  VGA_GetCursorPosition ();
  VGA_GetCursorShape ();

  OnMakeTextMode (true);

  if (iflag)
    enable ();
}
