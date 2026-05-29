// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  AdLib Tracker II instrument bank file format
  Supported file format versions: 1..10
  Filename extension: .a2b
  All values are little-endian.

  Version | Compression
  1,5     | SixPack
  2,6     | LZW
  3,7     | LZSS
  4,8     | no compression
  9       | Jibz's aPLib
  10      | own implementation of LZH

  File structure 1..8:
    `header_a2b_t'
    `header_a2b_v1_t'
    block 0: `block_0_a2b_v1_t' (may be packed) with notice in `ins_data_a2b_v1_t'

  File structure 9:
    `header_a2b_t'
    `header_a2b_v9_t'
    block 0: `block_0_a2b_v9_t' (packed)

  File structure 10:
    `header_a2b_t'
    `header_a2b_v9_t'
    block 0: `block_0_a2b_v10_t' (packed)

*****************************************************************************/

#pragma once

#ifndef FORMATS_A2B_H
#define FORMATS_A2B_H 1

#define INSTRUMENTS_MAX_A2B_V1  INSTRUMENTS_MAX_A2M_V1  // file format version 1..8
#define INSTRUMENTS_MAX_A2B_V9  INSTRUMENTS_MAX_A2M_V9  // file format version 9..10

#define ID_SIZE_A2B 11

static const char GCC_ATTRIBUTE((nonstring)) id_a2b[ID_SIZE_A2B] = { "_A2insbank_" };

#pragma pack(push, 1)

typedef struct {  // file header
  char id[ID_SIZE_A2B]; // ID string
  uint32_t crc32;       // 32-bit cyclic redundancy check value
  uint8_t version;      // file format version
} header_a2b_t; // common for all file format versions

#define put_string_a2b  put_string_a2m
//unpack_a2b()
//pack_a2b()

/*** File format version 1..8 ***/

typedef struct {  // data header
  uint16_t block_size;  // data block size
} header_a2b_v1_t;  // file format version 1..8

typedef ins_name_a2m_v1_t // instrument name
        ins_name_a2b_v1_t;  // file format version 1..8 (as `a2m')

#define get_ins_names_a2b_v1  get_ins_names_a2m_v1

typedef ins_fm_data_a2m_v1_t  // instrument FM-register data
        ins_fm_data_a2b_v1_t; // file format version 1..10 (as `a2m')

#define get_ins_fm_data_a2b_v1  get_ins_fm_data_a2m_v1

typedef ins_data_a2m_v1_t   // instrument data
        ins_data_a2b_v1_t;  // file format version 1..8 (as `a2m')
  // see notice for `panning' field (as `a2m')

#define get_ins_data_a2b_v1 get_ins_data_a2m_v1

typedef struct {  // instruments data block
  ins_name_a2b_v1_t ins_names[INSTRUMENTS_MAX_A2B_V1];  // as in `block_0_a2m_v1_t'
  ins_data_a2b_v1_t ins_data[INSTRUMENTS_MAX_A2B_V1];   // as in `block_0_a2m_v1_t'
} block_0_a2b_v1_t; // file format version 1..8

//load_block_0_a2b_v1()

/*** File format version 9 ***/

typedef struct {  // data header
  uint32_t block_size;  // data block size
} header_a2b_v9_t;  // file format version 9..10

typedef ins_name_a2m_v9_t // instrument name
        ins_name_a2b_v9_t;  // file format version 9..10 (as `a2m')

#define put_ins_names_a2b_v9  put_ins_names_a2m_v9

typedef ins_data_a2m_v9_t // instrument data
        ins_data_a2b_v9_t;  // file format version 9..10 (as `a2m')

#define put_ins_data_a2b_v9 put_ins_data_a2m_v9

typedef struct {  // instruments data block
  ins_name_a2b_v9_t ins_names[INSTRUMENTS_MAX_A2B_V9];  // as in `block_0_a2m_v9_t'
  ins_data_a2b_v9_t ins_data[INSTRUMENTS_MAX_A2B_V9];   // as in `block_0_a2m_v9_t'
} block_0_a2b_v9_t; // file format version 9

//load_block_0_a2b_v9()

/*** File format version 10 ***/

typedef ins_4op_data_a2m_v12_t  // 4-op instruments
        ins_4op_data_a2b_v10_t; // file format version 10

#define get_ins_4op_data_a2b_v10  get_ins_4op_data_a2m_v12
#define put_ins_4op_data_a2b_v10  put_ins_4op_data_a2m_v12

typedef struct {  // instruments data block
  ins_name_a2b_v9_t ins_names[INSTRUMENTS_MAX_A2B_V9];  // as in `block_0_a2m_v9_t'
  ins_data_a2b_v9_t ins_data[INSTRUMENTS_MAX_A2B_V9];   // as in `block_0_a2m_v9_t'
  ins_4op_data_a2b_v10_t ins_4op_data;                  // as in `block_0_a2m_v12_t'
} block_0_a2b_v10_t;  // file format version 10

//load_block_0_a2b_v10()
//save_block_0_a2b_v10()

#pragma pack(pop)

#endif  // !defined(FORMATS_A2B_H)
