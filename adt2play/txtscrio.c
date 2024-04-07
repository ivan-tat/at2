// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"

#include <stddef.h>
#if USE_FPC
#include "pascal.h"
#if GO32
#include "pascal/farptr.h"
#include "pascal/go32.h"
#include "pascal/pc.h"
#endif // GO32
#include "pascal/dos.h"
#include "pascal/string.h"
#else // !USE_FPC
#if GO32
#include <farptr.h>
#include <go32.h>
#include <pc.h>
#endif // GO32
#include <dos.h>
#include <string.h>
#endif // !USE_FPC
#if GO32
#include "go32/VBIOS.h"
#include "go32/VGA.h"
#endif // GO32
#include "common.h"
#include "txtscrio.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

int32_t SCREEN_MEM_SIZE = MAX_SCREEN_MEM_SIZE;

tSCREEN_MEM text_screen_shadow;

void *screen_ptr = &text_screen_shadow;

uint8_t MaxCol = 80;
uint8_t MaxLn = 25;

int32_t cursor_backup;

static uint16_t absolute_pos;

#include "txtscrio/PosChar.c"

#include "txtscrio/ShowC3Str.c"

#include "txtscrio/WhereX.c"
#include "txtscrio/WhereY.c"
#include "txtscrio/GotoXY.c"
#include "txtscrio/GetCursor.c"
#include "txtscrio/SetCursor.c"
#include "txtscrio/ThinCursor.c"
#include "txtscrio/WideCursor.c"
#include "txtscrio/HideCursor.c"
#include "txtscrio/GetCursorShape.c"
#include "txtscrio/SetCursorShape.c"

uint16_t v_seg = 0xB800;
uint16_t v_ofs = 0;
uint8_t  v_mode = 3;

uint8_t DispPg;

#include "txtscrio/go32/iVGA.c"
#include "txtscrio/go32/initialize.c"
#include "txtscrio/go32/GetRGBitem.c"
#include "txtscrio/go32/SetRGBitem.c"
#include "txtscrio/go32/GetPalette.c"
#include "txtscrio/go32/SetPalette.c"
#include "txtscrio/go32/WaitRetrace.c"

#include "txtscrio/go32/fade.c"

#include "txtscrio/go32/VgaFade.c"
