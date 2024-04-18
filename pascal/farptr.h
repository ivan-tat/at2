// DJGPP support
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef PASCAL_FARPTR_H
#define PASCAL_FARPTR_H

#include <stdint.h>

__inline__ uint16_t _fargetsel (void) {
  uint16_t sel;

  __asm__ __volatile__ (
    "movw %%fs,%w[sel]"
    : [sel] "=r" (sel)
  );

  return sel;
}

__inline__ void _farsetsel (uint16_t sel) {
  __asm__ __volatile__ (
    "movw %w[sel],%%fs"
    :
    : [sel] "rm" (sel)
  );
}

__inline__ uint8_t _farnspeekb (uint32_t ofs) {
  uint8_t x;

  __asm__ __volatile__ (
    ".byte 0x64\n\t"
    "movb (%k[ofs]),%b[x]"
    : [x] "=q" (x)
    : [ofs] "r" (ofs)
  );

  return x;
}

__inline__ uint16_t _farnspeekw (uint32_t ofs) {
  uint16_t x;

  __asm__ __volatile__ (
    ".byte 0x64\n\t"
    "movw (%k[ofs]),%w[x]"
    : [x] "=r" (x)
    : [ofs] "r" (ofs)
  );

  return x;
}

__inline__  uint32_t _farnspeekl (uint32_t ofs) {
  uint32_t x;

  __asm__ __volatile__ (
    ".byte 0x64\n\t"
    "movl (%k[ofs]),%k[x]"
    : [x] "=r" (x)
    : [ofs] "r" (ofs)
  );

  return x;
}

__inline__ void _farnspokeb (uint32_t ofs, uint8_t x) {
  __asm__ __volatile__ (
    ".byte 0x64\n\t"
    "movb %b[x],(%k[ofs])"
    :
    : [x] "qi" (x), [ofs] "r" (ofs)
  );
}

__inline__ void _farnspokew (uint32_t ofs, uint16_t x) {
  __asm__ __volatile__ (
    ".byte 0x64\n\t"
    "movw %w[x],(%k[ofs])"
    :
    : [x] "ri" (x), [ofs] "r" (ofs)
  );
}

__inline__ void _farnspokel (uint32_t ofs, uint32_t x) {
  __asm__ __volatile__ (
    ".byte 0x64\n\t"
    "movl %k[x],(%k[ofs])"
    :
    : [x] "ri" (x), [ofs] "r" (ofs)
  );
}

#endif // !defined(PASCAL_FARPTR_H)
