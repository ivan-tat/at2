// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pascal/dos.h"

int custom_enable (void) {
  int a;

  __asm__ __volatile__ (
    "pushf\n\t"
    "sti\n\t"
    "pop %%eax\n\t"
    "shr $9,%%eax\n\t"
    "and $1,%%eax"
    : "=a" (a)
    :
    : "cc", "memory"
  );

  return a;
}

int custom_disable (void) {
  int a;

  __asm__ __volatile__ (
    "pushf\n\t"
    "cli\n\t"
    "pop %%eax\n\t"
    "shr $9,%%eax\n\t"
    "and $1,%%eax"
    : "=a" (a)
    :
    : "cc", "memory"
  );

  return a;
}
