// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "adt2sys.h"
#include "adt2unit.h"
#include "adt2ext3.h"
#include "dialogio.h"
#include "stringio.h"
#include "txtscrio.h"
#include "adt2extn.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

uint8_t remap_mtype         = 1;
uint8_t remap_ins1          = 1;
uint8_t remap_ins2          = 1;
uint8_t remap_selection     = 1;
uint8_t rearrange_selection = 1;
uint8_t replace_selection   = 1;
bool    replace_prompt = false;
struct pattern_event_replace_data_s replace_data =
{
  .event_to_find =
  {
    .note = { "\x03" "???" },
    .inst = { "\x02" "??" },
    .fx_1 = { "\x03" "???" },
    .fx_2 = { "\x03" "???" }
  },
  .new_event =
  {
    .note = { "\x03" "???" },
    .inst = { "\x02" "??" },
    .fx_1 = { "\x03" "???" },
    .fx_2 = { "\x03" "???" }
  }
};

ExtKeyCode fkey;

uint8_t tracing_block_pattern;
uint8_t tracing_block_xend;
uint8_t tracing_block_yend;

uint8_t copypos1 = 1;
uint8_t copypos2 = 1;
uint8_t copypos3 = 1;
uint8_t copypos4 = 1;
uint8_t clearpos = 1;
uint8_t pattern_list__page = 1;
uint8_t pattern2use = UINT8_NULL;

#include "adt2extn/_patts_marked.c"

#include "adt2extn/nul_volume_bars.c"

/*static*/ bool    transp_menu2 = false;
/*static*/ uint8_t transp_pos1 = 1;
/*static*/ uint8_t transp_pos2 = 1;

#include "adt2extn/fselect_external_proc.c" // HINT: static

#pragma pack(pop)
