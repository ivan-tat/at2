// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pascal/pc.h"

void inportsb (uint16_t port, uint8_t *buf, size_t len) {
  __asm__ __volatile__ (
    "mov %[port],%%edx\n\t"
    "mov %[buf],%%edi\n\t"
    "mov %[len],%%ecx\n\t"
    "cld\n\t"
    "rep insb (%%dx),%%es:(%%edi)"
    :
    : [port] "g" (port), [buf] "g" (buf), [len] "g" (len)
    : "ecx", "edx", "edi", "cc", "memory"
  );
}

void outportsb (uint16_t port, const uint8_t  *buf, size_t len) {
  __asm__ __volatile__ (
    "mov %[port],%%edx\n\t"
    "mov %[buf],%%esi\n\t"
    "mov %[len],%%ecx\n\t"
    "cld\n\t"
    "rep outsb %%ds:(%%esi),(%%dx)"
    :
    : [port] "g" (port), [buf] "g" (buf), [len] "g" (len)
    : "ecx", "edx", "esi", "cc", "memory"
  );
}
