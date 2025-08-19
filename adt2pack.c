// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"
#include <stdint.h>
#include <stdlib.h>
#include "adt2unit.h"
#include "adt2extn.h"
#include "adt2pack.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

// DEFAULT COMPRESSION:
#if !ADT2PLAY
#define WIN_SIZE_DEF (1 << 12) // buffer 4k
#endif // !ADT2PLAY
#define DIC_SIZE_DEF (1 << 13) // dictionary 8kb
// ULTRA COMPRESSION:
#if !ADT2PLAY
#define WIN_SIZE_MAX (1 << 15) // buffer 32k
#endif // !ADT2PLAY
#define DIC_SIZE_MAX (1 << 14) // dictionary 16kb

#define MATCH_BIT 8
#define MAX_MATCH (1 << MATCH_BIT)
#define THRESHOLD 2
#define PBIT 14
#define TBIT 15
#define CBIT 16
#define DIC_BIT 14
#define CODE_BIT 16
#define NC (255 + MAX_MATCH + 2 - THRESHOLD)
#define NP (DIC_BIT + 1)
#define NT (CODE_BIT + 3)
#define MAX_HASH_VAL (3 * (1 << DIC_BIT) + (((1 << DIC_BIT) >> 9) + 1) * 255)
#define PERC_FLAG 32768

#define _put_memb_off(struc, memb) .memb = __builtin_offsetof (struc, memb)

#include "adt2pack/LZH_compress.c"
#include "adt2pack/LZH_decompress.c"

#pragma pack(pop)
