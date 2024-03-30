// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"

#if USE_FPC
#include "pascal.h"
#endif
#include "common.h"

void *memsetw (void *s, int c, size_t n) {
#if USE_FPC
  Pascal_FillWord (s, n, c);
#else
  uint16_t *p = s;

  while (n) {
    *p = c;
    p++;
    n--;
  }
#endif

  return s;
}