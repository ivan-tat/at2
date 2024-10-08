// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef GO32_BIOS_H
#define GO32_BIOS_H

#include <stdint.h>
#include <sys/farptr.h>

// BIOS Data Area (BDA)

#include "go32/BIOS/BDA_get_keyboard_flag_byte_0.c"
#include "go32/BIOS/BDA_get_keyboard_buffer_head.c"
#include "go32/BIOS/BDA_get_keyboard_buffer_tail.c"
#include "go32/BIOS/BDA_set_keyboard_buffer_tail.c"
#include "go32/BIOS/BDA_get_active_video_mode.c"
#include "go32/BIOS/BDA_set_active_video_mode.c"
#include "go32/BIOS/BDA_get_screen_character_height.c"
#include "go32/BIOS/BDA_set_screen_character_height.c"
#include "go32/BIOS/BDA_get_screen_text_columns.c"
#include "go32/BIOS/BDA_set_screen_text_columns.c"
#include "go32/BIOS/BDA_get_screen_text_rows.c"
#include "go32/BIOS/BDA_set_screen_text_rows.c"
#include "go32/BIOS/BDA_get_video_regen_buffer_size.c"
#include "go32/BIOS/BDA_set_video_regen_buffer_size.c"
#include "go32/BIOS/BDA_get_video_page_offset.c"
#include "go32/BIOS/BDA_set_video_page_offset.c"
#include "go32/BIOS/BDA_get_active_video_page.c"
#include "go32/BIOS/BDA_set_active_video_page.c"
#include "go32/BIOS/BDA_set_cursor_shape.c"
#include "go32/BIOS/BDA_set_cursor_position.c"
#include "go32/BIOS/BDA_get_active_6845_CRTC_port.c"
#include "go32/BIOS/BDA_set_active_6845_CRTC_port.c"

#endif // !defined(GO32_BIOS_H)
