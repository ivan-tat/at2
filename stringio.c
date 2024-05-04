// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"

#include "stringio.h"
#if USE_FPC
#include "pascal/string.h"
#else // !USE_FPC
#include <string.h>
#endif // !USE_FPC

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

// UCS-2 to `AT2' code page conversion table
static uint8_t UCS2_to_AT2_ct[0x10000];

// `AT2' upper case conversion table
uint8_t AT2_upper_case_ct[0x100];

// `AT2' lower case conversion table
uint8_t AT2_lower_case_ct[0x100];

CharSet_t DEC_NUM_CHARSET = { // '0'..'9'
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  MK_UINT8 (1,1,1,1,1,1,1,1), MK_UINT8 (1,1,0,0,0,0,0,0), // '0'..'9'
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00
};

CharSet_t HEX_NUM_CHARSET = { // '0'..'9', 'A'..'F', 'a'..'f'
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  MK_UINT8 (1,1,1,1,1,1,1,1), MK_UINT8 (1,1,0,0,0,0,0,0), // '0'..'9'
  MK_UINT8 (0,1,1,1,1,1,1,0), MK_UINT8 (0,0,0,0,0,0,0,0), // 'A'..'F'
  0x00, 0x00,
  MK_UINT8 (0,1,1,1,1,1,1,0), MK_UINT8 (0,0,0,0,0,0,0,0), // 'a'..'f'
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00
};

#include "stringio/CP437_it.c"
#include "stringio/CP437_case_conv_it.c"

#include "stringio/UTF8_to_wchar.c"
#include "stringio/UTF8str_to_AT2.c"
#include "stringio/UTF8nstr_to_AT2.c"

#if !ADT2PLAY
#include "stringio/CStrLen.c"
#include "stringio/CStr2Len.c"
#include "stringio/C3StrLen.c"
#endif // !ADT2PLAY

#include "stringio/ExpStrR.c"
#if !ADT2PLAY
#include "stringio/CutStrR.c"
#include "stringio/FilterStr2.c"
#endif // !ADT2PLAY
#include "stringio/Num2str.c"
#include "stringio/Str2num.c"

#include "stringio/init_StringIO.c"
