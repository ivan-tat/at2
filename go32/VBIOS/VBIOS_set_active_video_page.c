// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ void VBIOS_set_active_video_page (uint8_t page) {
  uint16_t a = 0x500 + page; // MSB = 5 (select video page)

  __asm__ __volatile__ (
    "int $0x10"
    :
    : "a" (a)
    : "cc", "memory"
  );
}
