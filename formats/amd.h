// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  Amusic module file format
  Supported file format versions: 16, 17
  Filename extensions: .amd, .xms
  All values are little-endian.

  File structure:
    `header_amd_t'
    pattern data follows (see appropriate functions)

*****************************************************************************/

#pragma once

#ifndef FORMATS_AMD_H
#define FORMATS_AMD_H 1

#define NOTE_MAX_AMD (12 * 7 + 11) // B-8
#define INSTRUMENTS_MAX_AMD 26
#define CHANNELS_MAX_AMD 9
#define PATTERN_LEN_AMD 64
#define PATTERN_ORDER_LEN_AMD 128

#define NOTE_UNPACK_AMD(x) ((x)-1)
#define NOTE_PACK_AMD(x)   ((x)+1)
#define INS_UNPACK_AMD(x)  ((x)-1)
#define INS_PACK_AMD(x)    ((x)+1)

#pragma pack(push, 1)

#define ID_SIZE_AMD 9

static const char GCC_ATTRIBUTE((nonstring)) id_amd[ID_SIZE_AMD] = { "<o\xEFQU\xEERoR" };
static const char GCC_ATTRIBUTE((nonstring)) id_xms[ID_SIZE_AMD] = { "MaDoKaN96" };

typedef struct {  // instrument FM data
  uint8_t AM_Vibrato_EG_modulator;
  uint8_t KSL_Volume_modulator;
  uint8_t Attack_Decay_modulator;
  uint8_t Sustain_Release_modulator;
  uint8_t WaveForm_modulator; // 0..3
  uint8_t AM_Vibrato_EG_carrier;
  uint8_t KSL_Volume_carrier;
  uint8_t Attack_Decay_carrier;
  uint8_t Sustain_Release_carrier;
  uint8_t WaveForm_carrier;   // 0..3
  uint8_t Feedback_FM;        // 0..15
} ins_fm_data_amd_t;

//get_ins_fm_data_amd()

typedef struct {  // instrument
  char name[23];    // ASCIIZ
  ins_fm_data_amd_t fm_data;
} ins_data_amd_t;

//get_ins_data_amd()

typedef struct {  // header
  char title[24];       // ASCIIZ
  char composer[24];    // ASCIIZ
  ins_data_amd_t instruments[INSTRUMENTS_MAX_AMD];
  uint8_t length;       // song length (see `order')
  uint8_t patterns;     // number of patterns -1
  uint8_t order[PATTERN_ORDER_LEN_AMD];
  char id[ID_SIZE_AMD];
  uint8_t version;
} header_amd_t;

//get_pat_event_amd()

//load_patterns_amd_v16()
//load_patterns_amd_v17()

//load_song_amd()

#pragma pack(pop)

#endif  // !defined(FORMATS_AMD_H)
