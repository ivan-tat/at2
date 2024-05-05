// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"

#if USE_FPC
#include "pascal/pc.h"
#else // !USE_FPC
#include <pc.h>
#endif // !USE_FPC
#include "go32/VGA.h"

uint8_t  v_mode = 3;
uint8_t  v_font = 16;
uint8_t  v_cols = 80;
uint8_t  v_rows = 25;
uint8_t  v_page = 0;
uint16_t v_regen_size = 4096;
uint16_t v_ofs = 0;
uint16_t v_seg = VGA_SEG_B800;
uint16_t v_curpos = 0x0000;
uint16_t v_curshape = 0x0D0E;

#include "go32/VGA/VGA_WaitDisplayDisabled.c"
#include "go32/VGA/VGA_WaitVerticalRetrace.c"
#include "go32/VGA/VGA_GetCursorShape.c"
#include "go32/VGA/VGA_SetCursorShape.c"
#include "go32/VGA/VGA_GetPaletteEntry.c"
#include "go32/VGA/VGA_GetPalette.c"
#include "go32/VGA/VGA_SetPaletteEntry.c"
#include "go32/VGA/VGA_SetPalette.c"
#include "go32/VGA/VGA_SetPresetRowScan.c"
#include "go32/VGA/VGA_SetStartAddress.c"
#include "go32/VGA/VGA_SetLineCompare.c"
#include "go32/VGA/VGA_SetPixelPanningMode.c"
