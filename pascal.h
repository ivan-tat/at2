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

extern void Pascal_FillChar (void *x, ssize_t count, uint8_t value);
extern void Pascal_FillWord (void *x, ssize_t count, uint16_t value);

extern void Pascal_Move (const void *src, void *dest, ssize_t n);

extern size_t Pascal_strlen (const char *s);

extern void Pascal_Delay (uint16_t ms);

#if GO32

extern uint16_t Pascal_dosmemselector (void);
extern uint16_t *Pascal_int31error_ptr (void);

extern void Pascal_dosmemget (uint16_t seg, uint16_t ofs, void *data,
                              uint32_t count);
extern void Pascal_dosmemput (uint16_t seg, uint16_t ofs, const void *data,
                              uint32_t count);

extern int32_t Pascal_global_dos_alloc (int32_t bytes);
extern bool Pascal_global_dos_free (uint16_t selector);

extern bool Pascal_realintr (uint16_t intnr, void *regs);

#endif // GO32

#endif // !defined(PASCAL_H)
