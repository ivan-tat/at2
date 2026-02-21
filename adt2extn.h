// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef ADT2EXTN_H
#define ADT2EXTN_H

#include "defines.h"
#include <stdint.h>
#include "adt2keyb.h"

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

extern uint8_t remap_mtype;
extern uint8_t remap_ins1;
extern uint8_t remap_ins2;
extern uint8_t remap_selection;
extern uint8_t rearrange_selection;
extern uint8_t replace_selection;
extern bool    replace_prompt;
extern struct pattern_event_replace_data_s
{
  struct
  {
    String GCC_ATTRIBUTE((nonstring)) note[3+1];
    String GCC_ATTRIBUTE((nonstring)) inst[2+1];
    String GCC_ATTRIBUTE((nonstring)) fx_1[3+1];
    String GCC_ATTRIBUTE((nonstring)) fx_2[3+1];
  } event_to_find;
  struct
  {
    String GCC_ATTRIBUTE((nonstring)) note[3+1];
    String GCC_ATTRIBUTE((nonstring)) inst[2+1];
    String GCC_ATTRIBUTE((nonstring)) fx_1[3+1];
    String GCC_ATTRIBUTE((nonstring)) fx_2[3+1];
  } new_event;
} replace_data;

extern ExtKeyCode fkey;

extern uint8_t tracing_block_pattern;
extern uint8_t tracing_block_xend;
extern uint8_t tracing_block_yend;

extern uint8_t copypos1;
extern uint8_t copypos2;
extern uint8_t copypos3;
extern uint8_t copypos4;
extern uint8_t clearpos;
extern uint8_t pattern_list__page;
extern uint8_t pattern2use;

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

#pragma pack(pop)

#endif // !defined(ADT2EXTN_H)
