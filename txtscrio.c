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
#include "pascal/pc.h"
#endif // GO32
#include "pascal/dos.h"
#include "pascal/string.h"
#else // !USE_FPC
#if GO32
#include <pc.h>
#endif // GO32
#include <dos.h>
#include <string.h>
#endif // !USE_FPC
#if GO32
#include "go32/adt2dpmi.h"
#if !ADT2PLAY
#include "go32/adt2vesa.h"
#endif // !ADT2PLAY
#include "go32/BIOS.h"
#include "go32/VBIOS.h"
#include "go32/VGA.h"
#endif // GO32
#if !ADT2PLAY
#include "adt2ext2.h"
#include "adt2sys.h"
#include "adt2unit.h"
#include "dialogio.h"
#endif // !ADT2PLAY
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

#if !ADT2PLAY
uint16_t SCREEN_RES_X = 720;
uint16_t SCREEN_RES_Y = 480;
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
#endif // !ADT2PLAY

tSCREEN_MEM text_screen_shadow;
#if !ADT2PLAY
tSCREEN_MEM temp_screen;
tSCREEN_MEM temp_screen2;
tSCREEN_MEM screen_backup;
tSCREEN_MEM scr_backup;
tSCREEN_MEM scr_backup2;
tSCREEN_MEM screen_mirror;
tSCREEN_MEM screen_emulator;
tSCREEN_MEM *centered_frame_vdest;
#endif // !ADT2PLAY

void *screen_ptr          = &text_screen_shadow;
#if !ADT2PLAY
void *ptr_temp_screen     = &temp_screen;
void *ptr_temp_screen2    = &temp_screen2;
void *ptr_screen_backup   = &screen_backup;
void *ptr_scr_backup      = &scr_backup;
void *ptr_scr_backup2     = &scr_backup2;
void *ptr_screen_mirror   = &screen_mirror;
void *ptr_screen_emulator = &screen_emulator;

void   *move_to_screen_data = NULL;
uint8_t move_to_screen_area[4] = { 0, 0, 0, 0 };
void  (*move_to_screen_routine) () = NULL;

uint8_t program_screen_mode = 0;
#endif // !ADT2PLAY

uint8_t MaxCol = 0;
uint8_t MaxLn = 0;
#if !ADT2PLAY
uint8_t hard_maxcol = 0;
uint8_t hard_maxln = 0;
uint8_t work_MaxCol = 0;
uint8_t work_MaxLn = 0;
uint8_t scr_font_width = 0;
uint8_t scr_font_height = 0;

uint8_t area_x1 = 0;
uint8_t area_y1 = 0;
uint8_t area_x2 = 0;
uint8_t area_y2 = 0;
uint8_t scroll_pos0 = UINT8_NULL;
uint8_t scroll_pos1 = UINT8_NULL;
uint8_t scroll_pos2 = UINT8_NULL;
uint8_t scroll_pos3 = UINT8_NULL;
uint8_t scroll_pos4 = UINT8_NULL;
#endif // !ADT2PLAY

int32_t cursor_backup;

#if !ADT2PLAY
#include "txtscrio/show_str.c"
#include "txtscrio/show_cstr.c"
#include "txtscrio/show_cstr_alt.c"
#include "txtscrio/show_vstr.c"
#include "txtscrio/show_vcstr.c"
#endif // !ADT2PLAY

static uint16_t absolute_pos;

#include "txtscrio/PosChar.c"
#if !ADT2PLAY
#include "txtscrio/DupChar.c"

#include "txtscrio/ShowStr.c"
#include "txtscrio/ShowVStr.c"
#include "txtscrio/ShowCStr.c"
#include "txtscrio/ShowVCStr.c"
#include "txtscrio/ShowCStr2.c"
#include "txtscrio/ShowVCStr2.c"
#endif // !ADT2PLAY
#include "txtscrio/ShowC3Str.c"
#if !ADT2PLAY
#include "txtscrio/ShowVC3Str.c"
#include "txtscrio/ShowC4Str.c"

#include "txtscrio/ScreenMemCopy.c"
#include "txtscrio/move2screen.c"
#include "txtscrio/move2screen_alt.c"
#include "txtscrio/TxtScrIO_Init.c"
#include "txtscrio/is_default_screen_mode.c"
#if GO32
#include "txtscrio/go32/is_VESA_emulated_mode.c"
#include "txtscrio/go32/get_VESA_emulated_mode_idx.c"
#endif // GO32
#include "txtscrio/is_scrollable_screen_mode.c"

tFRAME_SETTING fr_setting = {
  .shadow_enabled = true,
  .wide_range_type = false,
  .zooming_enabled = false,
  .update_area = true
};

#include "txtscrio/Frame.c"
#endif // !ADT2PLAY

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

#if GO32

uint16_t v_seg = 0xB800;
uint16_t v_ofs = 0;
uint8_t  v_mode = 3;

uint8_t DispPg;

#include "txtscrio/go32/iVGA.c"
#include "txtscrio/go32/initialize.c"
#if !ADT2PLAY
#include "txtscrio/go32/ResetMode.c"
#include "txtscrio/go32/SetCustomVideoMode.c"
#endif // !ADT2PLAY
#include "txtscrio/go32/GetRGBitem.c"
#include "txtscrio/go32/SetRGBitem.c"
#include "txtscrio/go32/GetPalette.c"
#include "txtscrio/go32/SetPalette.c"
#include "txtscrio/go32/WaitRetrace.c"

uint8_t fade_speed = 63;

#include "txtscrio/go32/VgaFade.c"

#if !ADT2PLAY

#include "txtscrio/go32/RefreshEnable.c"
#include "txtscrio/go32/RefreshDisable.c"
#include "txtscrio/go32/Split2Static.c"
#include "txtscrio/go32/SplitScr.c"
#include "txtscrio/go32/SetSize.c"
#include "txtscrio/go32/SetTextDisp.c"

#include "txtscrio/go32/set_vga_txtmode_80x25.c"
#include "txtscrio/go32/set_svga_txtmode.c"
#include "txtscrio/go32/set_svga_txtmode_100x38.c"
#include "txtscrio/go32/set_svga_txtmode_128x48.c"

uint8_t svga_txtmode_cols = 100;
uint8_t svga_txtmode_rows = 37;
VGA_REG_DATA svga_txtmode_regs = {
  { .port = VGA_MISC_WRITE_PORT, .idx = 0x00, .val = 0x6B }, // Miscellaneous output
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x00, .val = 0x70 }, // Horizontal total
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x01, .val = 0x63 }, // Horizontal display enable end
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x02, .val = 0x64 }, // Horizontal blank start
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x03, .val = 0x82 }, // Horizontal blank end
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x04, .val = 0x65 }, // Horizontal retrace start
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x05, .val = 0x82 }, // Horizontal retrace end
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x06, .val = 0x70 }, // Vertical total
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x07, .val = 0xF0 }, // Overflow register
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x08, .val = 0x00 }, // Preset row scan
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x09, .val = 0x4F }, // Maximum scan line/char height
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x10, .val = 0x5B }, // Vertical retrace start
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x11, .val = 0x8C }, // Vertical retrace end
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x12, .val = 0x4F }, // Vertical display enable end
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x13, .val = 0x3C }, // Offset/logical width
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x14, .val = 0x00 }, // Underline location
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x15, .val = 0x58 }, // Vertical blank start
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x16, .val = 0x70 }, // Vertical blank end
  { .port = VGA_CRTC_ADDR_PORT, .idx = 0x17, .val = 0xA3 }, // Mode control
  { .port = VGA_SEQ_ADDR_PORT, .idx = 0x01, .val = 0x01 }, // Clock mode register
  { .port = VGA_SEQ_ADDR_PORT, .idx = 0x03, .val = 0x00 }, // Character generator select
  { .port = VGA_SEQ_ADDR_PORT, .idx = 0x04, .val = 0x00 }, // Memory mode register
  { .port = VGA_GRAPH_ADDR_PORT, .idx = 0x05, .val = 0x10 }, // Mode register
  { .port = VGA_GRAPH_ADDR_PORT, .idx = 0x06, .val = 0x0E }, // Miscellaneous register
  { .port = VGA_ATTR_WRITE_PORT, .idx = 0x10, .val = 0x02 }, // Mode control
  { .port = VGA_ATTR_WRITE_PORT, .idx = 0x11, .val = 0x00 }, // Screen border color
  { .port = VGA_ATTR_WRITE_PORT, .idx = 0x12, .val = 0x0F }, // Color plane enable
  { .port = VGA_ATTR_WRITE_PORT, .idx = 0x13, .val = 0x00 }, // Horizontal panning
  { .port = VGA_ATTR_WRITE_PORT, .idx = 0x14, .val = 0x00 } // Color select
};

#include "txtscrio/go32/out_reg.c"
#include "txtscrio/go32/LoadVgaRegisters.c"
#include "txtscrio/go32/set_custom_svga_txtmode.c"

#else // ADT2PLAY

#include "txtscrio/go32/GetVideoState.c"
#include "txtscrio/go32/SetVideoState.c"

#endif // ADT2PLAY

#endif // GO32

#if !ADT2PLAY

tINPUT_STR_SETTING is_setting = {
  .insert_mode = true,
  .replace_enabled = true,
  .append_enabled = true,
  .char_filter = { // 32..255
    0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
  },
  .character_set = { // 32..255
    0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
  },
  .valid_chars = { // 32..255
    0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
  },
  .word_characters = { // '0'..'9', 'A'..'Z', '_', 'a'..'z'
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    MK_UINT8 (1,1,1,1,1,1,1,1), MK_UINT8 (1,1,0,0,0,0,0,0), // '0'..'9'
    MK_UINT8 (0,1,1,1,1,1,1,1), MK_UINT8 (1,1,1,1,1,1,1,1), // 'A'..'O'
    MK_UINT8 (1,1,1,1,1,1,1,1), MK_UINT8 (1,1,1,0,0,0,0,1), // 'P'..'Z', '_'
    MK_UINT8 (0,1,1,1,1,1,1,1), MK_UINT8 (1,1,1,1,1,1,1,1), // 'a'..'o'
    MK_UINT8 (1,1,1,1,1,1,1,1), MK_UINT8 (1,1,1,0,0,0,0,0), // 'p'..'z'
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00
  },
  .terminate_keys = {
    0x011B, 0x1C0D, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000
  }
};

tINPUT_STR_ENVIRONMENT is_environment;

#endif // !ADT2PLAY

#include "txtscrio/init_TxtScrIO.c"
