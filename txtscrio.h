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
#endif // USE_FPC
#if !ADT2PLAY
#include "stringio.h"
#endif // !ADT2PLAY

// HINT: (FPC) $PACKRECORDS 1: Alignment of record elements (1)

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

#define MAX_SCREEN_MEM_SIZE (180 * 60 * 2)
extern int32_t  SCREEN_MEM_SIZE;
#if !ADT2PLAY
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
#endif // !ADT2PLAY

typedef uint8_t tSCREEN_MEM[MAX_SCREEN_MEM_SIZE]; // HINT: (FPC) start index 0

extern tSCREEN_MEM text_screen_shadow;
#if !ADT2PLAY
extern tSCREEN_MEM temp_screen;
extern tSCREEN_MEM temp_screen2;
extern tSCREEN_MEM screen_backup;
extern tSCREEN_MEM scr_backup;
extern tSCREEN_MEM scr_backup2;
extern tSCREEN_MEM screen_mirror;
extern tSCREEN_MEM screen_emulator;
extern tSCREEN_MEM *centered_frame_vdest;
#endif // !ADT2PLAY

extern void *screen_ptr;
#if !ADT2PLAY
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
#endif // !ADT2PLAY

extern uint8_t MaxCol;
extern uint8_t MaxLn;
#if !ADT2PLAY
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
#endif // !ADT2PLAY

extern int32_t cursor_backup;

#if !ADT2PLAY
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
#endif // !ADT2PLAY
void ShowC3Str (tSCREEN_MEM *dest, uint8_t x, uint8_t y, const String *str,
                uint8_t attr1, uint8_t attr2, uint8_t attr3);
#if !ADT2PLAY
void ShowVC3Str (tSCREEN_MEM *dest, uint8_t x, uint8_t y, const String *str,
                 uint8_t attr1, uint8_t attr2, uint8_t attr3);
void ShowC4Str (tSCREEN_MEM *dest, uint8_t x, uint8_t y, const String *str,
                uint8_t attr1, uint8_t attr2, uint8_t attr3, uint8_t attr4);

void ScreenMemCopy (tSCREEN_MEM *src, tSCREEN_MEM *dest);
void move2screen (void);
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
#endif // !ADT2PLAY

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

#if GO32

#if !ADT2PLAY
typedef uint8_t tCUSTOM_VIDEO_MODE; // 0..52
#endif // !ADT2PLAY

bool iVGA (void);
void initialize (void);
#if !ADT2PLAY
void ResetMode (void);
void SetCustomVideoMode (tCUSTOM_VIDEO_MODE vmode);
#endif // !ADT2PLAY
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

#if !ADT2PLAY

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

#else // ADT2PLAY

int GetVideoState (VGAVideoState_t *vs, bool save_screen);
void SetVideoState (VGAVideoState_t *vs, bool restore_screen);
void FreeVideoState (VGAVideoState_t *vs);

#endif // ADT2PLAY

#endif // GO32

#if !ADT2PLAY

#pragma pack(push, 1)

typedef struct {
  bool insert_mode;
  bool replace_enabled;
  bool append_enabled;
  CharSet_t char_filter;
  CharSet_t character_set;
  CharSet_t valid_chars;
  CharSet_t word_characters;
  uint16_t terminate_keys[50]; // HINT: (FPC) start index 1
} tINPUT_STR_SETTING;

typedef struct {
  uint16_t keystroke;
  uint8_t locate_pos;
  bool insert_mode;
  uint32_t min_num;
  uint32_t max_num;
  String cur_str[255+1];
  void (*ext_proc) (void);
} tINPUT_STR_ENVIRONMENT;

#pragma pack(pop)

extern tINPUT_STR_SETTING is_setting;
extern tINPUT_STR_ENVIRONMENT is_environment;

String_t InputStr (const String *s, uint8_t x, uint8_t y, uint8_t ln,
                   uint8_t ln1, uint8_t attr1, uint8_t attr2);
#endif // !ADT2PLAY

#endif // !defined(TXTSCRIO_H)
