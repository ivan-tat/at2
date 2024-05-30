// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef PARSERIO_H
#define PARSERIO_H

#include "defines.h"

#include <stdint.h>
#if USE_FPC
#include "pascal/stdio.h" // ssize_t
#else // !USE_FPC
#include <stdio.h> // ssize_t
#endif // !USE_FPC

int32_t Scan (const void *buf, ssize_t skip, ssize_t size, const String *str);
int32_t SensitiveScan (const void *buf, ssize_t skip, ssize_t size,
                       const String *str);
bool Compare (const char *buf1, const char *buf2, size_t size);
bool Empty (const char *buf, size_t size);
size_t CountLines (const char *buf, size_t size);
uint16_t Update16 (const uint8_t *buf, size_t size, uint16_t CRC);
uint32_t Update32 (const uint8_t *buf, size_t size, uint32_t CRC);
void make_table_16bit (void);
void make_table_32bit (void);

#endif // !defined(PARSERIO_H)
