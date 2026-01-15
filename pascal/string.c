// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024-2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pascal/string.h"

void *custom_memchr (const void *s, char c, size_t n) {
  const char *p = s;

  while (n) {
    if (*p == c)
      return (void *) p;
    p++;
    n--;
  }

  return NULL;
}

int custom_memcmp (const void *s1, const void *s2, size_t n) {
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

void *custom_memcpy (void *dest, const void *src, size_t n) {
  Pascal_Move (src, dest, n);
  return dest;
}

void *custom_memmove (void *dest, const void *src, size_t n) {
  Pascal_Move (src, dest, n);
  return dest;
}

void *custom_memset (void *s, int c, size_t n) {
  Pascal_FillChar (s, n, c);
  return s;
}

size_t custom_strnlen (const char *s, size_t n) {
  if (s) {
    const char *p = s;
    size_t count = n;

    while (*p && count) {
      p++;
      count--;
    }

    return n - count;
  }

  return 0;
}

char *custom_stpncpy (char *dest, const char *src, size_t n) {
  size_t len = custom_strnlen (src, n);

  Pascal_FillChar (dest, n, 0);
  Pascal_Move (src, dest, len);
  return dest + len;
}

char *StringToStr (char *dest, const String *src, uint8_t n) {
  size_t l = src ? Length (src) : 0;

  if (l > n)
    l = n;
  if (l)
    memcpy (dest, GetStr (src), l);

  dest[l] = '\0';

  return dest;
}

String *StrToString (String *dest, const char *src, uint8_t n) {
  size_t l = src ? Pascal_strlen (src) : 0;

  if (l > n)
    l = n;
  if (l)
    Pascal_Move (src, GetStr (dest), l);

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

String_t Copy (const String *s, ssize_t index, ssize_t count) {
  String_t r;
  ssize_t len = Length (s);

  if (count < 0)
    count = 0;

  if (index > 1)
    index--;
  else
    index = 0;

  if (index > len)
    count = 0;
  else
    if (count > len - index)
      count = len - index;

  r.len = count;
  Pascal_Move (GetStr (s) + index, r.str, count);
  return r;
}

void Delete (String *s, ssize_t index, ssize_t count) {
  index--;
  if ((index >= 0) && (index < Length (s)) && (count > 0)) {
    if (count >= Length (s) - index)
      count = Length (s) - index;

    SetLength (s, Length (s) - count);

    if (index < Length (s)) {
      Pascal_Move (GetStr (s) + index + count,
                   GetStr (s) + index,
                   Length (s) - index);
    }
  }
}

void Insert (const String *src, String *s, ssize_t n, ssize_t index) {
  ssize_t srclen, indexlen;

  index--;
  if (index < 0)
    index = 0;

  if (index >= Length (s)) {
    index = Length (s);
    if (index >= n)
      return;
  }

  indexlen = Length (s) - index;
  srclen = Length (src);
  if ((ssize_t) Length (src) + Length (s) - n > 0) {
    ssize_t cut = Length (src) + Length (s) - n;

    if (cut > indexlen) {
      srclen -= cut - indexlen;
      indexlen = 0;
    } else
      indexlen -= cut;
  }

  Pascal_Move (GetStr (s) + index,
               GetStr (s) + index + srclen,
               indexlen);
  Pascal_Move (GetStr (src),
               GetStr (s) + index,
               srclen);
  SetLength (s, index + srclen + indexlen);
}

void InsertChr (char c, String *s, ssize_t n, ssize_t index) {
  ssize_t indexlen;

  index--;
  if (index < 0)
    index = 0;
  else if (index > Length (s)) {
    index = Length (s);
    if (index >= n)
      return;
  }

  indexlen = Length (s) - index;

  if (((ssize_t) Length (s) == n) && (indexlen > 0))
    indexlen--;

  Pascal_Move (GetStr (s) + index,
               GetStr (s) + index + 1,
               indexlen);
  (GetStr (s))[index] = c;
  SetLength (s, index + indexlen + 1);
}

ssize_t Pos (const String *substr, const String *s) {
  if (Length (substr)) {
    ssize_t maxlen = Length (s) - Length (substr), i = 0;
    const uint8_t *pc = GetStr (s);

    while (i <= maxlen) {
      i++;
      if (((GetStr (substr))[0] == *pc)
      &&  !custom_memcmp (GetStr (substr), pc, Length (substr)))
        return i;
      pc++;
    }
  }

  return 0;
}

ssize_t PosChr (char c, const String *s) {
  const uint8_t *pc = GetStr (s);
  ssize_t i;

  for (i = 1; i <= Length (s); i++) {
    if (*pc == c)
      return i;
    pc++;
  }

  return 0;
}

int CompareStrings (const String *s1, const String *s2) {
  size_t l1 = Length (s1);
  size_t l2 = Length (s2);
  size_t len = (l1 < l2) ? l1 : l2;

  for (size_t i = 1; i <= len; i++) {
    if (s1[i] > s2[i])
      return 1;
    else if (s1[i] < s2[i])
      return -1;
  }

  if (l1 > l2)
    return 1;
  else if (l1 < l2)
    return -1;
  else
    return 0;
}
