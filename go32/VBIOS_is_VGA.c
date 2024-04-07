// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ bool VBIOS_is_VGA (void) {
  uint16_t a = 0x1A00;
  uint8_t ra, rb;

  __asm__ __volatile__ (
    "int $0x10"
    : "=a" (ra), "=b" (rb)
    : "a" (a)
    : "cc", "memory"
  );

  return ((ra == 0x1A) && (rb >= 7) && (rb < 0xFF));
}
