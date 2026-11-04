// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  HSC AdLib Composer / HSC-Tracker song file loader
  Supported file format versions: all
  Filename extension: .hsc
  Warning: no file signature available - use file extension instead.

  File structure:
    `header_hsc_t'
    patterns data (may be incomplete)

*****************************************************************************/

#pragma once

#ifndef FORMATS_HSC_H
#define FORMATS_HSC_H 1

#define NOTE_MAX_HSC (12*9-1) // B-9
#define INSTRUMENTS_MAX_HSC 128
#define CHANNELS_MAX_HSC 9
#define PATTERN_LEN_HSC 64
#define PATTERNS_MAX_HSC 50
#define PATTERN_ORDER_LEN_HSC 50

#pragma pack(push, 1)

typedef struct {
  uint8_t AM_Vibrato_EG_carrier;
  uint8_t AM_Vibrato_EG_modulator;
  uint8_t KSL_Volume_carrier;
  uint8_t KSL_Volume_modulator;
  uint8_t Attack_Decay_carrier;
  uint8_t Attack_Decay_modulator;
  uint8_t Sustain_Release_carrier;
  uint8_t Sustain_Release_modulator;
  uint8_t Feedback_FM;        // bits 3..0: 0..15
  uint8_t WaveForm_carrier;   // bits 1..0: 0..3
  uint8_t WaveForm_modulator; // bits 1..0: 0..3
} ins_fm_data_hsc_t;

//get_ins_fm_data_hsc()

typedef struct {
  ins_fm_data_hsc_t fm_data;
  uint8_t fine_tune;  // bits 7..4: 0..15
} ins_data_hsc_t;

//get_ins_data_hsc()

typedef struct {
  ins_data_hsc_t ins_data[INSTRUMENTS_MAX_HSC];
  uint8_t pat_order[PATTERN_ORDER_LEN_HSC];
  uint8_t unused;
} header_hsc_t;

//get_instruments_hsc()
//get_pat_order_hsc()

#define CMD_NOTE_PACK_HSC(x)   ((x)+1)
#define CMD_NOTE_UNPACK_HSC(x) ((x)-1)
#define CMD_NOTE_OFF_HSC       0x7F
#define CMD_INS_FLAG_HSC       0x80
#define INS_PACK_HSC(x)        (x)
#define INS_UNPACK_HSC(x)      (x)

typedef struct {
  uint8_t command, param;
} pat_event_hsc_t;

//get_pat_event_hsc()

typedef pat_event_hsc_t pat_data_hsc_t[PATTERN_LEN_HSC][CHANNELS_MAX_HSC];

typedef pat_data_hsc_t patterns_hsc_t[PATTERNS_MAX_HSC];

//fix_patterns_hsc()
//get_patterns_hsc()
//load_patterns_hsc()

//load_song_hsc()

#pragma pack(pop)

#endif  // !defined(FORMATS_HSC_H)
