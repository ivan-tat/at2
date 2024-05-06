// This file is part of Adlib Tracker II (AT2).
//
// Ported from `adt2play/txtscrio.pas'.
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int VGA_SaveVideoState (VGAVideoState_t *vs, bool save_screen) {
  int err = 0;
  VGAVideoState_t _vs;
  int32_t data_sel = -1;
  __dpmi_regs regs;

  _vs.data = NULL;
  _vs.screen = NULL;

  regs.d.res = 0;
  regs.x.sp = 0;
  regs.x.ss = 0;
  regs.x.ax = 0x1C00; // VGA Query Size of State-Save Buffer
  regs.x.cx = 7; // video HW state info, VBIOS data area info, DAC state & color registers
  if (!__dpmi_simulate_real_mode_interrupt (0x10, &regs)) {
    int32_t data_seg = -1;
    uint16_t orig_fs;

    _vs.data_size = (regs.h.al == 0x1C) ? regs.x.bx * 64 : 0;
    if (_vs.data_size) {
      // Allocate state-buffer in DOS
      data_seg = __dpmi_allocate_dos_memory (_vs.data_size / 16, &data_sel);
      if (data_seg >= 0) {
        // Allocate state-buffer in DPMI
        _vs.data = malloc (_vs.data_size);
        if (!_vs.data) {
          // Skip state-buffer saving
          _vs.data_size = 0;
          __dpmi_free_dos_memory (data_sel);
          data_sel = -1;
          data_seg = -1;
        }
      } else
        _vs.data_size = 0; // Skip state-buffer saving
    }

    VBIOS_get_video_mode (&_vs.mode, &_vs.page);

    orig_fs = _fargetsel ();
    _farsetsel (_dos_ds);

    _vs.font = BDA_get_screen_character_height ();
    _vs.cols = BDA_get_screen_text_columns ();
    _vs.rows = BDA_get_screen_text_rows ();
    _vs.regen_size = BDA_get_video_regen_buffer_size ();
    _vs.ofs = BDA_get_video_page_offset ();
    _vs.seg = (_vs.mode == 7) ? VGA_SEG_B000 : VGA_SEG_B800;

    _farsetsel (orig_fs);

    _vs.curpos = VBIOS_get_cursor_pos (_vs.page);
    _vs.curshape = VGA_GetCursorShape ();

    if (save_screen) {
      // Save screen data
      _vs.screen = malloc (_vs.regen_size);
      if (_vs.screen)
        dosmemget (_vs.seg * 16 + _vs.ofs, _vs.regen_size, _vs.screen);
    }

    if (_vs.data) {
      bool ok;
      uint8_t BDA_backup[168] __UNUSED;

      if (false) {
        // Save BIOS Data Area
        dosmemget (0x400, sizeof (BDA_backup), &BDA_backup);
      }

      // Save video state to state-buffer in DOS
      regs.d.res = 0;
      regs.x.sp = 0;
      regs.x.ss = 0;
      regs.x.ax = 0x1C01; // VGA Save Video State
      regs.x.cx = 7; // video HW state info, VBIOS data area info, DAC state & color registers
      regs.x.bx = 0; // ES:BX = buffer to save state data
      regs.x.es = data_seg;
      ok = !__dpmi_simulate_real_mode_interrupt (0x10, &regs);

      if (false) {
        // Restore BIOS Data Area
        dosmemput (&BDA_backup, sizeof (BDA_backup), 0x400);
      }

      if (ok) {
        // Copy state-buffer from DOS to DPMI
        dosmemget (data_seg * 16, _vs.data_size, _vs.data);
      } else {
        // Free DPMI state-buffer
        free (_vs.data);
        _vs.data = NULL;
        _vs.data_size = 0;
      }
    }
  } else
    err = -1;

  if (!err)
    memcpy (vs, &_vs, sizeof (*vs));
  else
    VGA_FreeVideoState (&_vs);

  // Free DOS state-buffer
  if (data_sel >= 0)
    __dpmi_free_dos_memory (data_sel);

  return err;
}
