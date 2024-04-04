// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ void VBIOS_set_video_mode (uint8_t mode, uint8_t page) {
  __asm__ __volatile__ (
    "xor %%eax,%%eax\n\t"
    "mov %[mode],%%al\n\t"
    "mov %[page],%%bh\n\t"
    "int $0x10"
    :
    : [mode] "qQmi,mi" (mode),
      [page] "mi,qQmi" (page)
    : "eax", "bh", "cc", "memory"
  );
}
