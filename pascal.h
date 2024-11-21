// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef PASCAL_H
#define PASCAL_H

#include "defines.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

typedef unsigned char Shortstring;

typedef struct {
  uint8_t len;
  Shortstring str[255];
} Shortstring_t;

typedef Shortstring String;

typedef Shortstring_t String_t;

#define Length(s) ((s)[0])
#define SetLength(s, l) (s)[0] = (l)
#define GetStr(s) (&(s)[1])

// `go32' unit

#if GO32

extern uint16_t __v2prt0_ds_alias;
extern uint16_t Pascal_dosmemselector;

extern uint16_t Pascal_allocate_ldt_descriptors (uint16_t count);
extern bool Pascal_free_ldt_descriptor (uint16_t selector);
extern uint16_t Pascal_get_next_selector_increment_value (void);
extern bool Pascal_set_segment_base_address (uint16_t selector, int32_t address);
extern bool Pascal_set_segment_limit (uint16_t selector, int32_t limit);

extern void Pascal_dosmemget (uint16_t seg, uint16_t ofs, void *data, uint32_t count);
extern void Pascal_dosmemput (uint16_t seg, uint16_t ofs, const void *data, uint32_t count);

extern int32_t Pascal_global_dos_alloc (int32_t bytes);
extern bool Pascal_global_dos_free (uint16_t selector);

extern bool Pascal_get_pm_interrupt (uint8_t vector, void *intaddr);
extern bool Pascal_set_pm_interrupt (uint8_t vector, const void *intaddr);

extern bool Pascal_realintr (uint16_t intnr, void *regs);

extern int32_t Pascal_get_linear_addr (int32_t phys_addr, int32_t size);

extern bool Pascal_lock_data (const void *data, int32_t size);
extern bool Pascal_lock_code (const void *functionaddr, int32_t size);
extern bool Pascal_unlock_data (const void *data, int32_t size);
extern bool Pascal_unlock_code (const void *functionaddr, int32_t size);

#endif // GO32

// `system' unit

typedef double TDateTime;

#if GO32
#if USE_FPC
extern bool Pascal_LFNSupport;
#endif // USE_FPC
#endif // GO32

extern void *Pascal_Output;

extern uint16_t *Pascal_InOutRes_ptr;
#define Pascal_InOutRes (*Pascal_InOutRes_ptr)

extern void Pascal_Halt (int32_t errnum) __NORETURN;

extern void Pascal_FillChar (void *x, ssize_t count, uint8_t value);
extern void Pascal_FillWord (void *x, ssize_t count, uint16_t value);

extern void Pascal_Move (const void *src, void *dest, ssize_t n);

extern float Pascal_Abs_Single (float x);
extern double Pascal_Abs_Double (double x);

extern float Pascal_Trunc_Single (float x);
extern double Pascal_Trunc_Double (double x);

extern void *Pascal_AllocMem (size_t size);
extern size_t Pascal_FreeMem (void *p);
extern void *Pascal_ReAllocMem (void **p, size_t size);

extern void Pascal_Write_PChar (void *text, const char *str);
extern void Pascal_Write_String (void *text, const String *str);
extern void Pascal_Flush (void *text);

extern int Pascal_Random (int l);

// `strings' unit

extern size_t Pascal_strlen (const char *s);
extern char *Pascal_strcopy (char *dest, const char *src);
extern char *Pascal_strecopy (char *dest, const char *src);
extern char *Pascal_strlcopy (char *dest, const char *src, ssize_t n);
extern char *Pascal_strcat (char *dest, const char *src);
extern char *Pascal_strlcat (char *dest, const char *src, ssize_t n);
extern ssize_t Pascal_strcomp (const char *s1, const char *s2);
extern ssize_t Pascal_strlcomp (const char *s1, const char *s2, ssize_t n);
extern char *Pascal_strnew (const char *s);
extern char *Pascal_strscan (const char *s, char c);
extern char *Pascal_strrscan (const char *s, char c);
extern char *Pascal_strpos (const char *haystack, const char *needle);
extern char *Pascal_stripos (const char *haystack, const char *needle);

// `crt' unit

extern void Pascal_Delay (uint16_t ms);
extern bool Pascal_KeyPressed (void);
extern char Pascal_ReadKey (void);

// `sysutils' unit

extern TDateTime Pascal_Now (void);

// `dateutils' unit

extern int64_t Pascal_DateTimeToUnix (const TDateTime value);

#endif // !defined(PASCAL_H)
