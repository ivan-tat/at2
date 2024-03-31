// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef PASCAL_H
#define PASCAL_H

#include <stdint.h>
#include <sys/types.h>

// Pascal

typedef unsigned char Shortstring;

typedef Shortstring String;

#define Length(s) ((s)[0])
#define SetLength(s, l) (s)[0] = (l)
#define GetStr(s) (&(s)[1])

extern void Pascal_FillChar (void *x, ssize_t count, uint8_t value);
extern void Pascal_FillWord (void *x, ssize_t count, uint16_t value);

extern void Pascal_Move (const void *src, void *dest, ssize_t n);

// C

#include "pascal/string.h"

#endif // !defined(PASCAL_H)
