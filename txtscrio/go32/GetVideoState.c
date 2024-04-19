// This file is part of Adlib Tracker II (AT2).
//
// Ported from `adt2play/txtscrio.pas'.
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void GetVideoState (tVIDEO_STATE *data) {
#if !USE_FPC
  uint16_t orig_fs;
#endif // !USE_FPC
  uint8_t bios_data_backup[168];
  int32_t dos_sel, dos_seg;
  __dpmi_regs regs;

  VBIOS_get_video_mode (&v_mode, &DispPg);

  dosmemget (v_seg * 16 + v_ofs, MAX_SCREEN_MEM_SIZE, &data->screen);

#if !USE_FPC
  orig_fs = _fargetsel ();
  _farsetsel (_dos_ds);
#endif // !USE_FPC

  data->cursor = GetCursor ();
  data->font = BDA_get_screen_character_height ();
  data->v_seg = 0xB800;
  data->v_ofs = BDA_get_video_page_offset ();
  data->DispPg = DispPg;
  data->v_mode = v_mode;
  data->MaxCol = BDA_get_screen_text_columns ();
  data->MaxLn = BDA_get_screen_text_rows ();

#if !USE_FPC
  _farsetsel (orig_fs);
#endif // !USE_FPC

  dosmemget (0x400, sizeof (bios_data_backup), &bios_data_backup); // BIOS Data Area

  dos_seg = __dpmi_allocate_dos_memory ((sizeof (data->data) + 15) / 16,
                                        &dos_sel);
  if (dos_seg < 0) {
    // FIXME: no check for error in original code
  }

  regs.d.res = 0;
  regs.x.sp = 0;
  regs.x.ss = 0;
  regs.d.eax = 0x1C01; // VGA Save Video State
  regs.d.ecx = 7; // video HW state info, VBIOS data area info, DAC state & color registers
  regs.d.ebx = 0; // ES:BX = buffer to save state data
  regs.x.es = dos_seg;
  if (__dpmi_simulate_real_mode_interrupt (0x10, &regs)) {
    // FIXME: no check for error in original code
  }

  dosmemput (&bios_data_backup, sizeof (bios_data_backup), 0x400); // BIOS Data Area
  dosmemget (dos_seg * 16, sizeof (data->data), &data->data);
  __dpmi_free_dos_memory (dos_sel);
}
