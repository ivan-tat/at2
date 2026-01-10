// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef STRINGIO_H
#define STRINGIO_H

#include "pascal.h"
#include "common.h"

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

String_t asciiz_string (const String *str);

#if !ADT2PLAY

// UCS-2 `code page'

#define UCS2_CP_SIZE 0x4000

// UCS-2 upper/lower case flags
extern uint8_t UCS2_upper_case_f[(UCS2_CP_SIZE + 7) / 8];
extern uint8_t UCS2_lower_case_f[(UCS2_CP_SIZE + 7) / 8];

#define UCS2IsUpper(c) bit_test (UCS2_upper_case_f, c)
#define UCS2IsLower(c) bit_test (UCS2_lower_case_f, c)

// UCS-2 upper/lower case conversion tables
extern uint16_t UCS2_upper_case_ct[UCS2_CP_SIZE];
extern uint16_t UCS2_lower_case_ct[UCS2_CP_SIZE];

#define UCS2UpCase(c) UCS2_upper_case_ct[c]
#define UCS2LoCase(c) UCS2_lower_case_ct[c]

// ASCII (intersects with `AT2' code page)

extern CharSet_t DEC_NUM_CHARSET;
extern CharSet_t HEX_NUM_CHARSET;

#endif // !ADT2PLAY

#if DEBUG

bool IsUpper (uint8_t c);
bool IsLower (uint8_t c);
uint8_t UpCase (uint8_t c);
uint8_t LoCase (uint8_t c);

#else // !DEBUG

// Inline versions (depends on `DEBUG' macro)
#include "stringio/IsUpper.c"
#include "stringio/IsLower.c"
#include "stringio/UpCase.c"
#include "stringio/LoCase.c"

#endif // !DEBUG

#if !ADT2PLAY

// `AT2' code page

#define AT2_CP_SIZE 0x100

typedef uint8_t AT2_char_t;

typedef CharSet_t tCHARSET;

// UCS-2 to `AT2' code page conversion table
extern AT2_char_t UCS2_to_AT2_ct[UCS2_CP_SIZE];

// `AT2' code page upper/lower case flags
extern uint8_t AT2_upper_case_f[(AT2_CP_SIZE + 7) / 8];
extern uint8_t AT2_lower_case_f[(AT2_CP_SIZE + 7) / 8];

#define AT2IsUpper(c) bit_test (AT2_upper_case_f, c)
#define AT2IsLower(c) bit_test (AT2_lower_case_f, c)

// `AT2' code page upper/lower case conversion tables
extern AT2_char_t AT2_upper_case_ct[AT2_CP_SIZE];
extern AT2_char_t AT2_lower_case_ct[AT2_CP_SIZE];

#define AT2UpCase(c) AT2_upper_case_ct[c]
#define AT2LoCase(c) AT2_lower_case_ct[c]

// UTF-8

AT2_char_t *UTF8str_to_AT2 (AT2_char_t *dest, const char *s);
AT2_char_t *UTF8nstr_to_AT2 (AT2_char_t *dest, size_t size, const char *s);

// Mixed CP437 and `AT2' code page, use with caution

uint8_t CStrLen (const String *str);
uint8_t CStr2Len (const String *str);
uint8_t C3StrLen (const String *str);

#endif // !ADT2PLAY

String_t byte2hex (uint8_t value);
String_t byte2dec (uint8_t value);
String_t Capitalize (const String *str);
String_t Upper (const String *str);
String_t Lower (const String *str);
#if !ADT2PLAY
String_t iCASE (const String *str);
String_t RotStrL (const String *str1, const String *str2, uint8_t shift);
String_t RotStrR (const String *str1, const String *str2, uint8_t shift);
#endif // !ADT2PLAY
String_t ExpStrL (const String *str, uint8_t size, char chr);
String_t ExpStrR (const String *str, uint8_t size, char chr);
#if !ADT2PLAY
String_t ExpCStrR (const String *str, uint8_t size, char chr);
String_t ExpC2StrL (const String *str, uint8_t size, char chr);
String_t ExpC2StrR (const String *str, uint8_t size, char chr);
String_t ExpC3StrL (const String *str, uint8_t size, char chr);
String_t ExpC3StrR (const String *str, uint8_t size, char chr);
String_t CenterStr (const String *str, uint8_t size);
#endif // !ADT2PLAY
String_t DietStr (const String *str, uint8_t size);
String_t CutStr (const String *str);
#if !ADT2PLAY
String_t CutStrL (const String *str, uint8_t margin);
String_t CutStrR (const String *str, uint8_t margin);
String_t FlipStr (const String *str);
String_t FilterStr1 (const String *str, char chr0);
#endif // !ADT2PLAY
String_t FilterStr (const String *str, char chr0, char chr1);
#if !ADT2PLAY
String_t FilterStr2 (const String *str, const uint8_t *charset, char chr);
#endif // !ADT2PLAY
String_t Num2str (uint32_t num, uint8_t base);
int32_t Str2num (const String *str, uint8_t base);
#if !ADT2PLAY
String_t Bpm2str (double bpm);
String *number_with_commas (String *dst, uint32_t num, uint8_t base);
String *remove_spaces_right (String *s);
#endif // !ADT2PLAY

String_t truncate_string (const String *s);

#if !ADT2PLAY
String_t Upper_filename (const String *str);
#endif // !ADT2PLAY
String_t Lower_filename (const String *str);
#if !ADT2PLAY
String_t iCASE_filename (const String *str);

ssize_t FindPart (const String *wilds, const String *str);
bool IsWild (const String *str, const String *wilds, bool ignore_case);
bool SameName (const String *mask, const String *str);
#endif // !ADT2PLAY

void DoDirSeparators (String *p);
#if !ADT2PLAY
void FSplit (const String *path, String *dir, String *name, String *ext);
#endif // !ADT2PLAY
String_t PathOnly (const String *path);
String_t NameOnly (const String *path);
String_t BaseNameOnly (const String *path);
String_t ExtOnly (const String *path);

void init_StringIO (void);

#endif // !defined(STRINGIO_H)
