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

typedef CharSet_t tCHARSET;

// `AT2' upper case conversion table
extern uint8_t AT2_upper_case_ct[0x100];

// `AT2' lower case conversion table
extern uint8_t AT2_lower_case_ct[0x100];

extern CharSet_t DEC_NUM_CHARSET;
extern CharSet_t HEX_NUM_CHARSET;

__inline__ uint8_t UpCase (uint8_t c) {
  return AT2_upper_case_ct[c];
}

__inline__ uint8_t LoCase (uint8_t c) {
  return AT2_lower_case_ct[c];
}

char *UTF8str_to_AT2 (char *dest, const char *s);
char *UTF8nstr_to_AT2 (char *dest, size_t size, const char *s);

#if !ADT2PLAY
uint8_t CStrLen (const String *str);
uint8_t CStr2Len (const String *str);
uint8_t C3StrLen (const String *str);
#endif // !ADT2PLAY

String_t ExpStrR (const String *str, uint8_t size, char chr);
#if !ADT2PLAY
String_t CutStrR (const String *str, uint8_t margin);
String_t FilterStr2 (const String *str, const uint8_t *charset, char chr);
#endif // !ADT2PLAY
String_t Num2str (uint32_t num, uint8_t base);
int32_t Str2num (const String *str, uint8_t base);

void init_StringIO (void);

#endif // !defined(STRINGIO_H)
