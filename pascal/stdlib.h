// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024-2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef PASCAL_STDLIB_H
#define PASCAL_STDLIB_H

#include "pascal.h"

int custom_abs (int i);

#define ATEXIT_MAX 16

// Returns 0 on success, -1 on error.
int custom_atexit (void (*function) (void));

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

void *custom_malloc (size_t size);
void custom_free (void *ptr);
void *custom_realloc (void *ptr, size_t size);

#define RAND_MAX INT_MAX

int custom_rand (void);
void custom_srand (unsigned seed);

// Aliases
#define abs custom_abs
#define atexit custom_atexit
#define exit Pascal_Halt
#define malloc custom_malloc
#define free custom_free
#define realloc custom_realloc
#define rand custom_rand
#define srand custom_srand

#endif // !defined(PASCAL_STDLIB_H)
