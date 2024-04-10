// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ int VESA_set_video_mode (uint16_t mode) {
  uint16_t a = 0x4F02;
  uint16_t ra;

  __asm__ __volatile__ (
    "int $0x10"
    : "=a" (ra)
    : "a" (a), "b" (mode)
    : "cc", "memory"
  );

  return _VESA_CheckStatus (ra);
}
