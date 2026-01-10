// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef ADT2EXTN_H
#define ADT2EXTN_H

#include "defines.h"
#include <stdint.h>
#include "adt2keyb.h"

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

extern ExtKeyCode fkey;

extern uint8_t  progress_xstart, progress_ystart;
extern uint8_t  progress_num_steps;
extern uint8_t  progress_step;
extern uint32_t progress_value;
extern uint8_t  progress_old_value, progress_new_value;

typedef enum
{
  ttTransposeUp,
  ttTransposeDown,
  ttTransposeCurrentIns,
  ttTransposeAllIns
} TRANSPOSE_TYPE_enum;
typedef uint8_t tTRANSPOSE_TYPE; // holds `TRANSPOSE_TYPE_enum'

uint8_t _patts_marked (void);

void nul_volume_bars (void);

void show_progress (int32_t value);
void show_progress2 (int32_t value, int32_t refresh_dif);

#pragma pack(pop)

#endif // !defined(ADT2EXTN_H)
