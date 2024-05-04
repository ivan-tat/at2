// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef COMMON_H
#define COMMON_H

#include "defines.h"

#include <stdbool.h>
#include <stdint.h>
#if USE_FPC
#include "pascal/string.h"
#else // !USE_FPC
#include <string.h>
#endif // !USE_FPC

#define MK_UINT8(a,b,c,d,e,f,g,h) (a+b*2+c*4+d*8+e*16+f*32+g*64+h*128)
#define MK_UINT32(a,b,c,d) (a+((b)<<8)+((c)<<16)+((d)<<24))

typedef uint8_t CharSet_t[32];

__inline__ void Set_Insert (uint8_t *s, uint8_t value) {
  s[value / 8] |= 1 << (value % 8);
}

__inline__ void Set_InsertRange (uint8_t *s, uint8_t start, uint8_t end) {
  size_t i;

  for (i = start; i <= end; i++)
    s[i / 8] |= 1 << (i % 8);
}

__inline__ void Set_Remove (uint8_t *s, uint8_t value) {
  s[value / 8] &= ~(1 << (value % 8));
}

__inline__ void Set_RemoveRange (uint8_t *s, uint8_t start, uint8_t end) {
  size_t i;

  for (i = start; i <= end; i++)
    s[i / 8] &= ~(1 << (i % 8));
}

__inline__ bool Set_Contains (const uint8_t *s, uint8_t value) {
  return (s[value / 8] & (1 << (value % 8))) ? true : false;
}

void *memsetw (void *s, int c, size_t n);

#endif // !defined(COMMON_H)
