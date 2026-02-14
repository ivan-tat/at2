// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef ADT2PACK_H
#define ADT2PACK_H

#include "defines.h"
#include <stdint.h>
#include "adt2unit.h"

#pragma pack(push, 1)
typedef struct
{
  bool ultra;     // ultra compression flag
  uint32_t size;  // original (unpacked) size
} LZH_block_info_t;
#pragma pack(pop)

// Compression algorithm: LZH
// Algorithm developed by Haruhiko Okomura & Haruyasu Yoshizaki
uint32_t LZH_compress (const void *source, void *dest, uint32_t size);
uint32_t LZH_compress_ultra (const void *source, void *dest, uint32_t size);
uint32_t LZH_decompress (const void *source, void *dest, uint32_t size, progress_callback_t *progress);

#endif // !defined(ADT2PACK_H)
