// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"
#include "platform.h"
#if GO32
#if USE_FPC
#include "pascal/stdlib.h"
#else // !USE_FPC
#include "<stdlib.h>"
#endif // !USE_FPC
#include "go32/adt2dpmi.h"
#endif // GO32
#include "adt2unit.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

#if GO32
uint8_t ___ADT2UNIT_DATA_START___ = 0;
#endif // GO32

#include "typcons1.c"
#include "typcons2.c"

bool     no_status_refresh = false;

uint8_t pattord_page;
uint8_t pattord_hpos;
uint8_t pattord_vpos;
uint8_t instrum_page;
uint8_t pattern_patt;
uint8_t pattern_page;
uint8_t pattern_hpos;

#include "realtime.c"

#include "adt2unit/min.c"
#include "adt2unit/max.c"

int32_t seconds_counter = 0;
int32_t hundereds_counter = 0;

#if GO32

uint8_t ___ADT2UNIT_DATA_END___ = 0;

static void lock_adt2unit (bool lock) {
  LOCK_DECL (lock, v, f);

  v (&___ADT2UNIT_DATA_START___, (ptrdiff_t) &___ADT2UNIT_DATA_END___
                                 - (ptrdiff_t) &___ADT2UNIT_DATA_START___);
}

static void done_adt2unit (void);

void init_adt2unit (void) {
  lock_adt2unit (true);
  atexit (done_adt2unit);
}

static void done_adt2unit (void) {
  lock_adt2unit (false);
}

#endif // GO32
