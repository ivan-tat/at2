// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  Digital-FM Tracker song file format
  Supported file format versions: all
  Filename extension: .dfm
  All values are little-endian.

  File structure:
    `header_dfm_t'
    packed pattern data follows (see appropriate function)

*****************************************************************************/

#pragma once

#ifndef FORMATS_DFM_H
#define FORMATS_DFM_H 1

#define TITLE_LEN_DFM 32
#define INSTRUMENT_NAME_LEN_DFM 11

#define INSTRUMENTS_MAX_DFM 32
#define CHANNELS_MAX_DFM 9
#define PATTERN_LEN_DFM 64
#define PATTERNS_MAX_DFM 128
#define PATTERN_ORDER_LEN_DFM 128
#define PATTERN_ORDER_END_DFM 0x80

#define ID_SIZE_DFM 4
static const char GCC_ATTRIBUTE((nonstring)) id_dfm[ID_SIZE_DFM] = { "DFM\x1A" };

#pragma pack(push, 1)

typedef struct {
  uint8_t length;
  char data[TITLE_LEN_DFM];
} title_dfm_t;

//get_title_dfm()

typedef struct {
  uint8_t length;
  char data[INSTRUMENT_NAME_LEN_DFM];
} ins_name_dfm_t;

//get_ins_name_dfm()

typedef struct {
  uint8_t AM_Vibrato_EG_modulator;
  uint8_t AM_Vibrato_EG_carrier;
  uint8_t KSL_Volume_modulator;
  uint8_t KSL_Volume_carrier;
  uint8_t Attack_Decay_modulator;
  uint8_t Attack_Decay_carrier;
  uint8_t Sustain_Release_modulator;
  uint8_t Sustain_Release_carrier;
  uint8_t WaveForm_modulator;
  uint8_t WaveForm_carrier;
  uint8_t Feedback_FM;
} ins_fm_data_dfm_t;

//get_ins_fm_data_dfm()
//get_ins_data_dfm()

typedef uint8_t pat_order_dfm_t[PATTERN_ORDER_LEN_DFM];

//get_pat_order_dfm()

typedef struct
{
  char id[ID_SIZE_DFM];
  uint16_t version;     // tracker version (not used)
  title_dfm_t title;
  uint8_t tempo;
  ins_name_dfm_t ins_names[INSTRUMENTS_MAX_DFM];
  ins_fm_data_dfm_t ins_data[INSTRUMENTS_MAX_DFM];
  pat_order_dfm_t pat_order;
  uint8_t num_patterns; // number of patterns (not used)
} header_dfm_t;

//get_pat_event_dfm()
//load_patterns_dfm()

//load_song_dfm()

#pragma pack(pop)

#endif  // !defined(FORMATS_DFM_H)
