// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

void *memsetw (void *s, int c, size_t n) {
#if USE_FPC
  Pascal_FillWord (s, n, c);
#else // !USE_FPC
  uint16_t *p = s;

  while (n) {
    *p = c;
    p++;
    n--;
  }
#endif // !USE_FPC

  return s;
}
