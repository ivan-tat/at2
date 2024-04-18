// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"
#if USE_FPC
#include "pascal.h"
#else // !USE_FPC
#include <dpmi.h>
#endif // !USE_FPC
#include "go32/dpmi.h"

#if USE_FPC

int32_t _go32_dpmi_unlock_code (void *lockaddr, uint32_t locksize) {
  return Pascal_unlock_code (lockaddr, locksize) ? 0 : -1;
}

int32_t _go32_dpmi_unlock_data (void *lockaddr, uint32_t locksize) {
  return Pascal_unlock_data (lockaddr, locksize) ? 0 : -1;
}

#else // !USE_FPC

int32_t _go32_dpmi_unlock_code (void *lockaddr, uint32_t locksize) {
  __dpmi_meminfo mi = {
    .size = locksize,
    .address = 0;
  };

  if (__dpmi_get_segment_base_address (_go32_my_cs, &mi.address))
    return -1;

  mi.address += (uint32_t) lockaddr;

  return __dpmi_unlock_linear_region (mi);
}

int32_t _go32_dpmi_unlock_data (void *lockaddr, uint32_t locksize) {
  __dpmi_meminfo mi = {
    .size = locksize,
    .address = 0;
  };

  if (__dpmi_get_segment_base_address (_go32_my_ds, &mi.address))
    return -1;

  mi.address += (uint32_t) lockaddr;

  return __dpmi_unlock_linear_region (mi);
}

#endif // !USE_FPC
