// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#define HAVE_INLINE_bit_test 1

__inline__ bool bit_test (const uint8_t *s, uint32_t value) {
  const uint8_t *mem = s;
  uint32_t reg = value;
  bool r;

  __asm__ __volatile__ (
    "btl %[r],(%[m])"
    : "=@ccb" (r)
    : [r] "r" (reg), [m] "r" (mem)
  );

  return r;
}
