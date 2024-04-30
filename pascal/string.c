// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

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

int memcmp (const void *s1, const void *s2, size_t n) {
  int res = 0;
  const uint8_t *p1 = s1, *p2 = s2;

  while ((!res) && n) {
    if (*p1 < *p2)
      res = -1;
    else if (*p1 > *p2)
      res = 1;
    else {
      p1++;
      p2++;
      n--;
    }
  }

  return res;
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

String *AppendString (String *dest, const String *src, uint8_t n) {
  int_least16_t l1, l2;

  l1 = Length (dest);
  l2 = Length (src);

  if (l1 + l2 > n)
    l2 = n - l1;

  if (l2 > 0)
    Pascal_Move (GetStr (src), GetStr (dest) + l1, l2);

  SetLength (dest, l1 + l2);

  return dest;
}
