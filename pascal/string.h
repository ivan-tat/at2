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

#endif // !defined(STRING_H)
