// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"
#include "debug.h"
#include "parserio.h"
#include "adt2unit.h"
#include "adt2ext5.h"
#include "adt2ext4.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

bool _arp_vib_loader = false;
bool _arp_vib_mode = false;
bool _macro_editor__pos[2] = { 1, 8 };
bool _macro_editor__fmreg_hpos[2] = { 1, 1 };
bool _macro_editor__fmreg_page[2] = { 1, 1 };
bool _macro_editor__fmreg_left_margin[2] = { 1, 1 };
bool _macro_editor__fmreg_cursor_pos[2] = { 1, 1 };
bool _macro_editor__arpeggio_page[2] = { 1, 1 };
bool _macro_editor__vibrato_hpos[2] = { 1, 1 };
bool _macro_editor__vibrato_page[2] = { 1, 1 };

uint8_t arpvib_arpeggio_table = 1;
uint8_t arpvib_vibrato_table = 1;

#include "adt2ext4/MACRO_BROWSER.c"

#pragma pack(pop)
