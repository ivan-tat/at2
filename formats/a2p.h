// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  AdLib Tracker II pattern file format
  Supported file format versions: 1..11
  Filename extension: .a2p
  All values are little-endian.

  Version | Compression
  1,5     | SixPack
  2,6     | LZW
  3,7     | LZSS
  4,8     | no compression
  9,10    | Jibz's aPLib
  11      | own implementation of LZH

  File structure 1..4:
    `header_a2p_t'
    `header_a2p_v1_t'
    block 0: `pat_data_a2p_v1_t' (may be packed)

  File structure 5..8:
    `header_a2p_t'
    `header_a2p_v1_t'
    block 0: `pat_data_a2p_v5_t' (may be packed)

  File structure 9:
    `header_a2p_t'
    `header_a2p_v9_t'
    block 0: `pat_data_a2p_v9_t' (packed)

  File structure 10..11:
    `header_a2p_t'
    `header_a2p_v9_t'
    block 0: `block_0_a2p_v10_t' (packed)

*****************************************************************************/

#pragma once

#ifndef FORMATS_A2P_H
#define FORMATS_A2P_H 1

#define CHANNELS_MAX_A2P_V1 CHANNELS_MAX_A2M_V1 // file format version 1..4  (as `a2m')
#define CHANNELS_MAX_A2P_V5 CHANNELS_MAX_A2M_V5 // file format version 5..8  (as `a2m')
#define CHANNELS_MAX_A2P_V9 CHANNELS_MAX_A2M_V9 // file format version 9..11 (as `a2m')
#define PATTERN_LEN_A2P_V1  PATTERN_LEN_A2M_V1  // file format version 1..8  (as `a2m')
#define PATTERN_LEN_A2P_V9  PATTERN_LEN_A2M_V9  // file format version 9..11 (as `a2m')

#define ID_SIZE_A2P 11

static const char GCC_ATTRIBUTE((nonstring)) id_a2p[ID_SIZE_A2P] = { "_A2pattern_" };

#pragma pack(push, 1)

typedef struct {  // file header
  char id[ID_SIZE_A2P]; // ID string
  uint32_t crc32;       // 32-bit cyclic redundancy check value
  uint8_t version;      // file format version
} header_a2p_t; // common for all file format versions

#define check_crc32_a2p check_crc32_a2m
//unpack_a2p()
//pack_a2p()
#define put_string_a2p  put_string_a2m

/*** File format version 1..4 ***/

typedef struct {  // data header
  uint16_t block_size;  // data block size
} header_a2p_v1_t;  // file format version 1..8

typedef pat_event_a2m_v1_t  // pattern event
        pat_event_a2p_v1_t; // file format version 1..8 (as `a2m')

#define get_pat_event_a2p_v1  get_pat_event_a2m_v1

typedef pat_data_a2m_v1_t   // pattern data
        pat_data_a2p_v1_t;  // file format version 1..4 (as `a2m')

#define get_pat_data_a2p_v1 get_pat_data_a2m_v1
//load_pattern_a2p_v1()

/*** File format version 5..8 ***/

typedef pat_data_a2m_v5_t   // pattern data
        pat_data_a2p_v5_t;  // file format version 5..8 (as `a2m')

#define get_pat_event_a2p_v5  get_pat_event_a2m_v5
#define get_pat_data_a2p_v5   get_pat_data_a2m_v5
//load_pattern_a2p_v5()

/*** File format version 9 ***/

typedef struct {  // data header
  uint32_t block_size;  // data block size
} header_a2p_v9_t;  // file format version 9..11

typedef pat_event_a2m_v9_t  // pattern event
        pat_event_a2p_v9_t; // file format version 9..11 (as `a2m')

#define get_pat_event_a2p_v9  get_pat_event_a2m_v9

typedef pat_data_a2m_v9_t   // pattern data
        pat_data_a2p_v9_t;  // file format version 9..11 (as `a2m')

#define get_pat_data_a2p_v9 get_pat_data_a2m_v9
#define put_pat_data_a2p_v9 put_pat_data_a2m_v9
//load_pattern_a2p_v9()

/*** File format version 10..11 ***/

typedef struct {  // pattern name
  uint8_t len;
  uint8_t data[30];
} pat_name_a2p_v10_t; // file format version 10..11

typedef struct {  // pattern data block
  pat_data_a2p_v9_t pat_data;
  pat_name_a2p_v10_t pat_name;
} block_0_a2p_v10_t;  // file format version 10..11

//load_pattern_a2p_v10()
//save_pattern_a2p_v10()

//load_pattern_a2p()
//save_pattern_a2p()

#pragma pack(pop)

#endif  // !defined(FORMATS_A2P_H)
