// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ bool VBIOS_is_VGA (void) {
  uint8_t a, b;

  __asm__ __volatile__ (
    "mov $0x1A00,%%ax\n\t"
    "int $0x10"
    : "=a" (a), "=b" (b)
    :
    : "cc", "memory"
  );

  return ((a == 0x1A) && (b >= 7) && (b < 0xFF));
}
