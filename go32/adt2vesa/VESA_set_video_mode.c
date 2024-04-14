// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// mode: Desired Mode to set
//   Bits 0-8   = Mode number
//   Bits 9-10  = Reserved (must be 0)
//   Bit  11    = 0 - Use current default refresh rate
//              = 1 - Use user specified CRTC values for refresh rate (VBE 3+)
//   Bits 12-13 = Reserved for VBE/AF (must be 0)
//   Bit  14    = 0 - Use windowed frame buffer model
//              = 1 - Use linear/flat frame buffer model (VBE 2+)
//   Bit  15    = 0 - Clear display memory
//              = 1 - Don't clear display memory
static __inline__ int VESA_set_video_mode (uint16_t mode) {
  uint16_t a = 0x4F02;
  uint16_t ra;

  __asm__ __volatile__ (
    // ES:DI = Pointer to CRTCInfoBlock structure (VBE 3+)
    "int $0x10"
    : "=a" (ra)
    : "a" (a), "b" (mode)
    : "cc", "memory"
  );

  return _VESA_CheckStatus (ra);
}
