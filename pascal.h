// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef PASCAL_H
#define PASCAL_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

typedef unsigned char Shortstring;

typedef Shortstring String;

#define Length(s) ((s)[0])
#define SetLength(s, l) (s)[0] = (l)
#define GetStr(s) (&(s)[1])

extern void Pascal_Halt (int32_t errnum);

extern void Pascal_FillChar (void *x, ssize_t count, uint8_t value);
extern void Pascal_FillWord (void *x, ssize_t count, uint16_t value);

extern void Pascal_Move (const void *src, void *dest, ssize_t n);

extern size_t Pascal_strlen (const char *s);

extern void Pascal_Delay (uint16_t ms);

extern float Pascal_Trunc_Single (float x);
extern double Pascal_Trunc_Double (double x);

extern void *Pascal_AllocMem (size_t size);
extern size_t Pascal_FreeMem (void *p);
extern void *Pascal_ReAllocMem (void **p, size_t size);

#if GO32

extern uint16_t Pascal_allocate_ldt_descriptors (uint16_t count);
extern bool Pascal_free_ldt_descriptor (uint16_t selector);
extern uint16_t Pascal_get_next_selector_increment_value (void);
extern bool Pascal_set_segment_base_address (uint16_t selector, int32_t address);
extern bool Pascal_set_segment_limit (uint16_t selector, int32_t limit);

extern void Pascal_dosmemget (uint16_t seg, uint16_t ofs, void *data, uint32_t count);
extern void Pascal_dosmemput (uint16_t seg, uint16_t ofs, const void *data, uint32_t count);

extern int32_t Pascal_global_dos_alloc (int32_t bytes);
extern bool Pascal_global_dos_free (uint16_t selector);

extern bool Pascal_realintr (uint16_t intnr, void *regs);

extern int32_t Pascal_get_linear_addr (int32_t phys_addr, int32_t size);

#endif // GO32

#endif // !defined(PASCAL_H)
