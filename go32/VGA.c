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

#include "go32/VGA/VGA_WaitRetrace.c"
#include "go32/VGA/VGA_GetPaletteEntry.c"
#include "go32/VGA/VGA_GetPalette.c"
#include "go32/VGA/VGA_SetPaletteEntry.c"
#include "go32/VGA/VGA_SetPalette.c"
#include "go32/VGA/VGA_SplitScreen.c"
