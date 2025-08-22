// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"
#include "platform.h"
#include <string.h>
#include "stringio.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

#include "stringio/asciiz_string.c"

#if !ADT2PLAY

// UCS-2

#include "stringio/UCS2_case_conv_it.c"

uint8_t UCS2_upper_case_f[(UCS2_CP_SIZE + 7) / 8];
uint8_t UCS2_lower_case_f[(UCS2_CP_SIZE + 7) / 8];

uint16_t UCS2_upper_case_ct[UCS2_CP_SIZE];
uint16_t UCS2_lower_case_ct[UCS2_CP_SIZE];

// ASCII (intersects with `AT2' code page)

CharSet_t DEC_NUM_CHARSET;
CharSet_t HEX_NUM_CHARSET;

#if DEBUG

// Normal versions (depends on `DEBUG' macro)
#include "stringio/IsUpper.c"
#include "stringio/IsLower.c"
#include "stringio/UpCase.c"
#include "stringio/LoCase.c"

#endif // DEBUG

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

#include "stringio/CStrLen.c"
#include "stringio/CStr2Len.c"
#include "stringio/C3StrLen.c"

#endif // !ADT2PLAY

#include "stringio/byte2hex.c"
#include "stringio/byte2dec.c"
#include "stringio/Capitalize.c"
#include "stringio/Upper.c"
#include "stringio/Lower.c"
#if !ADT2PLAY
#include "stringio/iCASE.c"
#include "stringio/RotStrL.c"
#include "stringio/RotStrR.c"
#endif // !ADT2PLAY
#include "stringio/ExpStrL.c"
#include "stringio/ExpStrR.c"
#if !ADT2PLAY
#include "stringio/ExpCStrR.c"
#include "stringio/ExpC2StrL.c"
#include "stringio/ExpC2StrR.c"
#include "stringio/ExpC3StrL.c"
#include "stringio/ExpC3StrR.c"
#include "stringio/CenterStr.c"
#endif // !ADT2PLAY
#include "stringio/DietStr.c"
#include "stringio/CutStr.c"
#if !ADT2PLAY
#include "stringio/CutStrL.c"
#include "stringio/CutStrR.c"
#include "stringio/FlipStr.c"
#endif // !ADT2PLAY
#include "stringio/FilterStr.c"
#if !ADT2PLAY
#include "stringio/FilterStr1.c"
#include "stringio/FilterStr2.c"
#endif // !ADT2PLAY
#include "stringio/Num2str.c"
#include "stringio/Str2num.c"
#if !ADT2PLAY
#include "stringio/Bpm2str.c"
#include "stringio/number_with_commas.c"
#include "stringio/remove_spaces_right.c"

#include "stringio/Upper_filename.c"
#endif // !ADT2PLAY
#include "stringio/Lower_filename.c"
#if !ADT2PLAY
#include "stringio/iCASE_filename.c"

#include "stringio/FindPart.c"
#include "stringio/IsWild.c"
#include "stringio/SameName.c"
#endif // !ADT2PLAY

#include "stringio/DoDirSeparators.c"
#include "stringio/_FSplit.c"
#if !ADT2PLAY
#include "stringio/FSplit.c"
#endif // !ADT2PLAY
#include "stringio/PathOnly.c"
#include "stringio/NameOnly.c"
#include "stringio/BaseNameOnly.c"
#include "stringio/ExtOnly.c"

#include "stringio/init_StringIO.c"
