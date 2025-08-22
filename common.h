// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef COMMON_H
#define COMMON_H

#include "defines.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define MK_UINT8(a,b,c,d,e,f,g,h) (a+b*2+c*4+d*8+e*16+f*32+g*64+h*128)
#define MK_UINT32(a,b,c,d) (a+((b)<<8)+((c)<<16)+((d)<<24))

typedef uint8_t CharSet_t[32];

#if i386||_X86_||__x86_64

#if DEBUG

void bit_set (uint8_t *s, uint32_t value);
void bit_clear (uint8_t *s, uint32_t value);
bool bit_test (const uint8_t *s, uint32_t value);

#else // !DEBUG

// Inline versions
#include "common/x86/bit_set.h"
#include "common/x86/bit_clear.h"
#include "common/x86/bit_test.h"

#endif // !DEBUG

#else // !(i386||_X86_||__x86_64)

void bit_set (uint8_t *s, uint32_t value);
void bit_clear (uint8_t *s, uint32_t value);
bool bit_test (const uint8_t *s, uint32_t value);

#endif // !(i386||_X86_||__x86_64)

void bit_set_range (uint8_t *s, uint32_t start, uint32_t end);
void bit_clear_range (uint8_t *s, uint32_t start, uint32_t end);

void *memsetw (void *s, int c, size_t n);

#endif // !defined(COMMON_H)
