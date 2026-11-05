// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  MPU-401 Trakker song file loader
  Supported file format versions: all
  Filename extension: .mtk
  All values are little-endian.

  File structure:
    `header_mtk_t'
    packed data (variable size):
      `song_header_mtk_t'
      pattern data (same format as in HSC loader)

*****************************************************************************/

#pragma once

#ifndef FORMATS_MTK_H
#define FORMATS_MTK_H 1

#define TITLE_LEN_MTK 33
#define COMPOSER_LEN_MTK 33
#define INSTRUMENT_NAME_LEN_MTK 33

#define INSTRUMENTS_MAX_MTK INSTRUMENTS_MAX_HSC
#define CHANNELS_MAX_MTK    CHANNELS_MAX_HSC
#define PATTERN_LEN_MTK     PATTERN_LEN_HSC
#define PATTERNS_MAX_MTK    PATTERNS_MAX_HSC
#define PATTERN_ORDER_LEN_MTK 128

#pragma pack(push, 1)

#define ID_SIZE_MTK 18
static const char GCC_ATTRIBUTE((nonstring)) id_mtk[ID_SIZE_MTK] = { "mpu401tr\x92kk\xEEr@data" };

typedef struct {
  char id[ID_SIZE_MTK];
  uint16_t crc16;
  uint16_t blen;  // unpacked data size
} header_mtk_t;

typedef struct {
  uint8_t length;
  char data[TITLE_LEN_MTK];
} title_mtk_t;

//get_title_mtk()

typedef struct {
  uint8_t length;
  char data[COMPOSER_LEN_MTK];
} composer_mtk_t;

//get_composer_mtk()

typedef struct {
  uint8_t length;
  char data[INSTRUMENT_NAME_LEN_MTK];
} ins_name_mtk_t;

//get_ins_name_mtk()

typedef ins_fm_data_hsc_t ins_fm_data_mtk_t;

//get_ins_fm_data_mtk()

typedef ins_data_hsc_t ins_data_mtk_t;

//get_ins_data_mtk()

typedef struct {
  title_mtk_t title;
  composer_mtk_t composer;
  ins_name_mtk_t ins_names[INSTRUMENTS_MAX_MTK];
  ins_data_mtk_t ins_data[INSTRUMENTS_MAX_MTK];
  uint8_t pat_order[PATTERN_ORDER_LEN_MTK];
  uint8_t unused;
} song_header_mtk_t;

//get_pat_order_mtk()

#define CMD_NOTE_PACK_MTK   CMD_NOTE_PACK_HSC
#define CMD_NOTE_UNPACK_MTK CMD_NOTE_UNPACK_HSC
#define CMD_NOTE_OFF_MTK    CMD_NOTE_OFF_HSC
#define CMD_INS_FLAG_MTK    CMD_INS_FLAG_HSC
#define INS_PACK_MTK        INS_PACK_HSC
#define INS_UNPACK_MTK      INS_UNPACK_HSC

typedef pat_event_hsc_t pat_event_mtk_t;

#define get_pat_event_mtk get_pat_event_hsc

typedef pat_data_hsc_t pat_data_mtk_t;

typedef patterns_hsc_t patterns_mtk_t;

#define fix_patterns_mtk fix_patterns_hsc
#define get_patterns_mtk get_patterns_hsc

#pragma pack(pop)

#endif  // !defined(FORMATS_MTK_H)
