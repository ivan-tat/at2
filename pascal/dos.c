// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pascal/dos.h"

int custom_enable (void) {
  uint32_t ra;

  __asm__ __volatile__ (
    "pushf\n\t"
    "sti\n\t"
    "pop %%eax"
    : "=a" (ra)
    :
    : "cc", "memory"
  );

  return (ra >> 9) & 1;
}

int custom_disable (void) {
  uint32_t ra;

  __asm__ __volatile__ (
    "pushf\n\t"
    "cli\n\t"
    "pop %%eax"
    : "=a" (ra)
    :
    : "cc", "memory"
  );

  return (ra >> 9) & 1;
}
