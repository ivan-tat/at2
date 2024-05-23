// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pascal/pc.h"
#include "pascal/dpmi.h"

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

int custom_getkey (void) {
  __dpmi_regs regs;

  regs.d.res = 0;
  regs.x.sp = 0;
  regs.x.ss = 0;
  regs.h.ah = 0x10; // Read extended keyboard input
  __dpmi_simulate_real_mode_interrupt (0x16, &regs);
  // Output: al = ASCII character code or extended ASCII keystroke
  //         ah = scan code

  if ((regs.h.al == 0)
  ||  ((regs.h.al == 0xE0) && regs.h.ah))
    return regs.h.ah | 0x100;
  else
    return regs.h.al;
}
