// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ void VESA_set_video_mode (uint16_t mode) {
  __asm__ __volatile__ (
    "mov $0x4F02,%%ax\n\t"
    "mov %[mode],%%bx\n\t"
    "int $0x10"
    :
    : [mode] "g" (mode)
    : "ax", "bx", "cc", "memory"
  );
}
