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

// Compression algorithm: aPack
// Algorithm developed by Joergen Ibsen

uint32_t APACK_decompress (const void *source, void *dest);

// Compression algorithm: LZH
// Algorithm developed by Haruhiko Okomura & Haruyasu Yoshizaki

#pragma pack(push, 1)
typedef struct
{
  bool ultra;     // ultra compression flag
  uint32_t size;  // original (unpacked) size
} LZH_block_info_t;
#pragma pack(pop)

#if !ADT2PLAY
uint32_t LZH_compress (const void *source, void *dest, uint32_t size);
uint32_t LZH_compress_ultra (const void *source, void *dest, uint32_t size);
#endif // !ADT2PLAY
uint32_t LZH_decompress (const void *source, void *dest, uint32_t size, progress_callback_t *progress);

// Compression algorithm: LZSS
// Algorithm developed by Lempel-Ziv-Storer-Szymanski

uint16_t LZSS_decompress (const void *source, void *dest, uint16_t size);

// Compression algorithm: LZW
// Algorithm developed by Lempel-Ziv-Welch

uint16_t LZW_decompress (const void *source, void *dest);

// Compression algorithm: RDC
// Algorithm developed by Ed Ross

uint16_t RDC_decompress (const void *source, void *dest, uint16_t size);

// Compression algorithm: SixPack
// Algorithm developed by Philip G. Gage

uint16_t SIXPACK_decompress (const void *source, void *dest, uint16_t size);

#endif // !defined(ADT2PACK_H)
