// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ void VBIOS_get_video_mode (uint8_t *mode, uint8_t *page) {
  uint8_t ra;
  uint16_t rb;

  __asm__ __volatile__ (
    "mov $0x0F,%%ah\n\t"
    "int $0x10"
    : "=a" (ra), "=b" (rb)
    :
    : "cc", "memory"
  );

  *mode = ra & 0x7F;
  *page = rb >> 8;
}
