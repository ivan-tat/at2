//  This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef TXTSCRIO_H
#define TXTSCRIO_H

#include "defines.h"

#include <stdbool.h>
#include <stdint.h>
#if USE_FPC
#include "pascal.h"
#endif

// HINT: (FPC 2.6.4+GO32V2) always define external variables as public (bug)
// HINT: (FPC) $PACKRECORDS 1: Alignment of record elements (1)

extern uint16_t SCREEN_RES_x;
extern uint16_t SCREEN_RES_y;
extern uint8_t  MAX_COLUMNS;
extern uint8_t  MAX_ROWS;
extern uint8_t  MAX_TRACKS;
extern uint8_t  MAX_ORDER_COLS;
extern uint8_t  MAX_PATTERN_ROWS;
extern uint8_t  INSCTRL_xshift;
extern int8_t   INSCTRL_yshift;
extern uint8_t  INSEDIT_yshift;
extern uint8_t  PATTORD_xshift;
extern uint8_t  GOTOXY_xshift;

#define MAX_SCREEN_MEM_SIZE 180 * 60 * 2
extern int32_t SCREEN_MEM_SIZE;

typedef uint8_t tSCREEN_MEM[MAX_SCREEN_MEM_SIZE]; // HINT: (FPC) start index 0

extern tSCREEN_MEM temp_screen;
extern tSCREEN_MEM temp_screen2;
extern tSCREEN_MEM screen_backup;
extern tSCREEN_MEM scr_backup;
extern tSCREEN_MEM scr_backup2;
extern tSCREEN_MEM screen_mirror;
extern tSCREEN_MEM screen_emulator;
extern tSCREEN_MEM *centered_frame_vdest;
extern tSCREEN_MEM text_screen_shadow;

extern void *screen_ptr;
extern void *ptr_temp_screen;
extern void *ptr_temp_screen2;
extern void *ptr_screen_backup;
extern void *ptr_scr_backup;
extern void *ptr_scr_backup2;
extern void *ptr_screen_mirror;
extern void *ptr_screen_emulator;

extern void   *move_to_screen_data;
extern uint8_t move_to_screen_area[4]; // HINT: (FPC) start index 1
extern void  (*move_to_screen_routine) ();

extern uint8_t program_screen_mode;

extern uint8_t MaxLn;
extern uint8_t MaxCol;
extern uint8_t hard_maxcol;
extern uint8_t hard_maxln;
extern uint8_t work_maxcol;
extern uint8_t work_maxln;
extern uint8_t scr_font_width;
extern uint8_t scr_font_height;

extern uint8_t area_x1;
extern uint8_t area_y1;
extern uint8_t area_x2;
extern uint8_t area_y2;
extern uint8_t scroll_pos0;
extern uint8_t scroll_pos1;
extern uint8_t scroll_pos2;
extern uint8_t scroll_pos3;
extern uint8_t scroll_pos4;

extern int32_t cursor_backup;

#define Black    0x00
#define Blue     0x01
#define Green    0x02
#define Cyan     0x03
#define Red      0x04
#define Magenta  0x05
#define Brown    0x06
#define LGray    0x07
#define DGray    0x08
#define LBlue    0x09
#define LGreen   0x0A
#define LCyan    0x0B
#define LRed     0x0C
#define LMagenta 0x0D
#define Yellow   0x0E
#define White    0x0F
#define Blink    0x80

#pragma pack(push, 1)
typedef struct {
  bool shadow_enabled;
  bool wide_range_type;
  bool zooming_enabled;
  bool update_area;
} tFRAME_SETTING;
#pragma pack(pop)

extern tFRAME_SETTING fr_setting;

extern uint16_t v_seg;
extern uint16_t v_ofs;
extern uint8_t  v_mode;

void show_str (uint8_t xpos, uint8_t ypos, const String *str, uint8_t attr);
void show_cstr (uint8_t xpos, uint8_t ypos, const String *str, uint8_t attr1,
                uint8_t attr2);
void show_cstr_alt (uint8_t xpos, uint8_t ypos, const String *str,
                    uint8_t attr1, uint8_t attr2);
void show_vstr (uint8_t xpos, uint8_t ypos, const String *str, uint8_t attr);
void show_vcstr (uint8_t xpos, uint8_t ypos, const String *str, uint8_t attr1,
                 uint8_t attr2);

void ShowStr (tSCREEN_MEM *dest, uint8_t x, uint8_t y, const String *str,
              uint8_t attr);
void ShowVStr (tSCREEN_MEM *dest, uint8_t x, uint8_t y, const String *str,
               uint8_t attr);
void ShowCStr (tSCREEN_MEM *dest, uint8_t x, uint8_t y, const String *str,
               uint8_t attr1, uint8_t attr2);
void ShowVCStr (tSCREEN_MEM *dest, uint8_t x, uint8_t y, const String *str,
                uint8_t attr1, uint8_t attr2);
void ShowCStr2 (tSCREEN_MEM *dest, uint8_t x, uint8_t y, const String *str,
                uint8_t attr1, uint8_t attr2);
void ShowVCStr2 (tSCREEN_MEM *dest, uint8_t x, uint8_t y, const String *str,
                 uint8_t attr1, uint8_t attr2);
void ShowC3Str (tSCREEN_MEM *dest, uint8_t x, uint8_t y, const String *str,
                uint8_t attr1, uint8_t attr2, uint8_t attr3);
void ShowVC3Str (tSCREEN_MEM *dest, uint8_t x, uint8_t y, const String *str,
                 uint8_t attr1, uint8_t attr2, uint8_t attr3);
void ShowC4Str (tSCREEN_MEM *dest, uint8_t x, uint8_t y, const String *str,
                uint8_t attr1, uint8_t attr2, uint8_t attr3, uint8_t attr4);

uint8_t CStrLen (const String *str);
uint8_t CStr2Len (const String *str);
uint8_t C3StrLen (const String *str);

void Frame (tSCREEN_MEM *dest, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
            uint8_t attr1, const String *title, uint8_t attr2,
            const String *border);

int32_t GetCursor (void);

void ScreenMemCopy (tSCREEN_MEM *src, tSCREEN_MEM *dest);
void move2screen_alt (void);

#endif // !defined(TXTSCRIO_H)
