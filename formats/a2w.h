// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  AdLib Tracker II instruments with FM-register macro bank file format
  Supported file format versions: 1..3
  Filename extension: .a2w
  All values are little-endian.

  Version | Compression
  1,2     | Jibz's aPLib
  3       | own implementation of LZH

  File structure 1:
    `header_a2w_t'
    `header_a2w_v1_t'
    block 0: `block_0_a2w_v1_t' (packed)
    block 1: `macro_a2w_v1_t[INSTRUMENTS_MAX_A2W_V1]' (packed)

  File structure 2:
    `header_a2w_t'
    `header_a2w_v2_t'
    block 0: block 0 version 1
    block 1: block 1 version 1
    block 2: `dis_fmreg_col_a2w_v2_t[INSTRUMENTS_MAX_A2W_V1]' (packed)

  File structure 3:
    `header_a2w_t'
    `header_a2w_v2_t'
    block 0: `block_0_a2w_v3_t' (packed)
    block 1: block 1 version 1
    block 2: block 2 version 2

*****************************************************************************/

#pragma once

#ifndef FORMATS_A2W_H
#define FORMATS_A2W_H 1

#define INSTRUMENTS_MAX_A2W_V1            INSTRUMENTS_MAX_A2M_V9            // file format version 1..3
#define INSTRUMENTS_4OP_MAX_A2W_V3        INSTRUMENTS_4OP_MAX_A2M_V12       // file format version 3
#define INSTRUMENT_MACRO_ROWS_MAX_A2W_V1  INSTRUMENT_MACRO_ROWS_MAX_A2M_V9  // file format version 1..3
#define ARPEGGIO_MACRO_ROWS_MAX_A2W_V1    ARPEGGIO_MACRO_ROWS_MAX_A2M_V9    // file format version 1..3
#define VIBRATO_MACRO_ROWS_MAX_A2W_V1     VIBRATO_MACRO_ROWS_MAX_A2M_V9     // file format version 1..3

#define ID_SIZE_A2W 20

static const char GCC_ATTRIBUTE((nonstring)) id_a2w[ID_SIZE_A2W] = { "_A2insbank_w/macros_" };

#pragma pack(push, 1)

typedef struct {  // file header
  char id[ID_SIZE_A2W]; // ID string
  uint32_t crc32;       // 32-bit cyclic redundancy check value
  uint8_t version;      // file format version
} header_a2w_t; // common for all file format versions

#define put_string_a2w  put_string_a2m
//pack_a2w()

/*** File format version 1 ***/

typedef struct {  // data header
  uint32_t block_sizes[2];  // data block sizes
} header_a2w_v1_t;  // file format version 1

typedef ins_name_a2m_v9_t // instrument name
        ins_name_a2w_v1_t;  // file format version 1..3 (as `a2m' file format version 9..14)

#define put_ins_names_a2w_v9  put_ins_names_a2m_v9

typedef ins_fm_data_a2m_v1_t  // instrument FM-register data
        ins_fm_data_a2w_v1_t; // file format version 1..3 (as `a2m' file format version 1..14)

#define put_ins_fm_data_a2w_v1  put_ins_fm_data_a2m_v1

typedef ins_data_a2m_v9_t // instrument data
        ins_data_a2w_v1_t;  // file format version 1..3 (as `a2m' file format version 9..14)

#define put_ins_data_a2w_v1 put_ins_data_a2m_v9

typedef ins_macro_row_a2m_v9_t  // instrument macro row
        ins_macro_row_a2w_v1_t; // file format version 1..3 (as `a2m' file format version 9..14)
  // file format version 2..3: as `a2m' file format version 11..14

typedef ins_macro_a2m_v9_t  // instrument macro
        ins_macro_a2w_v1_t; // file format version 1..3 (as `a2m' file format version 9..14)
  // file format version 2..3: as `a2m' file format version 11..14

#define put_ins_macro_a2w_v1 put_ins_macro_a2m_v9

typedef struct {  // instruments data block
  ins_name_a2w_v1_t ins_names[INSTRUMENTS_MAX_A2W_V1];    // as in `block_0_a2m_v9_t'
  ins_data_a2w_v1_t ins_data[INSTRUMENTS_MAX_A2W_V1];     // as in `block_0_a2m_v9_t'
  ins_macro_a2w_v1_t ins_macros[INSTRUMENTS_MAX_A2W_V1];  // as in `block_0_a2m_v9_t'
} block_0_a2w_v1_t; // file format version 1..2

typedef arp_macro_a2m_v9_t  // arpeggio macro
        arp_macro_a2w_v1_t; // file format version 1..3 (as `a2m' file format version 9..14)

#define put_arp_macro_a2w_v1  put_arp_macro_a2m_v9

typedef vib_macro_a2m_v9_t  // vibrato macro
        vib_macro_a2w_v1_t; // file format version 1..3 (as `a2m' file format version 9..14)

#define put_vib_macro_a2w_v1  put_vib_macro_a2m_v9

typedef macro_a2m_v9_t  // arpeggio and vibrato macro
        macro_a2w_v1_t; // file format version 1..3 (as `a2m' file format version 9..14)

#define put_macro_a2w_v1  put_macro_a2m_v9

//save_macros_a2w_v1()

/*** File format version 2 ***/

typedef struct {  // data block
  uint32_t block_sizes[3];  // data block sizes
} header_a2w_v2_t;  // file format version 2..3

typedef dis_fmreg_col_a2m_v11_t // disabled FM-register columns
        dis_fmreg_col_a2w_v2_t; // file format version 2..3 (as `a2m' file format version 11..14)

#define put_dis_fmreg_col_a2w_v2  put_dis_fmreg_col_a2m_v11

//save_dis_fmreg_col_a2w_v2()

/*** File format version 3 ***/

typedef ins_4op_data_a2m_v12_t  // 4-op instruments
        ins_4op_data_a2w_v3_t;  // file format version 3 (as `a2m' file format version 12..14)

#define put_ins_4op_data_a2w_v3 put_ins_4op_data_a2m_v12

typedef struct {  // instruments data block
  ins_name_a2w_v1_t ins_names[INSTRUMENTS_MAX_A2W_V1];    // as in `block_0_a2m_v9_t'
  ins_data_a2w_v1_t ins_data[INSTRUMENTS_MAX_A2W_V1];     // as in `block_0_a2m_v9_t'
  ins_macro_a2w_v1_t ins_macros[INSTRUMENTS_MAX_A2W_V1];  // as in `block_0_a2m_v9_t'
  ins_4op_data_a2w_v3_t ins_4op_data;                     // as in `block_0_a2m_v12_t'
} block_0_a2w_v3_t; // file format version 3

//save_block_0_a2w_v3()

//save_instrument_bank_a2w()

#pragma pack(pop)

#endif  // !defined(FORMATS_A2W_H)
