// DJGPP support
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef GO32_DPMI_H
#define GO32_DPMI_H

#include <stdint.h>

// Helper functions that are missing in <dpmi.h>

int32_t _go32_dpmi_unlock_code (void *lockaddr, uint32_t locksize);
int32_t _go32_dpmi_unlock_data (void *lockaddr, uint32_t locksize);

#endif // !defined(GO32_DPMI_H)
