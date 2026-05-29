// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  AdLib Tracker II instrument file format
  Supported file format versions: 1..10
  Filename extension: .a2i
  All values are little-endian.

  Version | Compression
  1,5     | SixPack
  2,6     | LZW
  3,7     | LZSS
  4,8     | no compression
  9       | Jibz's aPLib
  10      | own implementation of LZH

  File structure 1..8:
    `header_a2i_t'
    `header_a2i_v1_t'
    block 0: (may be packed)
      `ins_data_a2i_v1_t' with notice for `panning' field
      instrument name: 1-byte length + data (variable size)
                       (at most INSTRUMENT_NAME_MAX_A2I_V1 bytes)

  File structure 9:
    `header_a2i_t'
    `header_a2i_v9_t'
    block 0: (packed)
      `ins_data_a2i_v9_t'
      instrument name: 1-byte length + data (variable size)
                       (at most INSTRUMENT_NAME_MAX_A2I_V9 bytes)

  File structure 10:
    `header_a2i_t'
    `header_a2i_v9_t'
    block 0: (packed)
      block 0 version 9: single instrument or 1st of 4-op pair
      block 0 version 9: optional 2nd instrument of 4-op pair

*****************************************************************************/

#pragma once

#ifndef FORMATS_A2I_H
#define FORMATS_A2I_H 1

#define INSTRUMENT_NAME_MAX_A2I_V1 (1+22) // file format version 1..8
#define INSTRUMENT_NAME_MAX_A2I_V9 (1+32) // file format version 9..10

#define ID_SIZE_A2I 7

static const char GCC_ATTRIBUTE((nonstring)) id_a2i[ID_SIZE_A2I] = { "_A2ins_" };

#pragma pack(push, 1)

typedef struct {  // file header
  char id[ID_SIZE_A2I]; // ID string
  uint16_t crc16;       // 16-bit cyclic redundancy check value
  uint8_t version;      // file format version
} header_a2i_t; // common for all file format versions

//check_crc16_a2i()
//unpack_a2i()
//pack_a2i()

/*** File format version 1..8 ***/

typedef struct {  // data header
  uint8_t block_size; // data block size
} header_a2i_v1_t;  // file format version 1..8

typedef ins_fm_data_a2m_v1_t  // instrument FM-register data
        ins_fm_data_a2i_v1_t; // file format version 1..8

typedef ins_data_a2m_v1_t   // instrument data
        ins_data_a2i_v1_t;  // file format version 1..8

#define get_ins_fm_data_a2i_v1  get_ins_fm_data_a2m_v1
#define put_ins_fm_data_a2i_v1  put_ins_fm_data_a2m_v1
#define get_ins_data_a2i_v1     get_ins_data_a2m_v1
#define get_ins_data_a2i_v5     get_ins_data_a2m_v5
//load_block_0_a2i_v1()
//load_block_0_a2i_v5()

/*** File format version 9..10 ***/

typedef struct {  // data header
  uint16_t block_size;  // data block size
} header_a2i_v9_t;  // file format version 9..10

typedef ins_data_a2m_v9_t   // instrument data
        ins_data_a2i_v9_t;  // file format version 9..10

#define get_ins_data_a2i_v9 get_ins_data_a2m_v9
#define put_ins_data_a2i_v9 put_ins_data_a2m_v9
//load_block_0_a2i_v9()
//load_block_0_a2i_v10()
//save_block_0_a2i_v10()

//save_instrument_a2i()

#pragma pack(pop)

#endif  // !defined(FORMATS_A2I_H)
