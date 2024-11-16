// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "adt2ext2.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

uint8_t quick_mark_type = 0;
bool    discard_block = false;
uint8_t old_chan_pos = 1;
uint8_t old_hpos = 1;
uint8_t old_page = 0;
uint8_t old_block_chan_pos = 1;
uint8_t old_block_patt_hpos = 1;
uint8_t old_block_patt_page = 0;

#pragma pack(pop)
