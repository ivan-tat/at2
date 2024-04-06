// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// font: 0..7 for VGA
static __inline__ void VBIOS_load_ROM_font_8x16 (uint8_t font, bool enable) {
  uint16_t a = enable ? 0x1114 : 0x1104;

  __asm__ __volatile__ (
    "int $0x10"
    :
    : "a" (a), [font] "b" (font)
    : "cc", "memory"
  );
}
