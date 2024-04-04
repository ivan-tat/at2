// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// font: 0..3 for EGA, 0..7 for VGA
static __inline__ void VBIOS_load_ROM_font_8x8 (uint8_t font) {
  __asm__ __volatile__ (
    "mov $0x1102,%%ax\n\t"
    "mov %[font],%%bl\n\t"
    "int $0x10"
    :
    : [font] "g" (font)
    : "ax", "bl", "cc", "memory"
  );
}
