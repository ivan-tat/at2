// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef STRINGIO_H
#define STRINGIO_H

#include "pascal.h"

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

typedef uint8_t tCHARSET[32];

char *UTF8str_to_AT2 (char *dest, const char *s);
char *UTF8nstr_to_AT2 (char *dest, size_t size, const char *s);

#if !ADT2PLAY
uint8_t CStrLen (const String *str);
uint8_t CStr2Len (const String *str);
uint8_t C3StrLen (const String *str);
#endif // !ADT2PLAY

void init_StringIO (void);

#endif // !defined(STRINGIO_H)
