// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef TXTSCRIO_H
#define TXTSCRIO_H

#include "defines.h"

#include <stdint.h>
#if USE_FPC
#include "pascal.h"
#endif

// HINT: (FPC) $PACKRECORDS 1: Alignment of record elements (1)

#include "txtscrio/colors.h"

#define MAX_SCREEN_MEM_SIZE (180 * 60 * 2)
extern int32_t SCREEN_MEM_SIZE;

typedef uint8_t tSCREEN_MEM[MAX_SCREEN_MEM_SIZE]; // HINT: (FPC) start index 0

extern tSCREEN_MEM text_screen_shadow;

extern void *screen_ptr;

extern uint8_t MaxCol;
extern uint8_t MaxLn;

extern int32_t cursor_backup;

void ShowC3Str (tSCREEN_MEM *dest, uint8_t x, uint8_t y, const String *str,
                uint8_t attr1, uint8_t attr2, uint8_t attr3);

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

extern uint16_t v_seg;
extern uint16_t v_ofs;
extern uint8_t  v_mode;

extern uint8_t DispPg;

bool iVGA (void);
void initialize (void);
void GetRGBitem (uint8_t color, uint8_t *red, uint8_t *green, uint8_t *blue);
void SetRGBitem (uint8_t color, uint8_t red, uint8_t green, uint8_t blue);
void WaitRetrace (void);
void GetPalette (void *pal, uint8_t first, uint8_t last);
void SetPalette (void *pal, uint8_t first, uint8_t last);

#include "txtscrio/go32/fade.h"

void VgaFade (tFADE_BUF *data, tFADE fade, tDELAY dly);

#pragma pack(push, 1)
typedef struct {
  int32_t cursor;
  uint8_t font;
  uint8_t MaxLn, MaxCol, v_mode, DispPg;
  uint16_t v_seg, v_ofs;
  tSCREEN_MEM screen;
  uint8_t data[4096];
} tVIDEO_STATE;
#pragma pack(pop)

void GetVideoState (tVIDEO_STATE *data);
void SetVideoState (tVIDEO_STATE *data, bool restore_screen);

#endif // GO32

#endif // !defined(TXTSCRIO_H)
