// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
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

void custom_free (void *ptr);
void *custom_realloc (void *ptr, size_t size);

// Aliases
#define abs custom_abs
#define atexit custom_atexit
#define exit Pascal_Halt
#define malloc Pascal_AllocMem
#define free custom_free
#define realloc custom_realloc

#endif // !defined(PASCAL_STDLIB_H)
