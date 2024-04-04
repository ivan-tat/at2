// This file is part of Adlib Tracker II (AT2).
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

// HINT: (FPC) $PACKRECORDS 1: Alignment of record elements (1)

extern uint16_t SCREEN_RES_X;
extern uint16_t SCREEN_RES_Y;
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
extern uint8_t work_MaxCol;
extern uint8_t work_MaxLn;
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

void ScreenMemCopy (tSCREEN_MEM *src, tSCREEN_MEM *dest);
//move2screen
void move2screen_alt (void);
void TxtScrIO_Init (void);
bool is_default_screen_mode (void);
#if GO32
bool is_VESA_emulated_mode (void);
uint8_t get_VESA_emulated_mode_idx (void);
#endif // GO32
bool is_scrollable_screen_mode (void);

#pragma pack(push, 1)
typedef struct {
  bool shadow_enabled;
  bool wide_range_type;
  bool zooming_enabled;
  bool update_area;
} tFRAME_SETTING;
#pragma pack(pop)

extern tFRAME_SETTING fr_setting;

void Frame (tSCREEN_MEM *dest, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
            uint8_t attr1, const String *title, uint8_t attr2,
            const String *border);

uint8_t WhereX (void);
uint8_t WhereY (void);
void GotoXY (uint8_t x, uint8_t y);
int32_t GetCursor (void);
void SetCursor (int32_t cursor);
void ThinCursor (void);
void WideCursor (void);
void HideCursor (void);
uint16_t GetCursorShape (void);
void SetCursorShape (uint16_t shape);

extern uint16_t v_seg;
extern uint16_t v_ofs;
extern uint8_t  v_mode;

#if GO32

extern uint8_t DispPg;

typedef uint8_t tCUSTOM_VIDEO_MODE; // 0..52

bool iVGA (void);
void initialize (void);
void ResetMode (void);
void SetCustomVideoMode (tCUSTOM_VIDEO_MODE vmode);
void GetRGBitem (uint8_t color, uint8_t *red, uint8_t *green, uint8_t *blue);
void SetRGBitem (uint8_t color, uint8_t red, uint8_t green, uint8_t blue);
void WaitRetrace (void);
void GetPalette (void *pal, uint8_t first, uint8_t last);
void SetPalette (void *pal, uint8_t first, uint8_t last);

typedef enum {
  fadeFirst,
  fadeOut,
  fadeIn
} tFADE;

typedef enum {
  fast,
  delayed
} tDELAY;

#pragma pack(push, 1)
typedef struct {
  tFADE action;
  struct {
    uint8_t r, g, b;
  } pal0[256], pal1[256]; // HINT: (FPC) start index 0
} tFADE_BUF;
#pragma pack(pop)

extern uint8_t fade_speed;

void VgaFade (tFADE_BUF *data, tFADE fade, tDELAY dly);
void RefreshEnable (void);
void RefreshDisable (void);
void Split2Static (void);
void SplitScr (uint16_t line);
void SetSize (uint16_t columns, uint16_t lines);
void SetTextDisp (uint16_t x, uint16_t y);
void set_vga_txtmode_80x25 (void);
void set_svga_txtmode_100x38 (void);
void set_svga_txtmode_128x48 (void);
void set_custom_svga_txtmode (void);

#pragma pack(push, 1)
typedef struct {
  uint16_t port;
  uint8_t idx;
  uint8_t val;
} VGA_REGISTER;
#pragma pack(pop)

typedef VGA_REGISTER VGA_REG_DATA[29]; // HINT: (FPC) start index 1

extern uint8_t svga_txtmode_cols;
extern uint8_t svga_txtmode_rows;
extern VGA_REG_DATA svga_txtmode_regs;

#endif // GO32

#endif // !defined(TXTSCRIO_H)
