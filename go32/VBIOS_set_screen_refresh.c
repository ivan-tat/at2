// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ void VBIOS_set_screen_refresh (bool on) {
  uint16_t a = on ? 0x1200 : 0x1201;
  uint8_t b = 0x36;

  __asm__ __volatile__ (
    "int $0x10"
    :
    : "a" (a), "b" (b)
    : "cc", "memory"
  );
}
