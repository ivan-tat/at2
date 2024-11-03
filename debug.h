// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef DEBUG_H
#define DEBUG_H

#include "defines.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h> // __VA_ARGS__

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

extern bool _debug_;
extern char _dbg_verbose; // 0=silent, 1=printf, 2=+calls

#define DBG_CALLSTACK_MAX 32

extern struct _dbg_callstack_t
{
  const char *file;
  const char *func;
  const char *label;
} _dbg_callstack[DBG_CALLSTACK_MAX];

extern int  _dbg_callstackptr;

void _dbg_enter (const char *file, const char *func);
void _dbg_label (const char *label);
void _dbg_printf (const char *format, ...);
void _dbg_leave (void);
void _dbg_dump_callstack (void);

#define DBG_ENTER(func)      _dbg_enter (__FILE__, func)
#define DBG_LABEL(label)     _dbg_label (label)
#define DBG_PRINTF(fmt, ...) _dbg_printf (fmt, __VA_ARGS__)
#define DBG_LOG(s)           _dbg_printf ("%s", s)
#define DBG_LEAVE()          _dbg_leave ()

#pragma pack(pop)

#endif // !defined(DEBUG_H)
