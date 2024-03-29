// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"

#include <stddef.h>
#if USE_FPC
#include "pascal.h"
#else // !USE_FPC
#include <string.h> // memcpy, memmove
#endif // !USE_FPC
#include "txtscrio.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

uint16_t SCREEN_RES_x = 720;
uint16_t SCREEN_RES_y = 480;
uint8_t  MAX_COLUMNS = 90;
uint8_t  MAX_ROWS = 40;
uint8_t  MAX_TRACKS = 5;
uint8_t  MAX_ORDER_COLS = 9;
uint8_t  MAX_PATTERN_ROWS = 18;
uint8_t  INSCTRL_xshift = 0;
int8_t   INSCTRL_yshift = 0;
uint8_t  INSEDIT_yshift = 0;
uint8_t  PATTORD_xshift = 0;
uint8_t  GOTOXY_xshift = 0;

int32_t SCREEN_MEM_SIZE = MAX_SCREEN_MEM_SIZE;

tSCREEN_MEM temp_screen;
tSCREEN_MEM temp_screen2;
tSCREEN_MEM screen_backup;
tSCREEN_MEM scr_backup;
tSCREEN_MEM scr_backup2;
tSCREEN_MEM screen_mirror;
tSCREEN_MEM screen_emulator;
tSCREEN_MEM *centered_frame_vdest;
tSCREEN_MEM text_screen_shadow;

void *screen_ptr          = (void *) text_screen_shadow;
void *ptr_temp_screen     = (void *) temp_screen;
void *ptr_temp_screen2    = (void *) temp_screen2;
void *ptr_screen_backup   = (void *) screen_backup;
void *ptr_scr_backup      = (void *) scr_backup;
void *ptr_scr_backup2     = (void *) scr_backup2;
void *ptr_screen_mirror   = (void *) screen_mirror;
void *ptr_screen_emulator = (void *) screen_emulator;

void   *move_to_screen_data = NULL;
uint8_t move_to_screen_area[4] = { 0, 0, 0, 0 };
void  (*move_to_screen_routine) () = NULL;

uint8_t program_screen_mode = 0;

uint8_t MaxLn = 0;
uint8_t MaxCol = 0;
uint8_t hard_maxcol = 0;
uint8_t hard_maxln = 0;
uint8_t work_maxcol = 0;
uint8_t work_maxln = 0;
uint8_t scr_font_width = 0;
uint8_t scr_font_height = 0;

uint8_t area_x1 = 0;
uint8_t area_y1 = 0;
uint8_t area_x2 = 0;
uint8_t area_y2 = 0;
uint8_t scroll_pos0 = ~0;
uint8_t scroll_pos1 = ~0;
uint8_t scroll_pos2 = ~0;
uint8_t scroll_pos3 = ~0;
uint8_t scroll_pos4 = ~0;

int32_t cursor_backup;

tFRAME_SETTING fr_setting = {
  .shadow_enabled = true,
  .wide_range_type = false,
  .zooming_enabled = false,
  .update_area = true
};

uint16_t v_seg = 0xB800;
uint16_t v_ofs = 0;
uint8_t  v_mode = 3;

#include "txtscrio/show_str.c"
#include "txtscrio/show_cstr.c"
#include "txtscrio/show_cstr_alt.c"
#include "txtscrio/show_vstr.c"
#include "txtscrio/show_vcstr.c"

static uint16_t absolute_pos;

#include "txtscrio/DupChar.c"

#include "txtscrio/ShowStr.c"
#include "txtscrio/ShowVStr.c"
#include "txtscrio/ShowCStr.c"
#include "txtscrio/ShowVCStr.c"
#include "txtscrio/ShowCStr2.c"
#include "txtscrio/ShowVCStr2.c"
#include "txtscrio/ShowC3Str.c"
#include "txtscrio/ShowVC3Str.c"
#include "txtscrio/ShowC4Str.c"

#include "txtscrio/CStrLen.c"
#include "txtscrio/CStr2Len.c"
#include "txtscrio/C3StrLen.c"

#include "txtscrio/Frame.c"

#if GO32

#else // !GO32

#include "txtscrio/GetCursor.c"

#endif // !GO32

#include "txtscrio/ScreenMemCopy.c"
#include "txtscrio/move2screen_alt.c"
