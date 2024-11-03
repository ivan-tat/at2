// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "debug.h"
#include "platform.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

bool _debug_ = false;
char _dbg_verbose = 1;

struct _dbg_callstack_t _dbg_callstack[DBG_CALLSTACK_MAX];

int _dbg_callstackptr = -1;

#define Min(a,b) (((a)<=(b))?(a):(b))

#include "debug/_dbg_enter.c"
#include "debug/_dbg_label.c"
#include "debug/_dbg_printf.c"
#include "debug/_dbg_leave.c"
#include "debug/_dbg_dump_callstack.c"
