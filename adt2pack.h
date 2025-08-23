// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef ADT2PACK_H
#define ADT2PACK_H

#include "defines.h"
#include <stdint.h>

// Compression algorithm: LZH
// Algorithm developed by Haruhiko Okomura & Haruyasu Yoshizaki
uint32_t LZH_compress (const void *source, void *dest, uint32_t size);
uint32_t LZH_compress_ultra (const void *source, void *dest, uint32_t size);
uint32_t LZH_decompress (const void *source, void *dest, uint32_t size);

#endif // !defined(ADT2PACK_H)
