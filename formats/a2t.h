// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  AdLib Tracker II tiny module file format
  Supported file format versions: 1..14
  Filename extension: .a2t
  All values are little-endian.

  Version | Compression
  1,5     | SixPack
  2,6     | LZW
  3,7     | LZSS
  4,8     | no compression
  9..11   | Jibz's aPLib
  12..14  | own implementation of LZH

  File structure 1..4:
    `header_a2t_t'
    `header_a2t_v1_t'
    block 0: `ins_data_a2t_v1_t[0..INSTRUMENTS_MAX_A2T_V1]' with notice (may be packed)
    block 1: `pat_order_a2t_v1_t' (may be packed)
    block 2..5: `pat_data_a2t_v1_t[16]' (may be packed, last block may be smaller)

  File structure 5..8:
    `header_a2t_t'
    `header_a2t_v5_t'
    block 0: block 0 version 1..4 with notice
    block 1: block 1 version 1..4
    block 2..9: `pat_data_a2t_v5_t[8]' (may be packed, last block may be smaller)

  File structure 9:
    `header_a2t_t'
    `header_a2t_v9_t'
    block 0: `ins_data_a2t_v9_t[0..INSTRUMENTS_MAX_A2T_V9]' (packed)
    block 1: `ins_macro_a2t_v9_t[0..INSTRUMENTS_MAX_A2T_V9]' (packed)
    block 2: `macro_a2t_v9_t[0..INSTRUMENTS_MAX_A2T_V9]' (packed)
    block 3: block 1 version 1..4
    block 4..19: `pat_data_a2t_v9_t[8]' (packed, last block may be smaller)

  File structure 10:
    `header_a2t_t'
    `header_a2t_v10_t'
    block 0: block 0 version 9
    block 1: block 1 version 9
    block 2: block 2 version 9
    block 3: block 1 version 1..4
    block 4..19: block 4..19 version 9

  File structure 11:
    `header_a2t_t'
    `header_a2t_v11_t'
    block 0: block 0 version 9
    block 1: block 1 version 9
    block 2: block 2 version 9
    block 3: `dis_fmreg_col_a2t_v11_t[0..INSTRUMENTS_MAX_A2T_V9]' (packed)
    block 4: block 1 version 1..4
    block 5..20: block 4..19 version 9

  File structure 12..13:
    `header_a2t_t'
    `header_a2t_v11_t'
    block 0: (packed)
      * `block_0_a2t_v12_t'
      * block 0 version 9
    block 1: block 1 version 9
    block 2: block 2 version 9
    block 3: block 3 version 11
    block 4: block 1 version 1..4
    block 5..20: block 4..19 version 9

  File structure 14:
    `header_a2t_t'
    `header_a2t_v11_t'
    block 0: (packed)
      * `block_0_a2t_v14_t'
      * block 0 version 9
    block 1: block 1 version 9
    block 2: block 2 version 9
    block 3: block 3 version 11
    block 4: block 1 version 1..4
    block 5..20: block 4..19 version 9

*****************************************************************************/

#pragma once

#ifndef FORMATS_A2T_H
#define FORMATS_A2T_H 1

#define INSTRUMENTS_MAX_A2T_V1            INSTRUMENTS_MAX_A2M_V1            // file format version 1..8   (as `a2m')
#define INSTRUMENTS_MAX_A2T_V9            INSTRUMENTS_MAX_A2M_V9            // file format version 9..14  (as `a2m')
#define INSTRUMENTS_4OP_MAX_A2T_V12       INSTRUMENTS_4OP_MAX_A2M_V12       // file format version 12..14 (as `a2m')
#define INSTRUMENT_MACRO_ROWS_MAX_A2T_V9  INSTRUMENT_MACRO_ROWS_MAX_A2M_V9  // file format version 9..14  (as `a2m')
#define ARPEGGIO_MACRO_ROWS_MAX_A2T_V9    ARPEGGIO_MACRO_ROWS_MAX_A2M_V9    // file format version 9..14  (as `a2m')
#define VIBRATO_MACRO_ROWS_MAX_A2T_V9     VIBRATO_MACRO_ROWS_MAX_A2M_V9     // file format version 9..14  (as `a2m')
#define CHANNELS_MAX_A2T_V1               CHANNELS_MAX_A2M_V1               // file format version 1..4   (as `a2m')
#define CHANNELS_MAX_A2T_V5               CHANNELS_MAX_A2M_V5               // file format version 5..8   (as `a2m')
#define CHANNELS_MAX_A2T_V9               CHANNELS_MAX_A2M_V9               // file format version 9..14  (as `a2m')
#define PATTERN_LEN_A2T_V1                PATTERN_LEN_A2M_V1                // file format version 1..8   (as `a2m')
#define PATTERN_LEN_A2T_V9                PATTERN_LEN_A2M_V9                // file format version 9..14  (as `a2m')
#define PATTERNS_MAX_A2T_V1               PATTERNS_MAX_A2M_V1               // file format version 1..8   (as `a2m')
#define PATTERNS_MAX_A2T_V9               PATTERNS_MAX_A2M_V9               // file format version 9..14  (as `a2m')
#define PATTERN_ORDER_LEN_A2T_V1          PATTERN_ORDER_LEN_A2M_V1          // file format version 1..14  (as `a2m')

#define ID_SIZE_A2T 15

static const char GCC_ATTRIBUTE((nonstring)) id_a2t[ID_SIZE_A2T] = { "_A2tiny_module_" };

#pragma pack(push, 1)

typedef struct {  // file header
  char id[ID_SIZE_A2T]; // ID string
  uint32_t crc32;       // 32-bit cyclic redundancy check value
  uint8_t version;      // file format version
  uint8_t num_patterns; // as in `header_a2m_t'
  uint8_t tempo;        // as in `block_0_a2m_v1_t'
  uint8_t speed;        // as in `block_0_a2m_v1_t'
} header_a2t_t; // common for all file format versions

#define check_crc32_a2t check_crc32_a2m
#define is_packed_a2t   is_packed_a2m
#define unpack_a2t      unpack_a2m
#define pack_a2t        pack_a2m
#define put_string_a2t  put_string_a2m

/*** File format version 1..4 ***/

typedef struct {  // data header
  uint16_t block_sizes[6];  // data block sizes
} header_a2t_v1_t;  // file format version 1..4

typedef ins_fm_data_a2m_v1_t  // instrument FM-register data
        ins_fm_data_a2t_v1_t; // file format version 1..14 (as `a2m')
  // see notice for `Feedback_FM' field (as `a2m')

#define get_ins_fm_data_a2t_v1  get_ins_fm_data_a2m_v1

typedef ins_data_a2m_v1_t   // instrument data
        ins_data_a2t_v1_t;  // file format version 1..8 (as `a2m')
  // see notice for `panning' field (as `a2m')

#define get_ins_data_a2t_v1 get_ins_data_a2m_v1
//load_ins_data_a2t_v1()

typedef pat_order_a2m_v1_t  // pattern order
        pat_order_a2t_v1_t; // file format version 1..14 (as `a2m')

#define get_pat_order_a2t_v1  get_pat_order_a2m_v1
#define put_pat_order_a2t_v1  put_pat_order_a2m_v1
//load_pat_order_a2t_v1()

typedef pat_event_a2m_v1_t  // pattern event
        pat_event_a2t_v1_t; // file format version 1..8 (as `a2m')

#define get_pat_event_a2t_v1  get_pat_event_a2m_v1

typedef pat_data_a2m_v1_t   // pattern data
        pat_data_a2t_v1_t;  // file format version 1..4 (as `a2m')

#define get_pat_data_a2t_v1     get_pat_data_a2m_v1
#define replace_old_adsr_a2t_v1 replace_old_adsr_a2m_v1
#define load_patterns_a2t_v1    load_patterns_a2m_v1

/*** File format version 5..8 ***/

typedef struct {  // data header
  uint8_t flags;            // as in `block_0_a2m_v5_t'
  uint16_t block_sizes[10]; // data block sizes
} header_a2t_v5_t;  // file format version 5..8

#define get_flags_a2t_v5    get_flags_a2m_v5
#define get_ins_data_a2t_v5 get_ins_data_a2m_v5
//load_ins_data_a2t_v5()

typedef pat_data_a2m_v5_t   // pattern data
        pat_data_a2t_v5_t;  // file format version 5..8 (as `a2m')

#define get_pat_data_a2t_v5   get_pat_data_a2m_v5
#define load_patterns_a2t_v5  load_patterns_a2m_v5

/*** File format version 9 ***/

typedef struct {  // data header
  uint8_t flags;            // as in `block_0_a2m_v9_t'
  uint16_t pat_length;      // as in `block_0_a2m_v9_t'
  uint8_t num_channels;     // as in `block_0_a2m_v9_t'
  uint16_t macro_speedup;   // as in `block_0_a2m_v9_t'
  uint32_t block_sizes[20]; // data block sizes
} header_a2t_v9_t;  // file format version 9

#define get_flags_a2t_v9  get_flags_a2m_v9
#define put_flags_a2t_v9  put_flags_a2m_v9

typedef ins_data_a2m_v9_t   // instrument data
        ins_data_a2t_v9_t;  // file format version 9..14 (as `a2m')

#define get_ins_data_a2t_v9 get_ins_data_a2m_v9
#define put_ins_data_a2t_v9 put_ins_data_a2m_v9

typedef ins_macro_row_a2m_v9_t  // instrument macro row
        ins_macro_row_a2t_v9_t; // file format version 9..14 (as `a2m')
  // see notice for `fm_data' field (as `a2m')

typedef ins_macro_a2m_v9_t  // instrument macro
        ins_macro_a2t_v9_t; // file format version 9..14 (as `a2m')
  // see notice for `data' field (as `a2m')

#define get_ins_macro_a2t_v9  get_ins_macro_a2m_v9
#define put_ins_macro_a2t_v9  put_ins_macro_a2m_v9

typedef arp_macro_a2m_v9_t  // arpeggio macro
        arp_macro_a2t_v9_t; // file format version 9..14 (as `a2m')

#define get_arp_macro_a2t_v9  get_arp_macro_a2m_v9

typedef vib_macro_a2m_v9_t  // vibrato macro
        vib_macro_a2t_v9_t; // file format version 9..14 (as `a2m')

#define get_vib_macro_a2t_v9  get_vib_macro_a2m_v9

typedef macro_a2m_v9_t  // arpeggio and vibrato macro
        macro_a2t_v9_t; // file format version 9..14 (as `a2m')

#define get_macro_a2t_v9  get_macro_a2m_v9
#define put_macro_a2t_v9  put_macro_a2m_v9

typedef pat_event_a2m_v9_t  // pattern event
        pat_event_a2t_v9_t; // file format version 9..14 (as `a2m')

#define get_pat_event_a2t_v9  get_pat_event_a2m_v9

typedef pat_data_a2m_v9_t   // pattern data
        pat_data_a2t_v9_t;  // file format version 9..14 (as `a2m')

#define get_pat_data_a2t_v9   get_pat_data_a2m_v9
#define put_pat_data_a2t_v9   put_pat_data_a2m_v9
#define load_patterns_a2t_v9  load_patterns_a2m_v9
#define save_patterns_a2t_v9  save_patterns_a2m_v9

/*** File format version 10 ***/

#define get_chan_4op_flags_a2t_v10  get_chan_4op_flags_a2m_v10
#define put_chan_4op_flags_a2t_v10  put_chan_4op_flags_a2m_v10

typedef chan_flags_a2m_v10_t  // channel flags
        chan_flags_a2t_v10_t; // file format version 10..14 (as `a2m')

#define get_chan_flags_a2t_v10  get_chan_flags_a2m_v10

typedef struct {  // data header
  uint8_t flags;            // as in `block_0_a2m_v9_t'
  uint16_t pat_length;      // as in `block_0_a2m_v9_t'
  uint8_t num_channels;     // as in `block_0_a2m_v9_t'
  uint16_t macro_speedup;   // as in `block_0_a2m_v9_t'
  uint8_t chan_4op_flags;   // as in `block_0_a2m_v10_t'
  chan_flags_a2t_v10_t chan_flags; // as in `block_0_a2m_v10_t'
  uint32_t block_sizes[20]; // as in `header_a2t_v9_t'
} header_a2t_v10_t; // file format version 10

/*** File format version 11 ***/

typedef struct {  // data header
  uint8_t flags;            // as in `block_0_a2m_v9_t'
  uint16_t pat_length;      // as in `block_0_a2m_v9_t'
  uint8_t num_channels;     // as in `block_0_a2m_v9_t'
  uint16_t macro_speedup;   // as in `block_0_a2m_v9_t'
  uint8_t chan_4op_flags;   // as in `block_0_a2m_v10_t'
  chan_flags_a2t_v10_t chan_flags; // as in `block_0_a2m_v10_t'
  uint32_t block_sizes[21]; // data block sizes
} header_a2t_v11_t; // file format version 11..14

typedef dis_fmreg_col_a2m_v11_t   // disabled FM-register columns
        dis_fmreg_col_a2t_v11_t;  // file format version 11..14 (as `a2m')

#define get_dis_fmreg_col_a2t_v11 get_dis_fmreg_col_a2m_v11
#define put_dis_fmreg_col_a2t_v11 put_dis_fmreg_col_a2m_v11
//load_dis_fmreg_col_a2t_v11()

/*** File format version 12..13 ***/

#define get_chan_flags_a2t_v13  get_chan_flags_a2m_v13
#define put_chan_flags_a2t_v13  put_chan_flags_a2m_v13

typedef ins_4op_data_a2m_v12_t  // 4-op instruments
        ins_4op_data_a2t_v12_t; // file format version 12..14 (as `a2m')

#define get_ins_4op_data_a2t_v12  get_ins_4op_data_a2m_v12
#define put_ins_4op_data_a2t_v12  put_ins_4op_data_a2m_v12

typedef struct {  // data block 0
  ins_4op_data_a2t_v12_t ins_4op_data;  // as in `block_0_a2m_v12_t'
  uint8_t reserved[1024];               // as in `block_0_a2m_v12_t'
  //`ins_data_a2t_v9_t[0..INSTRUMENTS_MAX_A2T_V9]'  // as block 0 in file format version 9
} block_0_a2t_v12_t;  // file format version 12..13

//load_block_0_a2t_v12()

/*** File format version 14 ***/

typedef struct {  // data block 0
  uint8_t rows_per_beat;                // as in `block_0_a2m_v14_t'
  int16_t tempo_finetune;               // as in `block_0_a2m_v14_t'
  ins_4op_data_a2t_v12_t ins_4op_data;  // as in `block_0_a2m_v12_t'
  uint8_t reserved[1024];               // as in `block_0_a2m_v12_t'
  //`ins_data_a2t_v9_t[0..INSTRUMENTS_MAX_A2T_V9]'  // as block 0 in file format version 9
} block_0_a2t_v14_t;  // file format version 14

//load_block_0_a2t_v14()

#pragma pack(pop)

#endif  // !defined(FORMATS_A2T_H)
