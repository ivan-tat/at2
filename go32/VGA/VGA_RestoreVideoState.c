// This file is part of Adlib Tracker II (AT2).
//
// Ported from `adt2play/txtscrio.pas'.
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_RestoreVideoState (VGAVideoState_t *vs, bool restore_screen) {
  int32_t data_sel = -1, data_seg = -1;
  uint16_t orig_fs;

  if (vs->data && vs->data_size) {
    // Allocate state-buffer in DOS
    data_seg = __dpmi_allocate_dos_memory (vs->data_size / 16, &data_sel);
    if (data_seg >= 0) {
      // Copy state-buffer from DPMI to DOS
      dosmemput (vs->data, vs->data_size, data_seg * 16);
    }
  }

  VBIOS_set_video_mode (vs->mode);

  switch (vs->font) {
  case 8:
    VBIOS_load_ROM_font_8x8 (0, true);
    break;
  case 14:
    VBIOS_load_ROM_font_8x14 (0, true);
    break;
  case 16:
  default:
    VBIOS_load_ROM_font_8x16 (0, true);
    break;
  }

  orig_fs = _fargetsel ();

  _farsetsel (_dos_ds);
  BDA_set_screen_text_columns (vs->cols);
  BDA_set_screen_text_rows (vs->rows);
  BDA_set_video_regen_buffer_size (vs->regen_size);

  VBIOS_set_active_video_page (vs->page);

  _farsetsel (_dos_ds);
  BDA_set_video_page_offset (vs->ofs);

  _farsetsel (orig_fs);

  VBIOS_set_cursor_pos (vs->page, vs->curpos & 0xFF, vs->curpos >> 8);
  VGA_SetCursorShape (vs->curshape);

  if (data_seg >= 0) {
    uint8_t BDA_backup[168] __UNUSED;
    __dpmi_regs regs;

    if (false) {
      // Save BIOS Data Area
      dosmemget (0x400, sizeof (BDA_backup), &BDA_backup);
    }

    // Restore video state from state-buffer in DOS
    regs.d.res = 0;
    regs.x.sp = 0;
    regs.x.ss = 0;
    regs.x.ax = 0x1C02; // VGA Restore Video State
    regs.x.cx = 7; // video HW state info, VBIOS data area info, DAC state & color registers
    regs.x.bx = 0; // ES:BX = buffer to read state data
    regs.x.es = data_seg;
    __dpmi_simulate_real_mode_interrupt (0x10, &regs);

    if (false) {
      // Restore BIOS Data Area
      dosmemput (&BDA_backup, sizeof (BDA_backup), 0x400);
    }

    // Free DOS state-buffer
    __dpmi_free_dos_memory (data_sel);
  }

  // Restore screen data
  if (restore_screen && vs->screen)
    dosmemput (vs->screen, vs->regen_size, vs->seg * 16 + vs->ofs);

  v_mode = vs->mode;
  v_font = vs->font;
  v_cols = vs->cols;
  v_rows = vs->rows;
  v_page = vs->page;
  v_regen_size = vs->regen_size;
  v_ofs = vs->ofs;
  v_seg = vs->seg;
  v_curpos = vs->curpos;
  v_curshape = vs->curshape;
}
