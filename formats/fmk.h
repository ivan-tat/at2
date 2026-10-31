// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  FM-Kingtracker song file format
  Supported file format versions: all
  Filename extension: .fmk
  All values are little-endian.

  File structure:
    `header_fmk_t'
    variable header data (see `var_data_fmk_t')
    description
    instruments data
    patterns data

*****************************************************************************/

#pragma once

#ifndef FORMATS_FMK_H
#define FORMATS_FMK_H 1

#define TITLE_MAX_FMK 28
#define COMPOSER_MAX_FMK 28
#define DESC_LINE_SIZE_FMK 20
#define DESC_LINES_MAX_FMK 24
#define INS_DOS_NAME_MAX_FMK 12
#define INS_NAME_MAX_FMK 27

#define OCTAVES_MAX_FMK 8
#define INSTRUMENTS_MAX_FMK 99
#define MELODIC_CHANNELS_MAX_FMK 18
#define CHANNELS_MAX_FMK 20
#define PATTERN_LEN_FMK 64
#define PATTERNS_MAX_FMK 64
#define PATTERN_ORDER_LEN_FMK 255
#define PATTERN_ORDER_END_FMK 0xFF

#pragma pack(push, 1)

#define ID_FMK MK_UINT32('F','M','K','!')

// flags
#define FL_STEREO_FMK   (1<<0)
#define FL_OPL3_FMK     (1<<1)
#define FL_RYTHM_FMK    (1<<2)
#define FL_TREMOLO_FMK  (1<<3)  // 4.8 db tremolo
#define FL_VIBRATO_FMK  (1<<4)  // 14 cent vibrato

typedef char title_fmk_t[TITLE_MAX_FMK]; // ASCIIZ but may not be NULL-terminated

//get_title_fmk()

typedef char composer_fmk_t[COMPOSER_MAX_FMK];  // ASCIIZ but may not be NULL-terminated

//get_composer_fmk()

typedef struct {
  uint32_t id;
  title_fmk_t title;
  composer_fmk_t composer;
  uint8_t bytef4;     // =0xF4
  uint8_t version;    // 1=mixed channels, 2=melodic channels only
  uint8_t flags;
  uint8_t base_speed; // Base speed (ticks / second). If set to 0 -> use 50.
  uint8_t init_speed; // Initial speed. If set to 0 -> use 1.
  uint8_t unused[9];
  uint8_t pat_order_len;
  uint8_t num_instruments;
  uint8_t num_patterns;
  uint8_t panning[5]; // Stereo pan positions for 18 channels, 36 bits total:
                      // bits 2*N-1..2*N-2 (where N=1..18): 0=left, 1=both, 2=right
} header_fmk_t;

typedef struct {
  uint8_t chan_set[CHANNELS_MAX_FMK]; // Channels settings
    // * bits 7..3=OPL-channel number: 1..18, 31=none
    // * bits 2..0=type: 0=melodic, 1=hi-hat, 2=top cymbal, 3=tom-tom, 4=snare drum, 5=bass drum, 6=4-op, 7=unused
    // For version 2 -> channel: 1..18, 31=unused; type: 0=normal, 6=4-op, 7=unused
  uint8_t pat_order[PATTERN_ORDER_LEN_FMK];
  uint16_t desc_offset;
  uint16_t ins_offsets[INSTRUMENTS_MAX_FMK];
  uint32_t pat_offsets[PATTERNS_MAX_FMK];
} var_data_fmk_t;

//get_pat_order_fmk()

//load_description_fmk()

typedef struct {
  uint8_t AM_Vibrato_EG_modulator;
  uint8_t AM_Vibrato_EG_carrier;
  uint8_t KSL_Volume_modulator;
  uint8_t KSL_Volume_carrier;
  uint8_t Attack_Decay_modulator;
  uint8_t Attack_Decay_carrier;
  uint8_t Sustain_Release_modulator;
  uint8_t Sustain_Release_carrier;
  uint8_t WaveForm_modulator; // 0..7
  uint8_t WaveForm_carrier;   // 0..7
  uint8_t Feedback_FM;        // 0..15
} ins_fm_data_fmk_t;

typedef struct {
  char dos_name[INS_DOS_NAME_MAX_FMK];  // DOS file name padded with spaces (not terminated by NUL)
  char ins_name[INS_NAME_MAX_FMK];      // Instrument name padded with spaces (not terminated by NUL)
  ins_fm_data_fmk_t fm_data;
} ins_data_fmk_t;

//get_ins_name()
//get_ins_fm_data()
//get_ins_data()
//load_instruments()

// pattern event
#define NOTE_OFF_FMK       0xFE
#define NOTE_EMPTY_FMK     0xFF
#define NOTE_PACK_FMK(x)   (x)
#define NOTE_UNPACK_FMK(x) (x)
#define INS_EMPTY_FMK      0
#define INS_PACK_FMK(x)    ((x)+1)
#define INS_UNPACK_FMK(x)  ((x)-1)
#define VOL_EMPTY_FMK      0xFF
#define VOL_PACK_FMK(x)    (x)
#define VOL_UNPACK_FMK(x)  (x)
#define CMD_EMPTY_FMK      0
#define CMD_PACK_FMK(x)    ((x)+1)
#define CMD_UNPACK_FMK(x)  ((x)-1)

typedef struct {
  uint8_t
    chan, // 1..CHANNELS_MAX_FMK
    note,
    ins,
    vol,
    cmd,
    info;
} pat_event_fmk_t;

//get_pat_event_fmk()
//load_patterns_fmk()

//load_song_fmk()

#pragma pack(pop)

#endif  // !defined(FORMATS_FMK_H)
