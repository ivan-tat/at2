// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define MK_UINT32(a,b,c,d) (a+((b)<<8)+((c)<<16)+((d)<<24))

void *memsetw (void *s, int c, size_t n);

#endif // !defined(COMMON_H)
