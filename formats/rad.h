// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  Reality Adlib Tracker song file loader
  Supported file format versions: 1.0 (16)
  Filename extension: .rad
  All values are little-endian.

  File structure:
    `header_rad_t'
    optional data follows (variable size):
      song description (packed)
      instruments data
      pattern order
      patterns data (packed)

*****************************************************************************/

#pragma once

#ifndef FORMATS_RAD_H
#define FORMATS_RAD_H 1

#define NOTE_MAX_RAD (12*8)
#define INSTRUMENTS_MAX_RAD 31
#define CHANNELS_MAX_RAD 9
#define PATTERN_LEN_RAD 64
#define PATTERNS_MAX_RAD 32
#define PATTERN_ORDER_LEN_RAD 128

#define IS_NOTE_RAD(a)       ((((a) & 0x0F) >= 1) && (((a) & 0x0F) <= 12))
#define IS_NOTE_EMPTY_RAD(a) (((a) & 0x0F) == 0)
#define IS_NOTE_OFF_RAD(a)   (((a) & 0x0F) == 0x0F)
#define NOTE_UNPACK_RAD(a)   ((((a) >> 4) & 7) * 12 + ((a) & 0x0F) - 1)
#define INS_GET_RAD(a, b)    ((((a) & 0x80) >> 3) + ((b) >> 4))
#define INS_EMPTY_RAD        0
#define INS_UNPACK_RAD(x)    ((x)-1)

#pragma pack(push, 1)

#define ID_SIZE_RAD 16
static const char GCC_ATTRIBUTE((nonstring)) id_rad[ID_SIZE_RAD] = { "RAD by REALiTY!!" };

typedef struct {
  char id[ID_SIZE_RAD];
  uint8_t version;
  uint8_t flags;  // bit 7: 1=description follows
                  // bit 6: 1="slow-timer" tune
                  // bit 5: ?
                  // bits 4..0: initial speed (1..31)
} header_rad_t;

//truncate_string_rad()
//get_description_rad()

typedef ins_fm_data_hsc_t ins_fm_data_rad_t;

//get_ins_fm_data_rad()
//get_ins_data_rad()
//get_instruments_rad()

//get_pat_order_rad()

//get_pat_event_rad()
//get_patterns_rad()

//load_song_rad()

#pragma pack(pop)

#endif  // !defined(FORMATS_RAD_H)
