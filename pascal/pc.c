// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pascal/pc.h"

void inportsb (uint16_t port, uint8_t *buf, size_t len) {
  __asm__ __volatile__ (
    "cld\n\t"
    "rep insb (%%dx),%%es:(%%edi)"
    :
    : "d" (port), "D" (buf), "c" (len)
    : "cc", "memory"
  );
}

void outportsb (uint16_t port, const uint8_t  *buf, size_t len) {
  __asm__ __volatile__ (
    "cld\n\t"
    "rep outsb %%ds:(%%esi),(%%dx)"
    :
    : "d" (port), "S" (buf), "c" (len)
    : "cc", "memory"
  );
}
