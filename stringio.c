// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"

#include <stdint.h>
#if USE_FPC
#include "pascal/string.h"
#else // !USE_FPC
#include <string.h>
#endif // !USE_FPC
#include "stringio.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

// UCS-2 to `AT2' code page conversion table
static uint8_t UCS2_to_AT2_ct[0x10000];

#include "stringio/CP437_it.c"

#include "stringio/UTF8_to_wchar.c"
#include "stringio/UTF8str_to_AT2.c"
#include "stringio/UTF8nstr_to_AT2.c"

#if !ADT2PLAY
#include "stringio/CStrLen.c"
#include "stringio/CStr2Len.c"
#include "stringio/C3StrLen.c"
#endif // !ADT2PLAY

#include "stringio/init_StringIO.c"
