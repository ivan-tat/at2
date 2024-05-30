// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int32_t SensitiveScan (const void *buf, ssize_t skip, ssize_t size,
                       const String *str) {

  ssize_t len = Length (str), tail = size - skip;
  const unsigned char *D = (unsigned char *)buf + skip;

  if (tail) {
    if (!len)
      return (intptr_t)D - (intptr_t)buf - 1;

    if (len == 1) {
      bool f = true;

      do {
        if (!tail)
          break;
        tail--;

        f = (GetStr (str))[0] == *D;
        D++;
      } while (!f);

      if (f)
        return (intptr_t)D - (intptr_t)buf - 1;
    } else {
      ssize_t last = len - 1;

      if (len <= tail) {
        ssize_t n = tail - len + 2;
        const unsigned char *S = GetStr (str) + 1;

        do {
          bool f;
          ssize_t count;

          n--;
          f = !n;

          do {
            if (!n)
              break;
            n--;

            f = (GetStr (str))[0] == *D;
            D++;
          } while (!f);

          count = last;
          if (!f)
            break;

          // f=true
          do {
            if (!count)
              break;
            count--;

            f = *S == *D;
            S++;
            D++;
          } while (f);

          if (f)
            return (intptr_t)D - (intptr_t)buf - last - 1;

          S += count - last;
          D += count - last + 1;
        } while (n);
      }
    }
  }

  return -1;
}
