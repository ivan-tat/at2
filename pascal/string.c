// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pascal.h"
#include "string.h"

void *memcpy (void *dest, const void *src, size_t n) {
  Pascal_Move (src, dest, n);
  return dest;
}

void *memmove (void *dest, const void *src, size_t n) {
  Pascal_Move (src, dest, n);
  return dest;
}

void *memset (void *s, int c, size_t n) {
  Pascal_FillChar (s, n, c);
  return s;
}
