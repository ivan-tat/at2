// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pascal.h"
#include "pascal/dpmi.h"

int32_t __dpmi_allocate_dos_memory (int32_t paras, int32_t *ret) {
  int32_t value = Pascal_global_dos_alloc ((paras & 0xFFFF) * 16);

  if (value) {
    *ret = value & 0xFFFF; // selector
    return (value >> 16) & 0xFFFF; // segment
  } else {
    uint16_t err = __dpmi_error; // save original value
    __dpmi_regs regs;

    regs.h.ah = 0x48; // allocate DOS memory
    regs.x.bx = 0xFFFF; // memory size in paragraphs to allocate
    if (__dpmi_simulate_real_mode_interrupt (0x21, &regs))
      *ret = 0; // failed
    else
      *ret = regs.x.bx; // maximal available memory blocks in paragraphs

    __dpmi_error = err; // restore
    return -1;
  }
}

int32_t __dpmi_free_dos_memory (int32_t selector) {
  if (Pascal_global_dos_free (selector))
    return 0;
  else {
    __dpmi_error = 0xFFFF; // original error code is ignored by FPC
    return -1;
  }
}

int32_t __dpmi_simulate_real_mode_interrupt (int32_t vec, __dpmi_regs *regs) {
  if (Pascal_realintr (vec, regs))
    return 0;
  else {
    __dpmi_error = 0xFFFF; // original error code is ignored by FPC
    return -1;
  }
}

int32_t __dpmi_physical_address_mapping (__dpmi_meminfo *info) {
  int32_t addr = Pascal_get_linear_addr (info->address, info->size);

  if (!__dpmi_error) {
    info->address = addr;
    return 0;
  } else
    return -1;
}
