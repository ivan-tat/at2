// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"
#include "pascal.h"
#include <string.h>
#include "depackio.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

#define WORKMEM_SIZE (64 * 1024) // 64KiB

uint8_t work_mem[WORKMEM_SIZE];

#include "depackio/RDC_decompress.c"
#include "depackio/LZSS_decompress.c"
#include "depackio/LZW_decompress.c"
#include "depackio/SIXPACK_decompress.c"
#include "depackio/APACK_decompress.c"

#pragma pack(pop)
