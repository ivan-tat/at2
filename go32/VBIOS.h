// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef VBIOS_H
#define VBIOS_H

#include <stdbool.h>
#include <stdint.h>

#include "go32/VBIOS_is_VGA.c"
#include "go32/VBIOS_get_video_mode.c"
#include "go32/VBIOS_set_video_mode.c"
#include "go32/VBIOS_load_ROM_font_8x8.c"
#include "go32/VBIOS_load_ROM_font_8x14.c"
#include "go32/VBIOS_load_ROM_font_8x16.c"
#include "go32/VBIOS_get_cursor_pos.c"
#include "go32/VBIOS_set_cursor_pos.c"
#include "go32/VBIOS_set_screen_refresh.c"

#endif // !defined(VBIOS_H)
