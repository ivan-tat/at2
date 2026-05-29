// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  AdLib Tracker II module file format
  Supported file format versions: 1..14
  Filename extension: .a2m
  All values are little-endian.

  Version | Compression
  1,5     | SixPack
  2,6     | LZW
  3,7     | LZSS
  4,8     | no compression
  9..11   | Jibz's aPLib
  12..14  | own implementation of LZH

  File structure 1..4:
    `header_a2m_t'
    `header_a2m_v1_t'
    block 0:    `block_0_a2m_v1_t' (may be packed)
    block 1..4: `pat_data_a2m_v1_t[16]' (may be packed, last block may be smaller)


  File structure 5..8:
    `header_a2m_t'
    `header_a2m_v5_t'
    block 0:    `block_0_a2m_v5_t' (may be packed)
    block 1..8: `pat_data_a2m_v5_t[8]' (may be packed, last block may be smaller)

  File structure 9:
    `header_a2m_t'
    `header_a2m_v9_t'
    block 0:     `block_0_a2m_v9_t' (packed)
    block 1..16: `pat_data_a2m_v9_t[8]' (packed, last block may be smaller)

  File structure 10:
    `header_a2m_t'
    `header_a2m_v9_t'
    block 0:     `block_0_a2m_v10_t' (packed)
    block 1..16: block 1..16 version 9

  File structure 11:
    `header_a2m_t'
    `header_a2m_v9_t'
    block 0:     `block_0_a2m_v11_t' (packed)
    block 1..16: block 1..16 version 9

  File structure 12..13:
    `header_a2m_t'
    `header_a2m_v9_t'
    block 0:     `block_0_a2m_v12_t' (packed)
    block 1..16: block 1..16 version 9

  File structure 14:
    `header_a2m_t'
    `header_a2m_v9_t'
    block 0:     `block_0_a2m_v14_t' (packed)
    block 1..16: block 1..16 version 9

*****************************************************************************/

#pragma once

#ifndef FORMATS_A2M_H
#define FORMATS_A2M_H 1

#define SONG_NAME_LEN_A2M_V1 42                         // file format version 1..14
#define COMPOSER_LEN_A2M_V1 SONG_NAME_LEN_A2M_V1        // file format version 1..14
#define INSTRUMENT_NAME_LEN_A2M_V1 32                   // file format version 1..8
#define INSTRUMENT_NAME_LEN_A2M_V9 SONG_NAME_LEN_A2M_V1 // file format version 9..14

#define INSTRUMENTS_MAX_A2M_V1 250            // file format version 1..8
#define INSTRUMENTS_MAX_A2M_V9 255            // file format version 9..14
#define INSTRUMENTS_4OP_MAX_A2M_V12 128       // file format version 12..14
#define INSTRUMENT_MACRO_ROWS_MAX_A2M_V9 255  // file format version 9..14
#define ARPEGGIO_MACRO_ROWS_MAX_A2M_V9 255    // file format version 9..14
#define VIBRATO_MACRO_ROWS_MAX_A2M_V9 255     // file format version 9..14
#define CHANNELS_MAX_A2M_V1 9                 // file format version 1..4
#define CHANNELS_MAX_A2M_V5 18                // file format version 5..8
#define CHANNELS_MAX_A2M_V9 20                // file format version 9..14
#define PATTERN_LEN_A2M_V1 64                 // file format version 1..8
#define PATTERN_LEN_A2M_V9 256                // file format version 9..14
#define PATTERNS_MAX_A2M_V1 64                // file format version 1..8
#define PATTERNS_MAX_A2M_V9 128               // file format version 9..14
#define PATTERN_ORDER_LEN_A2M_V1 128          // file format version 1..14

#define ID_SIZE_A2M 10

static const char GCC_ATTRIBUTE((nonstring)) id_a2m[ID_SIZE_A2M] = { "_A2module_" };

#pragma pack(push, 1)

typedef struct {  // file header
  char id[ID_SIZE_A2M]; // ID string
  uint32_t crc32;       // 32-bit cyclic redundancy check value
  uint8_t version;      // file format version
  uint8_t num_patterns; // number of patterns
} header_a2m_t; // common for all file format versions

//check_crc32_a2m()
//is_packed_a2m()
//unpack_a2m()
//pack_a2m()
//put_string_a2m()

/*** File format version 1..4 ***/

typedef struct {  // data header
  uint16_t block_sizes[5];  // data block sizes
} header_a2m_v1_t;  // file format version 1..4

typedef struct {  // song name
  uint8_t length;
  char data[SONG_NAME_LEN_A2M_V1];
} song_name_a2m_v1_t; // file format version 1..14

//get_song_name_a2m_v1()
//put_song_name_a2m_v1()

typedef song_name_a2m_v1_t  // composer
        composer_a2m_v1_t;  // file format version 1..14

//get_composer_a2m_v1()
//put_composer_a2m_v1()

typedef struct {  // instrument name
  uint8_t length;
  char data[INSTRUMENT_NAME_LEN_A2M_V1];
} ins_name_a2m_v1_t;  // file format version 1..8

//get_ins_name_a2m_v1()

typedef struct {  // instrument FM-register data ([0]=modulator, [1]=carrier)
  uint8_t AM_Vibrato_EG[2];
  uint8_t KSL_Volume[2];
  uint8_t Attack_Decay[2];
  uint8_t Sustain_Release[2];
  uint8_t WaveForm[2];
  uint8_t Feedback_FM;  // file format version 11..14: in `ins_macro_row_a2m_v9_t':
                        // - bit 7=retrigger flag (bool)
} ins_fm_data_a2m_v1_t; // file format version 1..14

//get_ins_fm_data_a2m_v1()
//put_ins_fm_data_a2m_v1()

typedef struct {  // instrument data
  ins_fm_data_a2m_v1_t fm_data;
  uint8_t panning;  // file format version 1..4: miscellaneous (unused)
                    // file format version 5..8: 0=center, 1=left, 2=right
  int8_t fine_tune;
} ins_data_a2m_v1_t;  // file format version 1..8

//get_ins_data_a2m_v1()
//put_ins_data_a2m_v1()
//load_ins_data_a2m_v1()

typedef uint8_t // pattern order
        pat_order_a2m_v1_t[PATTERN_ORDER_LEN_A2M_V1]; // file format version 1..14

//get_pat_order_a2m_v1()
//put_pat_order_a2m_v1()
//load_pat_order_a2m_v1()

typedef struct {  // song data block
  song_name_a2m_v1_t song_name;
  composer_a2m_v1_t composer;
  ins_name_a2m_v1_t ins_names[INSTRUMENTS_MAX_A2M_V1];
  ins_data_a2m_v1_t ins_data[INSTRUMENTS_MAX_A2M_V1];
  pat_order_a2m_v1_t pat_order;
  uint8_t tempo;  // initial tempo
  uint8_t speed;  // initial speed
} block_0_a2m_v1_t; // file format version 1..4

//load_block_0_a2m_v1()
//get_ins_names_a2m_v1()

typedef struct {  // pattern event
  uint8_t note; // 0..96=note, 255=key-off
  uint8_t ins;  // 0..INSTRUMENTS_MAX_A2M_V1=instrument
  uint8_t cmd;  // file format version 1..4: 0-15=effect command
                // file format version 5..8: 0-35=effect command
  uint8_t data; // effect data
} pat_event_a2m_v1_t; // file format version 1..8

//get_pat_event_a2m_v1()

typedef pat_event_a2m_v1_t  // pattern data
        pat_data_a2m_v1_t[PATTERN_LEN_A2M_V1][CHANNELS_MAX_A2M_V1]; // file format version 1..4

//get_pat_data_a2m_v1()
//replace_old_adsr_a2m_v1()
//load_patterns_a2m_v1()

/*** File format version 5..8 ***/

typedef struct {  // data header
  uint16_t block_sizes[9];  // data block sizes
} header_a2m_v5_t;  // file format version 5..8

//get_ins_data_a2m_v5()

typedef struct {  // song data block
  song_name_a2m_v1_t song_name; // as in `block_0_a2m_v1_t'
  composer_a2m_v1_t composer;   // as in `block_0_a2m_v1_t'
  ins_name_a2m_v1_t ins_names[INSTRUMENTS_MAX_A2M_V1];  // as in `block_0_a2m_v1_t'
  ins_data_a2m_v1_t ins_data[INSTRUMENTS_MAX_A2M_V1];   // as in `block_0_a2m_v1_t'
  pat_order_a2m_v1_t pat_order; // as in `block_0_a2m_v1_t'
  uint8_t tempo;  // as in `block_0_a2m_v1_t'
  uint8_t speed;  // as in `block_0_a2m_v1_t'
  uint8_t flags;  // - bit 0   =update speed (bool)
                  // - bit 1   =channel volume lock (bool)
                  // - bit 2   =volume peak lock (bool)
                  // - bit 3   =tremolo depth (0..1)
                  // - bit 4   =vibrato depth (0..1)
                  // - bit 5   =channel panning lock (bool)
                  // - bit 7..6=unused
} block_0_a2m_v5_t; // file format version 5..8

//get_flags_a2m_v5()
//load_block_0_a2m_v5()

//get_pat_event_a2m_v5()

typedef pat_event_a2m_v1_t  // pattern data
        pat_data_a2m_v5_t[CHANNELS_MAX_A2M_V5][PATTERN_LEN_A2M_V1]; // file format version 5..8

//get_pat_data_a2m_v5()
//load_patterns_a2m_v5()

/*** File format version 9 ***/

typedef struct {  // data header
  uint32_t block_sizes[17]; // data block sizes
} header_a2m_v9_t;  // file format version 9..14

typedef song_name_a2m_v1_t  // instrument name
        ins_name_a2m_v9_t;  // file format version 9..14

//get_ins_name_a2m_v9()
//put_ins_names_a2m_v9()

typedef struct {  // instrument data
  ins_fm_data_a2m_v1_t fm_data;
  uint8_t panning;    // 0=center, 1=left, 2=right
  int8_t fine_tune;
  uint8_t voice_type; // 0=melodic, 1..5=percussion (1=BD, 2=SD, 3=TT, 4=TC, 5=HH)
} ins_data_a2m_v9_t;  // file format version 9..14

//get_ins_data_a2m_v9()
//put_ins_data_a2m_v9()

typedef struct {  // instrument macro row
  ins_fm_data_a2m_v1_t fm_data; // see notice for `Feedback_FM' field
  int16_t frequency_slide;
  uint8_t panning;              // 0=center, 1=left, 2=right
  uint8_t duration;
} ins_macro_row_a2m_v9_t; // file format version 9..14

typedef struct {  // instrument macro
  uint8_t length;
  uint8_t loop_begin;     // loop begin position
  uint8_t loop_length;
  uint8_t keyoff_pos;     // key-off position
  uint8_t arpeggio_table; // arpeggio macro-table number
  uint8_t vibrato_table;  // vibrato macro-table number
  ins_macro_row_a2m_v9_t data[INSTRUMENT_MACRO_ROWS_MAX_A2M_V9];  // see notice for `fm_data' field
} ins_macro_a2m_v9_t; // file format version 9..14

//get_ins_macro_a2m_v9()
//put_ins_macro_a2m_v9()

typedef struct {  // arpeggio macro
  uint8_t length;
  uint8_t speed;
  uint8_t loop_begin;   // loop begin position
  uint8_t loop_length;
  uint8_t keyoff_pos;   // key-off position
  uint8_t data[ARPEGGIO_MACRO_ROWS_MAX_A2M_V9]; // 0=default note
                        // 1..96=number of half-tones to add
                        // 0x80+1..0x80+97=fixed-note 1..97
} arp_macro_a2m_v9_t; // file format version 9..14

//get_arp_macro_a2m_v9()
//put_arp_macro_a2m_v9()

typedef struct {  // vibrato macro
  uint8_t length;
  uint8_t speed;
  uint8_t delay;
  uint8_t loop_begin;   // loop begin position
  uint8_t loop_length;
  uint8_t keyoff_pos;   // key-off position
  int8_t data[VIBRATO_MACRO_ROWS_MAX_A2M_V9]; // 0=default frequency
                        // -127..+127=frequency units to add
} vib_macro_a2m_v9_t; // file format version 9..14

//get_vib_macro_a2m_v9()
//put_vib_macro_a2m_v9()

typedef struct {  // arpeggio and vibrato macro
  arp_macro_a2m_v9_t arpeggio;
  vib_macro_a2m_v9_t vibrato;
} macro_a2m_v9_t; // file format version 9..14

//get_macro_a2m_v9()
//put_macro_a2m_v9()

typedef struct {  // song data block
  song_name_a2m_v1_t song_name; // as in `block_0_a2m_v1_t'
  composer_a2m_v1_t composer;   // as in `block_0_a2m_v1_t'
  ins_name_a2m_v9_t ins_names[INSTRUMENTS_MAX_A2M_V9];
  ins_data_a2m_v9_t ins_data[INSTRUMENTS_MAX_A2M_V9];
  ins_macro_a2m_v9_t ins_macros[INSTRUMENTS_MAX_A2M_V9];
  macro_a2m_v9_t macros[INSTRUMENTS_MAX_A2M_V9];
  pat_order_a2m_v1_t pat_order; // as in `block_0_a2m_v1_t'
  uint8_t tempo;          // as in `block_0_a2m_v1_t'
  uint8_t speed;          // as in `block_0_a2m_v1_t'
  uint8_t flags;          // - bit 5..0=as in `block_0_a2m_v5_t'
                          // - bit 6   =percussion channel extension (bool)
                          // - bit 7   =volume scaling (bool)
  uint16_t pat_length;    // pattern length
  uint8_t num_channels;   // number of channels
  uint16_t macro_speedup; // macro speed-up factor
} block_0_a2m_v9_t; // file format version 9

//get_flags_a2m_v9()
//put_flags_a2m_v9()
//get_ins_names_a2m_v9()
//put_ins_names_a2m_v9()
//load_block_0_a2m_v9()

typedef struct {  // pattern event
  uint8_t note;   // 0..96=note, 0x90+0..0x90+96=fixed note, 255=key-off
  uint8_t ins;    // 0..INSTRUMENTS_MAX_A2M_V9=instrument
  uint8_t cmd1;   // effect 1 command
  uint8_t data1;  // effect 1 data
  uint8_t cmd2;   // effect 2 command
  uint8_t data2;  // effect 2 data
} pat_event_a2m_v9_t; // file format version 9..14

//get_pat_event_a2m_v9()

typedef pat_event_a2m_v9_t  // pattern data
        pat_data_a2m_v9_t[CHANNELS_MAX_A2M_V9][PATTERN_LEN_A2M_V9]; // file format version 9..14

//get_pat_data_a2m_v9()
//put_pat_data_a2m_v9()
//load_patterns_a2m_v9()
//save_patterns_a2m_v9()

/*** File format version 10 ***/

//get_chan_4op_flags_a2m_v10()
//put_chan_4op_flags_a2m_v10()

typedef uint8_t // channel flags
                // - bit 1..0=panning position (int): 0=center, 1=left, 2=right
                // - bit 3..2=volume slide type (int): 0=def, 1=car, 2=mod, 3=car w/mod
                // - bit 4   =volume lock state (bool)
                // - bit 5   =peak lock state (bool)
                // - bit 6   =file format version 10..12: unused
                //            file format version 13..14: 4-op channel volume lock (bool)
                // - bit 7   =unused
        chan_flags_a2m_v10_t[CHANNELS_MAX_A2M_V9];  // file format version 10..14

//get_chan_flags_a2m_v10()

typedef struct {  // song data block
  song_name_a2m_v1_t song_name; // as in `block_0_a2m_v1_t'
  composer_a2m_v1_t composer;   // as in `block_0_a2m_v1_t'
  ins_name_a2m_v9_t ins_names[INSTRUMENTS_MAX_A2M_V9];    // as in `block_0_a2m_v9_t'
  ins_data_a2m_v9_t ins_data[INSTRUMENTS_MAX_A2M_V9];     // as in `block_0_a2m_v9_t'
  ins_macro_a2m_v9_t ins_macros[INSTRUMENTS_MAX_A2M_V9];  // as in `block_0_a2m_v9_t'
  macro_a2m_v9_t macros[INSTRUMENTS_MAX_A2M_V9];          // as in `block_0_a2m_v9_t'
  pat_order_a2m_v1_t pat_order; // as in `block_0_a2m_v1_t'
  uint8_t tempo;          // as in `block_0_a2m_v9_t'
  uint8_t speed;          // as in `block_0_a2m_v9_t'
  uint8_t flags;          // as in `block_0_a2m_v9_t'
  uint16_t pat_length;    // as in `block_0_a2m_v9_t"
  uint8_t num_channels;   // as in `block_0_a2m_v9_t'
  uint16_t macro_speedup; // as in `block_0_a2m_v9_t'
  uint8_t chan_4op_flags; // 4-op channel extension flags
                          // - bit 0   =channels 1,2
                          // - bit 1   =channels 3,4
                          // - bit 2   =channels 5,6
                          // - bit 3   =channels 10,11
                          // - bit 4   =channels 12,13
                          // - bit 5   =channels 14,15
                          // - bit 7..6=unused
  chan_flags_a2m_v10_t chan_flags;  // initial channel flags for CHANNELS_MAX_A2M_V9 channels
} block_0_a2m_v10_t;  // file format version 10

//load_block_0_a2m_v10()

/*** File format version 11 ***/

typedef song_name_a2m_v1_t  // pattern name
        pat_name_a2m_v11_t; // file format version 11..14

//get_pat_names_a2m_v11()
//put_pat_names_a2m_v11()

typedef struct {  // disabled FM-register columns
  // each value is either 0 (disabled) or 1 (enabled)
  struct {
    uint8_t attack_rate;
    uint8_t decay_rate;
    uint8_t sustain_level;
    uint8_t release_rate;
    uint8_t waveform_type;
    uint8_t output_level;
    uint8_t key_scaling_level;
    uint8_t multiplier;
    uint8_t tremolo;
    uint8_t vibrato;
    uint8_t key_scale_rate;
    uint8_t sustain;
  } modulator, carrier;
  uint8_t connection_type;
  uint8_t feedback;
  uint8_t frequency_slide;
  uint8_t panning;
} dis_fmreg_col_a2m_v11_t;  // file format version 11..14

//get_dis_fmreg_col_a2m_v11()
//put_dis_fmreg_col_a2m_v11()

typedef struct {  // song data block
  song_name_a2m_v1_t song_name; // as in `block_0_a2m_v1_t'
  composer_a2m_v1_t composer;   // as in `block_0_a2m_v1_t'
  ins_name_a2m_v9_t ins_names[INSTRUMENTS_MAX_A2M_V9];    // as in `block_0_a2m_v9_t'
  ins_data_a2m_v9_t ins_data[INSTRUMENTS_MAX_A2M_V9];     // as in `block_0_a2m_v9_t'
  ins_macro_a2m_v9_t ins_macros[INSTRUMENTS_MAX_A2M_V9];  // as in `block_0_a2m_v9_t'
  macro_a2m_v9_t macros[INSTRUMENTS_MAX_A2M_V9];          // as in `block_0_a2m_v9_t'
  pat_order_a2m_v1_t pat_order; // as in `block_0_a2m_v1_t'
  uint8_t tempo;          // as in `block_0_a2m_v9_t'
  uint8_t speed;          // as in `block_0_a2m_v9_t'
  uint8_t flags;          // as in `block_0_a2m_v9_t'
  uint16_t pat_length;    // as in `block_0_a2m_v9_t"
  uint8_t num_channels;   // as in `block_0_a2m_v9_t'
  uint16_t macro_speedup; // as in `block_0_a2m_v9_t'
  uint8_t chan_4op_flags;           // as in `block_0_a2m_v10_t'
  chan_flags_a2m_v10_t chan_flags;  // as in `block_0_a2m_v10_t'
  pat_name_a2m_v11_t pat_names[PATTERNS_MAX_A2M_V9];
  dis_fmreg_col_a2m_v11_t dis_fmreg_col[INSTRUMENTS_MAX_A2M_V9];
} block_0_a2m_v11_t;  // file format version 11

//load_block_0_a2m_v11()

/*** File format version 12..13 ***/

typedef struct {  // 4-op instruments
  uint8_t count;  // number of 4-op instruments
  uint8_t indexes[INSTRUMENTS_4OP_MAX_A2M_V12]; // indexes of 4-op instrument pairs
} ins_4op_data_a2m_v12_t;  // file format version 12..14

//get_ins_4op_data_a2m_v12()
//put_ins_4op_data_a2m_v12()

typedef struct {  // song data block
  song_name_a2m_v1_t song_name; // as in `block_0_a2m_v1_t'
  composer_a2m_v1_t composer;   // as in `block_0_a2m_v1_t'
  ins_name_a2m_v9_t ins_names[INSTRUMENTS_MAX_A2M_V9];    // as in `block_0_a2m_v9_t'
  ins_data_a2m_v9_t ins_data[INSTRUMENTS_MAX_A2M_V9];     // as in `block_0_a2m_v9_t'
  ins_macro_a2m_v9_t ins_macros[INSTRUMENTS_MAX_A2M_V9];  // as in `block_0_a2m_v9_t'
  macro_a2m_v9_t macros[INSTRUMENTS_MAX_A2M_V9];          // as in `block_0_a2m_v9_t'
  pat_order_a2m_v1_t pat_order; // as in `block_0_a2m_v1_t'
  uint8_t tempo;          // as in `block_0_a2m_v9_t'
  uint8_t speed;          // as in `block_0_a2m_v9_t'
  uint8_t flags;          // as in `block_0_a2m_v9_t'
  uint16_t pat_length;    // as in `block_0_a2m_v9_t"
  uint8_t num_channels;   // as in `block_0_a2m_v9_t'
  uint16_t macro_speedup; // as in `block_0_a2m_v9_t'
  uint8_t chan_4op_flags;           // as in `block_0_a2m_v10_t'
  chan_flags_a2m_v10_t chan_flags;  // as in `block_0_a2m_v10_t' (see notes)
  pat_name_a2m_v11_t pat_names[PATTERNS_MAX_A2M_V9];              // as in `block_0_a2m_v11_t'
  dis_fmreg_col_a2m_v11_t dis_fmreg_col[INSTRUMENTS_MAX_A2M_V9];  // as in `block_0_a2m_v11_t'
  ins_4op_data_a2m_v12_t ins_4op_data;  // 4-op instrument data
  uint8_t reserved[1024];               // unused
} block_0_a2m_v12_t;  // file format version 12..13

//load_block_0_a2m_v12()

/*** File format version 13 ***/

//get_chan_flags_a2m_v13()
//put_chan_flags_a2m_v13()

/*** File format version 14 ***/

typedef struct {  // song data block
  song_name_a2m_v1_t song_name; // as in `block_0_a2m_v1_t'
  composer_a2m_v1_t composer;   // as in `block_0_a2m_v1_t'
  ins_name_a2m_v9_t ins_names[INSTRUMENTS_MAX_A2M_V9];    // as in `block_0_a2m_v9_t'
  ins_data_a2m_v9_t ins_data[INSTRUMENTS_MAX_A2M_V9];     // as in `block_0_a2m_v9_t'
  ins_macro_a2m_v9_t ins_macros[INSTRUMENTS_MAX_A2M_V9];  // as in `block_0_a2m_v9_t'
  macro_a2m_v9_t macros[INSTRUMENTS_MAX_A2M_V9];          // as in `block_0_a2m_v9_t'
  pat_order_a2m_v1_t pat_order; // as in `block_0_a2m_v1_t'
  uint8_t tempo;          // as in `block_0_a2m_v9_t'
  uint8_t speed;          // as in `block_0_a2m_v9_t'
  uint8_t flags;          // as in `block_0_a2m_v9_t'
  uint16_t pat_length;    // as in `block_0_a2m_v9_t"
  uint8_t num_channels;   // as in `block_0_a2m_v9_t'
  uint16_t macro_speedup; // as in `block_0_a2m_v9_t'
  uint8_t chan_4op_flags;           // as in `block_0_a2m_v10_t'
  chan_flags_a2m_v10_t chan_flags;  // as in `block_0_a2m_v10_t' (see notes)
  pat_name_a2m_v11_t pat_names[PATTERNS_MAX_A2M_V9];              // as in `block_0_a2m_v11_t'
  dis_fmreg_col_a2m_v11_t dis_fmreg_col[INSTRUMENTS_MAX_A2M_V9];  // as in `block_0_a2m_v11_t'
  ins_4op_data_a2m_v12_t ins_4op_data;  // as in `block_0_a2m_v12_t'
  uint8_t reserved[1024];               // as in `block_0_a2m_v12_t'
  uint8_t rows_per_beat;
  int16_t tempo_finetune;
} block_0_a2m_v14_t;  // file format version 14

//load_block_0_a2m_v14()
//save_block_0_a2m_v14()

//save_song_a2m()

#pragma pack(pop)

#endif  // !defined(FORMATS_A2M_H)
