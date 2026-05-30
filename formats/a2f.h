// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  AdLib Tracker II instrument with FM-register macro file format
  Supported file format versions: 1..2
  Filename extension: .a2f
  All values are little-endian.

  Version | Compression
  1       | Jibz's aPLib
  2       | own implementation of LZH

  File structure 1:
    `header_a2f_t'
    `header_a2f_v1_t'
    block 0: (packed)
      `ins_data_a2f_v1_t'
      instrument name: 1-byte length + data (variable size)
                       (at most INSTRUMENT_NAME_MAX_A2F_V1 bytes)
      `ins_macro_a2f_v1_t'
      `dis_fmreg_col_a2f_v1_t'

  File structure 2:
    `header_a2f_t'
    `header_a2f_v1_t'
    block 0: (packed)
      block 0 version 1: single instrument or 1st of 4-op pair
      block 0 version 1: optional 2nd instrument of 4-op pair

*****************************************************************************/

#pragma once

#ifndef FORMATS_A2F_H
#define FORMATS_A2F_H 1

#define INSTRUMENT_NAME_MAX_A2F_V1 (1+INSTRUMENT_NAME_LEN_A2M_V9) // file format version 1..2

#define INSTRUMENT_MACRO_ROWS_MAX_A2F_V1 INSTRUMENT_MACRO_ROWS_MAX_A2M_V9 // file format version 1..2

#define ID_SIZE_A2F 18

static const char GCC_ATTRIBUTE((nonstring)) id_a2f[ID_SIZE_A2F] = { "_a2ins_w/fm-macro_" };

#pragma pack(push, 1)

typedef struct {  // file header
  char id[ID_SIZE_A2F]; // ID string
  uint32_t crc32;       // 32-bit cyclic redundancy check value
  uint8_t version;      // file format version
} header_a2f_t; // common for all file format versions

#define check_crc32_a2f check_crc32_a2m
//unpack_a2f()
//pack_a2f()

/*** File format version 1..2 ***/

typedef struct {  // data header
  uint16_t block_size;  // data block size
} header_a2f_v1_t;  // file format version 1..2

typedef ins_fm_data_a2m_v1_t  // instrument FM-register data
        ins_fm_data_a2f_v1_t; // file format version 1..2

typedef ins_data_a2m_v9_t   // instrument data
        ins_data_a2f_v1_t;  // file format version 1..2

typedef ins_macro_row_a2m_v9_t  // instrument macro row
        ins_macro_row_a2f_v1_t; // file format version 1..2
  // `.fm.Feedback_FM': bit 7=retrigger flag

typedef ins_macro_a2m_v9_t  // instrument macro
        ins_macro_a2f_v1_t; // file format version 1..2
  // `.arpeggio_table' always 0
  // `.vibrato_table' always 0

typedef dis_fmreg_col_a2m_v11_t // disabled FM-register columns
        dis_fmreg_col_a2f_v1_t; // file format version 1..2

#define get_ins_fm_data_a2f_v1  get_ins_fm_data_a2m_v1
#define put_ins_fm_data_a2f_v1  put_ins_fm_data_a2m_v1
#define get_ins_data_a2f_v1     get_ins_data_a2m_v9
#define put_ins_data_a2f_v1     put_ins_data_a2m_v9
//get_ins_macro_a2f_v1()
//put_ins_macro_a2f_v1()
#define get_dis_fmreg_col_a2f_v1  get_dis_fmreg_col_a2m_v11
#define put_dis_fmreg_col_a2f_v1  put_dis_fmreg_col_a2m_v11
//save_block_0_a2f_v2()

//load_instrument_a2f()
//save_instrument_a2f()

#pragma pack(pop)

#endif  // !defined(FORMATS_A2F_H)
