// DJGPP support
//
// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
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
#define PAREA_CONCAT(a, b, c)  PAREA_CONCAT_ (a, b, c)
#define PAREA_CONCAT_(a, b, c) ___##a##_##b##_##c##___

#if 1

// Simple version
// All constants are placed in `.text' section, interleaving code.

// Helper macros (uses `PAREA' macro)
#define PAREA_LABEL(a, b) PAREA_CONCAT (PAREA, a, b)
#define PAREA_CONST_START ALIGNED_(1) static const char PAREA_LABEL (CONST, START) = 0;
#define PAREA_CONST_END   ALIGNED_(1) static const char PAREA_LABEL (CONST, END  ) = 0;
#define PAREA_DATA_START  ALIGNED_(1) static char PAREA_LABEL (DATA, START) = 0;
#define PAREA_DATA_END    ALIGNED_(1) static char PAREA_LABEL (DATA, END  ) = 0;
#define PAREA_BSS_START   ALIGNED_(1) static char PAREA_LABEL (BSS,  START);
#define PAREA_BSS_END     ALIGNED_(1) static char PAREA_LABEL (BSS,  END  );
#define PAREA_CODE_START  ALIGNED_(1) NAKED_RELAXED static void PAREA_LABEL (CODE, START) (void) { }
#define PAREA_CODE_END    ALIGNED_(1) NAKED_RELAXED static void PAREA_LABEL (CODE, END  ) (void) { }

#else // !1

// Complex version: needs modification of GNU linker script
// Referenced string constants are placed in `.text' section regardless of specified attributes.

#define PAREA_STR(a)  PAREA_STR_ (a)
#define PAREA_STR_(a) # a

// Helper macros (uses `PAREA' macro)
#define PAREA_LABEL(a, b) PAREA_CONCAT (PAREA, a, b)
#define PAREA_CONST_START GCC_ATTRIBUTE ((section (".rodata." PAREA_STR (PAREA) ".0"))) ALIGNED_(1) static const char PAREA_LABEL (CONST, START) = 0;
#define PAREA_CONST_END   GCC_ATTRIBUTE ((section (".rodata." PAREA_STR (PAREA) ".2"))) ALIGNED_(1) static const char PAREA_LABEL (CONST, END  ) = 0;
#define PAREA_DATA_START  GCC_ATTRIBUTE ((section (".data."   PAREA_STR (PAREA) ".0"))) ALIGNED_(1) static char       PAREA_LABEL (DATA,  START) = 0;
#define PAREA_DATA_END    GCC_ATTRIBUTE ((section (".data."   PAREA_STR (PAREA) ".2"))) ALIGNED_(1) static char       PAREA_LABEL (DATA,  END  ) = 0;
#define PAREA_BSS_START   GCC_ATTRIBUTE ((section (".bss."    PAREA_STR (PAREA) ".0"))) ALIGNED_(1) static char       PAREA_LABEL (BSS,   START);
#define PAREA_BSS_END     GCC_ATTRIBUTE ((section (".bss."    PAREA_STR (PAREA) ".2"))) ALIGNED_(1) static char       PAREA_LABEL (BSS,   END  );
#define PAREA_CODE_START  GCC_ATTRIBUTE ((section (".text."   PAREA_STR (PAREA) ".0"))) ALIGNED_(1) NAKED_RELAXED static void PAREA_LABEL (CODE, START) (void) { }
#define PAREA_CODE_END    GCC_ATTRIBUTE ((section (".text."   PAREA_STR (PAREA) ".2"))) ALIGNED_(1) NAKED_RELAXED static void PAREA_LABEL (CODE, END  ) (void) { }

// User side macros (uses `PAREA' macro)
#define PCONST GCC_ATTRIBUTE ((section (".rodata." PAREA_STR (PAREA) ".1")))
#define PDATA  GCC_ATTRIBUTE ((section (".data."   PAREA_STR (PAREA) ".1")))
#define PBSS   GCC_ATTRIBUTE ((section (".bss."    PAREA_STR (PAREA) ".1")))
#define PCODE  GCC_ATTRIBUTE ((section (".text."   PAREA_STR (PAREA) ".1")))

#endif // !1

#define PAREA_START(a) PAREA_##a##_START
#define PAREA_END(a)   PAREA_##a##_END
#define PAREA_SIZE(a)  ((ptrdiff_t) &PAREA_LABEL (a, END) - (ptrdiff_t) &PAREA_LABEL (a, START))

// Lock macros

#define LOCK_FUNC     NO_REORDER
#define END_LOCK_FUNC NO_REORDER ALIGNED_(1) NAKED_RELAXED

#define END_OF_FUNCTION(f)        END_LOCK_FUNC void f##_end (void) { }
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
  int32_t (*v) (void *lockaddr, uint32_t locksize) UNUSED;                   \
  int32_t (*f) (void *lockaddr, uint32_t locksize) UNUSED;                   \
                                                                             \
  if (lock) {                                                                \
    v = LOCK_DATA_PROC;                                                      \
    f = LOCK_CODE_PROC;                                                      \
  } else {                                                                   \
    v = UNLOCK_DATA_PROC;                                                    \
    f = UNLOCK_CODE_PROC;                                                    \
  }

#endif // !defined(GO32_ADT2DPMI_H)
