// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "txtscrio.h"

#include "go32/VBIOS.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

int32_t SCREEN_MEM_SIZE = MAX_SCREEN_MEM_SIZE;

tSCREEN_MEM text_screen_shadow;

void    *screen_ptr = &text_screen_shadow;
uint16_t v_seg = 0xB800;
uint16_t v_ofs = 0;
uint8_t  v_mode = 0x03;
uint8_t  MaxLn = 80;
uint8_t  MaxCol = 25;

int32_t cursor_backup;
uint8_t DispPg;

#include "txtscrio/go32/fade.c"
