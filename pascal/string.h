// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024-2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef PASCAL_STRING_H
#define PASCAL_STRING_H

#include "pascal.h"

void *custom_memchr (const void *s, char c, size_t n);
int custom_memcmp (const void *s1, const void *s2, size_t n);
void *custom_memcpy (void *dest, const void *src, size_t n);
void *custom_memmove (void *dest, const void *src, size_t n);
void *custom_memset (void *s, int c, size_t n);

size_t custom_strnlen (const char *s, size_t n);
char *custom_stpncpy (char *dest, const char *src, size_t n);

#define memchr custom_memchr
#define memcmp custom_memcmp
#define memcpy custom_memcpy
#define memmove custom_memmove
#define memset custom_memset

#define strlen Pascal_strlen
#define strnlen custom_strnlen
#define stpcpy Pascal_strecopy
#define stpncpy custom_stpncpy
#define strcpy Pascal_strcopy
#define strncpy Pascal_strlcopy
#define strcat Pascal_strcat
#define strncat Pascal_strlcat
#define strcmp Pascal_strcomp
#define strncmp Pascal_strlcomp
#define strchr Pascal_strscan
#define strrchr Pascal_strrscan
#define strdup Pascal_strnew
#define strstr Pascal_strpos
#define strcasestr Pascal_stripos

// Pascal strings support

// n=1..255
char *StringToStr (char *dest, const String *src, uint8_t n);

// FPC_PCHAR_TO_SHORTSTR
// n=1..255
String *StrToString (String *dest, const char *src, uint8_t n);

// FPC_SHORTSTR_TO_SHORTSTR
// n=1..255
String *CopyString (String *dest, const String *src, uint8_t n);

// FPC_SHORTSTR_APPEND_SHORTSTR
// n=1..255
String *AppendString (String *dest, const String *src, uint8_t n);

// FPC_SHORTSTR_COPY / Copy
// index: 1..255
String_t Copy (const String *s, ssize_t index, ssize_t count);

// Delete
// index: 1..255
void Delete (String *s, ssize_t index, ssize_t count);

// Insert
// index, n: 1..255
void Insert (const String *src, String *s, ssize_t n, ssize_t index);
void InsertChr (char c, String *s, ssize_t n, ssize_t index);

// Pos
// Returns index (1..255) if found, 0 if not.
ssize_t Pos (const String *substr, const String *s);
ssize_t PosChr (char c, const String *s);

// FPC_SHORTSTR_COMPARE
int CompareStrings (const String *s1, const String *s2);

#endif // !defined(PASCAL_STRING_H)
