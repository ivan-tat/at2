// DJGPP support
//
// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef GO32_ADT2DPMI_H
#define GO32_ADT2DPMI_H

#include "defines.h"
#include <dpmi.h>
#include <go32.h>
#include <sys/exceptn.h>
#include <sys/farptr.h>
#include "go32/dpmi.h"

// Protected area macros

// Helper macros
#define __PAREA_CONCAT(a, b, c)  __PAREA_CONCAT_ (a, b, c)
#define __PAREA_CONCAT_(a, b, c) ___##a##_##b##_##c##___

#if 1

// Simple version
// All constants are placed in `.text' section, interleaving code.

// Helper macros (uses `__PAREA' macro)
#define __PAREA_LABEL(a, b) __PAREA_CONCAT (__PAREA, a, b)
#define __PAREA_CONST_START __ALIGNED_(1) static const char __PAREA_LABEL (CONST, START) = 0;
#define __PAREA_CONST_END   __ALIGNED_(1) static const char __PAREA_LABEL (CONST, END  ) = 0;
#define __PAREA_DATA_START  __ALIGNED_(1) static char __PAREA_LABEL (DATA, START) = 0;
#define __PAREA_DATA_END    __ALIGNED_(1) static char __PAREA_LABEL (DATA, END  ) = 0;
#define __PAREA_BSS_START   __ALIGNED_(1) static char __PAREA_LABEL (BSS,  START);
#define __PAREA_BSS_END     __ALIGNED_(1) static char __PAREA_LABEL (BSS,  END  );
#define __PAREA_CODE_START  __ALIGNED_(1) __NAKED_RELAXED static void __PAREA_LABEL (CODE, START) (void) { }
#define __PAREA_CODE_END    __ALIGNED_(1) __NAKED_RELAXED static void __PAREA_LABEL (CODE, END  ) (void) { }

#else // !1

// Complex version: needs modification of GNU linker script
// Referenced string constants are placed in `.text' section regardless of specified attributes.

#define __PAREA_STR(a)  __PAREA_STR_ (a)
#define __PAREA_STR_(a) # a

// Helper macros (uses `__PAREA' macro)
#define __PAREA_LABEL(a, b) __PAREA_CONCAT (__PAREA, a, b)
#define __PAREA_CONST_START GCC_ATTRIBUTE ((section (".rodata." __PAREA_STR (__PAREA) ".0"))) __ALIGNED_(1) static const char __PAREA_LABEL (CONST, START) = 0;
#define __PAREA_CONST_END   GCC_ATTRIBUTE ((section (".rodata." __PAREA_STR (__PAREA) ".2"))) __ALIGNED_(1) static const char __PAREA_LABEL (CONST, END  ) = 0;
#define __PAREA_DATA_START  GCC_ATTRIBUTE ((section (".data."   __PAREA_STR (__PAREA) ".0"))) __ALIGNED_(1) static char       __PAREA_LABEL (DATA,  START) = 0;
#define __PAREA_DATA_END    GCC_ATTRIBUTE ((section (".data."   __PAREA_STR (__PAREA) ".2"))) __ALIGNED_(1) static char       __PAREA_LABEL (DATA,  END  ) = 0;
#define __PAREA_BSS_START   GCC_ATTRIBUTE ((section (".bss."    __PAREA_STR (__PAREA) ".0"))) __ALIGNED_(1) static char       __PAREA_LABEL (BSS,   START);
#define __PAREA_BSS_END     GCC_ATTRIBUTE ((section (".bss."    __PAREA_STR (__PAREA) ".2"))) __ALIGNED_(1) static char       __PAREA_LABEL (BSS,   END  );
#define __PAREA_CODE_START  GCC_ATTRIBUTE ((section (".text."   __PAREA_STR (__PAREA) ".0"))) __ALIGNED_(1) __NAKED_RELAXED static void __PAREA_LABEL (CODE, START) (void) { }
#define __PAREA_CODE_END    GCC_ATTRIBUTE ((section (".text."   __PAREA_STR (__PAREA) ".2"))) __ALIGNED_(1) __NAKED_RELAXED static void __PAREA_LABEL (CODE, END  ) (void) { }

// User side macros (uses `__PAREA' macro)
#define __PCONST GCC_ATTRIBUTE ((section (".rodata." __PAREA_STR (__PAREA) ".1")))
#define __PDATA  GCC_ATTRIBUTE ((section (".data."   __PAREA_STR (__PAREA) ".1")))
#define __PBSS   GCC_ATTRIBUTE ((section (".bss."    __PAREA_STR (__PAREA) ".1")))
#define __PCODE  GCC_ATTRIBUTE ((section (".text."   __PAREA_STR (__PAREA) ".1")))

#endif // !1

#define __PAREA_START(a) __PAREA_##a##_START
#define __PAREA_END(a)   __PAREA_##a##_END
#define __PAREA_SIZE(a)  ((ptrdiff_t) &__PAREA_LABEL (a, END) - (ptrdiff_t) &__PAREA_LABEL (a, START))

// Lock macros

#define __LOCK_FUNC     __NO_REORDER
#define __END_LOCK_FUNC __NO_REORDER __ALIGNED_(1) __NAKED_RELAXED

#define END_OF_FUNCTION(f)        __END_LOCK_FUNC void f##_end (void) { }
#define END_OF_STATIC_FUNCTION(f) static END_OF_FUNCTION (f)

#define LOCK_DATA_PROC   _go32_dpmi_lock_data
#define LOCK_CODE_PROC   _go32_dpmi_lock_code
#define UNLOCK_DATA_PROC _go32_dpmi_unlock_data
#define UNLOCK_CODE_PROC _go32_dpmi_unlock_code

#define LOCK_DATA(x, d, s)  x ((void *) d, s)
#define LOCK_CODE(x, c, s)  x ((void *) c, s)
#define LOCK_VARIABLE(x, v) LOCK_DATA (x, &v, sizeof (v))
#define LOCK_FUNCTION(x, f)                                                  \
  LOCK_CODE (x, (uintptr_t) f, (ptrdiff_t) f##_end - (ptrdiff_t) f)

#define LOCK_DECL(lock, v, f)                                                \
  int32_t (*v) (void *lockaddr, uint32_t locksize) __UNUSED;                 \
  int32_t (*f) (void *lockaddr, uint32_t locksize) __UNUSED;                 \
                                                                             \
  if (lock) {                                                                \
    v = LOCK_DATA_PROC;                                                      \
    f = LOCK_CODE_PROC;                                                      \
  } else {                                                                   \
    v = UNLOCK_DATA_PROC;                                                    \
    f = UNLOCK_CODE_PROC;                                                    \
  }

#endif // !defined(GO32_ADT2DPMI_H)
