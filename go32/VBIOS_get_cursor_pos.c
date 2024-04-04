// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ uint16_t VBIOS_get_cursor_pos (uint8_t page) {
  uint16_t d;

  __asm__ __volatile__ (
    "mov $3,%%ah\n\t"
    "mov %[page],%%bh\n\t"
    "int $0x10"
    // CH,CL = start,end scan line
    // DH,DL = row,column
    : "=d" (d)
    : [page] "qQmi" (page)
    : "ah", "bh", "cx", "cc", "memory"
  );

  return d;
}
