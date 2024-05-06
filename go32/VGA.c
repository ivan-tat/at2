// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"

#if USE_FPC
#include "pascal/dos.h"
#include "pascal/pc.h"
#include "pascal/stdlib.h"
#include "pascal/string.h"
#else // !USE_FPC
#include <dos.h>
#include <pc.h>
#include <stdlib.h>
#include <string.h>
#endif // !USE_FPC
#include "go32/adt2dpmi.h"
#include "go32/BIOS.h"
#include "go32/VBIOS.h"
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

#include "go32/VGA/VGA_GetPaletteEntry.c"
#include "go32/VGA/VGA_GetPalette.c"
#include "go32/VGA/VGA_SetPaletteEntry.c"
#include "go32/VGA/VGA_SetPalette.c"

#include "go32/VGA/VGA_GetCursorShape.c"
#include "go32/VGA/VGA_SetCursorShape.c"
#include "go32/VGA/VGA_SetPresetRowScan.c"
#include "go32/VGA/VGA_SetOffset.c"
#include "go32/VGA/VGA_SetStartAddress.c"
#include "go32/VGA/VGA_SetLineCompare.c"
#include "go32/VGA/VGA_SetPixelPanningMode.c"

#include "go32/VGA/VGA_ReadTextMode.c"
#include "go32/VGA/VGA_SetTextMode.c"
#include "go32/VGA/VGA_SetTextMode_80x25.c"
#include "go32/VGA/VGA_SetTextModeDimensions.c"
#include "go32/VGA/VGA_ResetTextMode.c"
#include "go32/VGA/VGA_MakeTextMode.c"

#if ADT2PLAY
#include "go32/VGA/VGA_SaveVideoState.c"
#include "go32/VGA/VGA_RestoreVideoState.c"
#include "go32/VGA/VGA_FreeVideoState.c"
#endif // ADT2PLAY
