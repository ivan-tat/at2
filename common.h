// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024-2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef COMMON_H
#define COMMON_H

#include "defines.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// Endianness conversion
__inline__ uint16_t byteswap_uint16(uint16_t x) { return (x>>8)+(x<<8); }
__inline__ uint32_t byteswap_uint32(uint32_t x) { return (x>>24)+((x&0xFF0000)>>8)+((x&0xFF00)<<8)+(x<<24); }
#if i386||_X86_||__x86_64
#define uint16_LE(x) (x)
#define uint16_BE(x) byteswap_uint16(x)
#define uint32_LE(x) (x)
#define uint32_BE(x) byteswap_uint32(x)
#endif // i386||_X86_||__x86_64

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

// memory stream

typedef struct
{
  const void *buf;
  size_t size;
  const void *curptr, *endptr;
} mem_stream_t;

void set_mem_stream (mem_stream_t *dst, const void *buf, size_t size);
bool read_bytes (void *dst, size_t size, mem_stream_t *stream);
bool read_string (String *dst, size_t size, mem_stream_t *stream);

#endif // !defined(COMMON_H)
