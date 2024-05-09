// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#define HAVE_INLINE_bit_set 1

__inline__ void bit_set (uint8_t *s, uint32_t value) {
  uint8_t *mem = s;
  uint32_t reg = value;

  __asm__ __volatile__ (
    "btsl %[r],(%[m])"
    :
    : [r] "r" (reg), [m] "r" (mem)
    : "cc", "memory"
  );
}
