// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  BoomTracker 4.0 song file format
  Supported file format versions: all
  Filename extension: .cff
  All values are little-endian.

  File structure:
    `header_cff_t'
    if raw data:
      unused 12 bytes;
      data block:
        `header2_cff_t'
        `pat_data_cff_t[0..PATTERNS_MAX_CFF]' (last pattern may be incomplete)
    if packed data:
      unused 28 bytes;
      data block: as raw format but packed.

*****************************************************************************/

#pragma once

#ifndef FORMATS_CFF_H
#define FORMATS_CFF_H 1

#define TITLE_MAX_CFF 20
#define COMPOSER_MAX_CFF TITLE_MAX_CFF
#define INSTRUMENT_NAME_MAX_CFF 20

#define NOTE_MAX_CFF (12 * 8 + 11) // B-9
#define INSTRUMENTS_MAX_CFF 47
#define CHANNELS_MAX_CFF 9
#define PATTERN_LEN_CFF 64
#define PATTERNS_MAX_CFF 36
#define PATTERN_ORDER_LEN_CFF 64

#define ID_SIZE_CFF_V1 16
static const char GCC_ATTRIBUTE((nonstring)) id_cff_v1[ID_SIZE_CFF_V1] = { "<CUD-FM-File>\x1A\xDE\xE0" };

#pragma pack(push, 1)

typedef struct {
  char id[ID_SIZE_CFF_V1];
  uint8_t version;      // format version (unused)
  uint16_t size;        // file size -32 (unused)
  uint8_t packed_flag;  // 1=compressed data, otherwise=raw
} header_cff_t;

#define RAW_DATA_START_CFF    (sizeof (header_cff_t) + 12)
#define PACKED_DATA_START_CFF (sizeof (header_cff_t) + 28)

typedef char  // title
        title_cff_t[TITLE_MAX_CFF];

//get_title_cff()

typedef title_cff_t // composer
        composer_cff_t;

//get_composer_cff()

typedef struct {  // instrument FM-register data
  uint8_t AM_Vibrato_EG_carrier;
  uint8_t AM_Vibrato_EG_modulator;
  uint8_t KSL_Volume_carrier;
  uint8_t KSL_Volume_modulator;
  uint8_t Attack_Decay_carrier;
  uint8_t Attack_Decay_modulator;
  uint8_t Sustain_Release_carrier;
  uint8_t Sustain_Release_modulator;
  uint8_t Feedback_FM;        // 0..15
  uint8_t WaveForm_carrier;   // 0..3
  uint8_t WaveForm_modulator; // 0..3
} ins_fm_data_cff_t;

//get_ins_fm_data_cff()

typedef struct {  // instrument data
  ins_fm_data_cff_t fm_data;
  uint8_t unused;
} ins_data_cff_t;

//get_ins_data_cff()

typedef char  // instrument name
        ins_name_cff_t[INSTRUMENT_NAME_MAX_CFF]; // space or zero padded

//get_ins_name_cff()

typedef struct {  // instrument
  ins_data_cff_t ins_data;
  ins_name_cff_t ins_name;
} ins_cff_t;

//get_ins_cff()

#define PATTERN_ORDER_END 0xFF

typedef uint8_t // pattern order
        pat_order_cff_t[PATTERN_ORDER_LEN_CFF];

//get_pat_order_cff()

#define ID_SIZE_CFF_V2 31
static const char GCC_ATTRIBUTE((nonstring)) id_cff_v2[ID_SIZE_CFF_V2] = { "CUD-FM-File - SEND A POSTCARD -" };

typedef struct
{
  ins_cff_t instruments[INSTRUMENTS_MAX_CFF];
  uint8_t num_patterns;
  char id[ID_SIZE_CFF_V2];
  composer_cff_t composer;
  title_cff_t title;
  pat_order_cff_t pat_order;
  uint8_t byteFF; // always 0xFF
} header2_cff_t;

#define NOTE_EMPTY_CFF     0
#define NOTE_OFF_CFF       0x6D
#define NOTE_PACK_CFF(x)   ((x)+1)
#define NOTE_UNPACK_CFF(x) ((x)-1)

typedef struct {  // pattern event
  uint8_t note, fx, param;
} pat_event_cff_t;

//is_empty_event_cff()
//get_pat_event_cff()
//update_pat_event_cff()

typedef pat_event_cff_t // pattern data
        pat_data_cff_t[PATTERN_LEN_CFF][CHANNELS_MAX_CFF];

typedef pat_event_cff_t // all patterns data
        cff_patterns_t[PATTERNS_MAX_CFF][PATTERN_LEN_CFF][CHANNELS_MAX_CFF];

//get_patterns_cff()

//load_song_cff()

#pragma pack(pop)

#endif  // !defined(FORMATS_CFF_H)
