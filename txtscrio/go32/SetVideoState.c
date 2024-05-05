// This file is part of Adlib Tracker II (AT2).
//
// Ported from `adt2play/txtscrio.pas'.
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void SetVideoState (tVIDEO_STATE *data, bool restore_screen) {
#if !USE_FPC
  uint16_t orig_fs;
#endif // !USE_FPC
  uint8_t bios_data_backup[168];
  int32_t dos_sel, dos_seg;
  __dpmi_regs regs;

  VBIOS_set_video_mode (data->mode, data->page);

  dosmemget (0x400, sizeof (bios_data_backup), &bios_data_backup); // BIOS Data Area

  dos_seg = __dpmi_allocate_dos_memory ((sizeof (data->data) + 15) / 16,
                                        &dos_sel);
  if (dos_seg < 0) {
    // FIXME: no check for error in original code
  }

  dosmemput (&data->data, sizeof (data->data), dos_seg * 16);

  regs.d.res = 0;
  regs.x.sp = 0;
  regs.x.ss = 0;
  regs.d.eax = 0x1C02; // VGA Restore Video State
  regs.d.ecx = 7; // video HW state info, VBIOS data area info, DAC state & color registers
  regs.d.ebx = 0; // ES:BX = buffer to read state data
  regs.x.es = dos_seg;
  if (__dpmi_simulate_real_mode_interrupt (0x10, &regs)) {
    // FIXME: no check for error in original code
  }

  __dpmi_free_dos_memory (dos_sel);
  dosmemput (&bios_data_backup, sizeof (bios_data_backup), 0x400); // BIOS Data Area

  switch (data->font) {
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

  VBIOS_set_cursor_pos (data->page, data->curpos & 0xFF, data->curpos >> 8);
  VGA_SetCursorShape (data->curshape);

#if !USE_FPC
  orig_fs = _fargetsel ();
  _farsetsel (_dos_ds);
#endif // !USE_FPC

  BDA_set_screen_text_columns (data->cols);
  BDA_set_screen_text_rows (data->rows);
  BDA_set_video_regen_buffer_size (data->regen_size);
  BDA_set_video_page_offset (data->ofs);

#if !USE_FPC
  _farsetsel (orig_fs);
#endif // !USE_FPC

  v_mode = data->mode;
  v_font = data->font;
  v_cols = data->cols;
  v_rows = data->rows;
  v_page = data->page;
  v_regen_size = data->regen_size;
  v_ofs = data->ofs;
  v_seg = data->seg;
  v_curpos = data->curpos;
  v_curshape = data->curshape;

  if (restore_screen)
    dosmemput (&data->screen,
               v_regen_size <= sizeof (data->screen) ? v_regen_size : sizeof (data->screen),
               v_seg * 16 + v_ofs);

  MaxCol = v_cols;
  MaxLn = v_rows;
#if !ADT2PLAY
  virtual_cur_shape = v_curshape;
#endif // !ADT2PLAY
}
