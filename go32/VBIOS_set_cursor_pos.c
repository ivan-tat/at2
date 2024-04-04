// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ void VBIOS_set_cursor_pos (uint8_t page, uint8_t x, uint8_t y) {
  __asm__ __volatile__ (
    "mov $2,%%ah\n\t"
    "mov %[page],%%bh\n\t"
    "mov %[x],%%dl\n\t"
    "mov %[y],%%dh\n\t"
    "int $0x10"
    :
    : [page] "qQmi,mi,mi" (page),
      [x]    "mi,qQmi,mi" (x),
      [y]    "mi,mi,qQmi" (y)
    : "ah", "bh", "dx", "cc", "memory"
  );
}
