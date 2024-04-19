// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ void VBIOS_set_cursor_pos (uint8_t page, uint8_t x, uint8_t y) {
  uint16_t d = x | (y << 8);

  __asm__ __volatile__ (
    "mov $2,%%ah\n\t"
    "mov %[page],%%bh\n\t"
    "int $0x10"
    :
    : [page] "qQmi" (page), "d" (d)
    : "ah", "bh", "cc", "memory"
  );
}
