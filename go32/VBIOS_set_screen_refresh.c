// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ void VBIOS_set_screen_refresh (bool on) {
  if (on) {
    __asm__ __volatile__ (
      "mov $0x1200,%%ax\n\t"
      "mov $0x36,%%bl\n\t"
      "int $0x10\n\t"
      :
      :
      : "ax", "bl", "cc", "memory"
    );
  } else {
    __asm__ __volatile__ (
      "mov $0x1201,%%ax\n\t"
      "mov $0x36,%%bl\n\t"
      "int $0x10\n\t"
      :
      :
      : "ax", "bl", "cc", "memory"
    );
  }
}
