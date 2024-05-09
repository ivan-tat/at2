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

// UCS-2

#include "stringio/UCS2_case_conv_it.c"

uint8_t UCS2_upper_case_f[(UCS2_CP_SIZE + 7) / 8];
uint8_t UCS2_lower_case_f[(UCS2_CP_SIZE + 7) / 8];

uint16_t UCS2_upper_case_ct[UCS2_CP_SIZE];
uint16_t UCS2_lower_case_ct[UCS2_CP_SIZE];

// ASCII (intersects with `AT2' code page)

CharSet_t DEC_NUM_CHARSET;
CharSet_t HEX_NUM_CHARSET;

// `AT2' code page

#include "stringio/CP437_it.c"
#include "stringio/CP437_case_conv_it.c"

AT2_char_t UCS2_to_AT2_ct[UCS2_CP_SIZE];

uint8_t AT2_upper_case_f[(AT2_CP_SIZE + 7) / 8];
uint8_t AT2_lower_case_f[(AT2_CP_SIZE + 7) / 8];

AT2_char_t AT2_upper_case_ct[AT2_CP_SIZE];
AT2_char_t AT2_lower_case_ct[AT2_CP_SIZE];

// UTF-8

#include "stringio/UTF8_to_wchar.c"
#include "stringio/UTF8str_to_AT2.c"
#include "stringio/UTF8nstr_to_AT2.c"

// Mixed CP437 and `AT2' code page, use with caution

#if !ADT2PLAY
#include "stringio/CStrLen.c"
#include "stringio/CStr2Len.c"
#include "stringio/C3StrLen.c"
#endif // !ADT2PLAY

#include "stringio/byte2hex.c"
#include "stringio/byte2dec.c"
#include "stringio/Capitalize.c"
#include "stringio/Upper.c"
#include "stringio/Lower.c"
#include "stringio/ExpStrL.c"
#include "stringio/ExpStrR.c"
#include "stringio/DietStr.c"
#include "stringio/CutStr.c"
#if !ADT2PLAY
#include "stringio/CutStrR.c"
#endif // !ADT2PLAY
#include "stringio/FilterStr.c"
#if !ADT2PLAY
#include "stringio/FilterStr2.c"
#endif // !ADT2PLAY
#include "stringio/Num2str.c"
#include "stringio/Str2num.c"

#include "stringio/init_StringIO.c"
