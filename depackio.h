// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef DEPACKIO_H
#define DEPACKIO_H

#include <stdint.h>

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

// Compression algorithm: RDC
// Algorithm developed by Ed Ross
uint16_t RDC_decompress (const void *source, void *dest, uint16_t size);

// Compression algorithm: LZSS
// Algorithm developed by Lempel-Ziv-Storer-Szymanski
uint16_t LZSS_decompress (const void *source, void *dest, uint16_t size);

// Compression algorithm: LZW
// Algorithm developed by Lempel-Ziv-Welch
uint16_t LZW_decompress (const void *source, void *dest);

// Compression algorithm: SixPack
// Algorithm developed by Philip G. Gage
uint16_t SIXPACK_decompress (const void *source, void *dest, uint16_t size);

// Compression algorithm: aPack
// Algorithm developed by Joergen Ibsen
uint32_t APACK_decompress (const void *source, void *dest);

#pragma pack(pop)

#endif // !defined(DEPACKIO_H)
