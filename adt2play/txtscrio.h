// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef TXTSCRIO_H
#define TXTSCRIO_H

#include <stdint.h>

// HINT: (FPC) $PACKRECORDS 1: Alignment of record elements (1)

#include "txtscrio/colors.h"

#define MAX_SCREEN_MEM_SIZE (180 * 60 * 2)
extern int32_t SCREEN_MEM_SIZE;

typedef uint8_t tSCREEN_MEM[MAX_SCREEN_MEM_SIZE]; // HINT: (FPC) start index 0

extern tSCREEN_MEM text_screen_shadow;

extern void    *screen_ptr;
extern uint16_t v_seg;
extern uint16_t v_ofs;
extern uint8_t  v_mode;
extern uint8_t  MaxCol;
extern uint8_t  MaxLn;

extern int32_t cursor_backup;
extern uint8_t DispPg;

#include "txtscrio/go32/fade.h"

#pragma pack(push, 1)
typedef struct {
  int32_t cursor;
  uint8_t font;
  uint8_t MaxLn, MaxCol, v_mode, DispPg;
  uint16_t v_seg, v_ofs;
  tSCREEN_MEM screen;
  uint8_t data[4096]; // HINT: (FPC) start index 0
} tVIDEO_STATE;
#pragma pack(pop)

#endif // !defined(TXTSCRIO_H)
