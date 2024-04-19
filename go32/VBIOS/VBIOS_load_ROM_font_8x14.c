// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// font: 0..3 for EGA, 0..7 for VGA
static __inline__ void VBIOS_load_ROM_font_8x14 (uint8_t font, bool enable) {
  uint16_t a = enable ? 0x1111 : 0x1101;

  __asm__ __volatile__ (
    "int $0x10"
    :
    : "a" (a), "b" (font)
    : "cc", "memory"
  );
}
