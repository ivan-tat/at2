// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pascal.h"
#include "pascal/string.h"

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

size_t strlen (const char *s) {
  if (s)
    return Pascal_strlen (s);
  else
    return 0;
}

String *StrToString (String *dest, const char *src, uint8_t n) {
  size_t l = src ? Pascal_strlen (src) : 0;

  if (l > n)
     l = n;
  if (l)
    Pascal_Move (src, dest+1, l);

  SetLength (dest, l);

  return dest;
}

String *CopyString (String *dest, const String *src, uint8_t n) {
  uint8_t l = Length (src);

  if (l < n)
    n = l;

  Pascal_Move (src, dest, n+1);

  if (l > n)
    SetLength (dest, n);

  return dest;
}
