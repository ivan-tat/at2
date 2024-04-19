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
#if USE_FPC
#include "pascal/dpmi.h"
#include "pascal/go32.h"
#include "pascal/sys/exceptn.h"
#include "pascal/sys/farptr.h"
#else // !USE_FPC
#include <dpmi.h>
#include <go32.h>
#include <sys/exceptn.h>
#include <sys/farptr.h>
#endif // !USE_FPC
#include "go32/dpmi.h"

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
