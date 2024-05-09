// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef STRINGIO_H
#define STRINGIO_H

#include "pascal.h"
#include "common.h"

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

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

__inline__ bool IsUpper (uint8_t c) {
  return (c >= 'A') && (c <= 'Z');
}

__inline__ bool IsLower (uint8_t c) {
  return (c >= 'a') && (c <= 'z');
}

__inline__ uint8_t UpCase (uint8_t c) {
  return IsLower (c) ? c - 'a' + 'A' : c;
}

__inline__ uint8_t LoCase (uint8_t c) {
  return IsUpper (c) ? c - 'A' + 'a' : c;
}

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

#if !ADT2PLAY
uint8_t CStrLen (const String *str);
uint8_t CStr2Len (const String *str);
uint8_t C3StrLen (const String *str);
#endif // !ADT2PLAY

String_t byte2hex (uint8_t value);
String_t byte2dec (uint8_t value);
String_t Capitalize (const String *str);
String_t Upper (const String *str);
String_t Lower (const String *str);
String_t ExpStrL (const String *str, uint8_t size, char chr);
String_t ExpStrR (const String *str, uint8_t size, char chr);
String_t DietStr (const String *str, uint8_t size);
String_t CutStr (const String *str);
#if !ADT2PLAY
String_t CutStrR (const String *str, uint8_t margin);
#endif // !ADT2PLAY
String_t FilterStr (const String *str, char chr0, char chr1);
#if !ADT2PLAY
String_t FilterStr2 (const String *str, const uint8_t *charset, char chr);
#endif // !ADT2PLAY
String_t Num2str (uint32_t num, uint8_t base);
int32_t Str2num (const String *str, uint8_t base);

void init_StringIO (void);

#endif // !defined(STRINGIO_H)
