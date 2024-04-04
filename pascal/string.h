// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef STRING_H
#define STRING_H

#include <stdint.h>

void *memcpy (void *dest, const void *src, size_t n);
void *memmove (void *dest, const void *src, size_t n);

void *memset (void *s, int c, size_t n);

size_t strlen (const char *s);

// Pascal strings support

// FPC_PCHAR_TO_SHORTSTR
// n=1..255
String *StrToString (String *dest, const char *src, uint8_t n);

// FPC_SHORTSTR_TO_SHORTSTR
// n=1..255
String *CopyString (String *dest, const String *src, uint8_t n);

#endif // !defined(STRING_H)
