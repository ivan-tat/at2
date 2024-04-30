// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef PASCAL_STRING_H
#define PASCAL_STRING_H

#include "pascal.h"

void *memcpy (void *dest, const void *src, size_t n);
void *memmove (void *dest, const void *src, size_t n);

void *memset (void *s, int c, size_t n);

int memcmp (const void *s1, const void *s2, size_t n);

#define strlen Pascal_strlen

// Pascal strings support

// FPC_PCHAR_TO_SHORTSTR
// n=1..255
String *StrToString (String *dest, const char *src, uint8_t n);

// FPC_SHORTSTR_TO_SHORTSTR
// n=1..255
String *CopyString (String *dest, const String *src, uint8_t n);

// FPC_SHORTSTR_APPEND_SHORTSTR
// n=1..255
String *AppendString (String *dest, const String *src, uint8_t n);

#endif // !defined(PASCAL_STRING_H)
