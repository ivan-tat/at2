// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"
#include "adt2unit.h"
#include "stringio.h"
#include "txtscrio.h"
#include "adt2extn.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

#include "adt2extn/_patts_marked.c"

#include "adt2extn/nul_volume_bars.c"

/*static*/ bool    transp_menu2 = false;
/*static*/ uint8_t transp_pos1 = 1;
/*static*/ uint8_t transp_pos2 = 1;

#pragma pack(pop)
