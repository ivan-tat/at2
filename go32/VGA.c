// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include "go32/adt2dpmi.h"
#include "go32/BIOS.h"
#include "go32/VBIOS.h"
#include "go32/VGA.h"

uint8_t  v_mode = 3;
uint8_t  v_font = 16;
uint8_t  v_cols = 80;
uint8_t  v_rows = 25;
uint8_t  v_page = 0;
uint16_t v_regen_size = (80 * 25 * 2 + 0xFF) & ~0xFF;
uint16_t v_ofs = 0;
uint16_t v_seg = VGA_SEG_B800;
uint16_t v_curpos = 0 + (0 << 8);
uint16_t v_curshape = 16 - 1 + ((15 - 1) << 8);

#if !ADT2PLAY
#include "go32/VGA/VGACustomTextModes.c"
#endif // !ADT2PLAY

#include "go32/VGA/VGA_WaitDisplayDisabled.c"
#include "go32/VGA/VGA_WaitVerticalRetrace.c"

#include "go32/VGA/VGA_GetPaletteEntry.c"
#include "go32/VGA/VGA_GetPalette.c"
#include "go32/VGA/VGA_SetPaletteEntry.c"
#include "go32/VGA/VGA_SetPalette.c"

#include "go32/VGA/VGA_GetFontHeight.c"
#include "go32/VGA/VGA_GetCursorPosition.c"
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
#if !ADT2PLAY
#include "go32/VGA/VGA_SetTextModeDimensions.c"
#include "go32/VGA/VGA_ResetTextMode.c"
#include "go32/VGA/OnMakeTextMode.c"
#include "go32/VGA/VGA_MakeTextMode.c"
#include "go32/VGA/VGA_MakeCustomTextMode.c"
#include "go32/VGA/VGA_MakeCustomTextModeEx.c"
#endif // !ADT2PLAY

#if ADT2PLAY
#include "go32/VGA/VGA_SaveVideoState.c"
#include "go32/VGA/VGA_RestoreVideoState.c"
#include "go32/VGA/VGA_FreeVideoState.c"
#endif // ADT2PLAY
