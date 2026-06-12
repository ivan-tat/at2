// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  BoomTracker 4.0 instrument file format
  Supported file format versions: all
  Filename extension: .cif

  File structure:
    `header_cif_t'
    `ins_cif_t'

*****************************************************************************/

#pragma once

#ifndef FORMATS_CIF_H
#define FORMATS_CIF_H 1

#define INSTRUMENT_NAME_MAX_CIF INSTRUMENT_NAME_MAX_CFF

#define ID_SIZE_CIF 20

static const char GCC_ATTRIBUTE((nonstring)) id_cif[ID_SIZE_CIF] = { "<CUD-FM-Instrument>\x1A" };

#pragma pack(push, 1)

typedef struct {  // file header
  char id[ID_SIZE_CIF]; // ID string
} header_cif_t;

typedef ins_fm_data_cff_t // instrument FM-register data
        ins_fm_data_cif_t;

#define get_ins_fm_data_cif get_ins_fm_data_cff

typedef ins_data_cff_t  // instrument data
        ins_data_cif_t;

#define get_ins_data_cif  get_ins_data_cff

typedef ins_name_cff_t  // instrument name
        ins_name_cif_t;

#define get_ins_name_cif  get_ins_name_cff

typedef ins_cff_t // instrument
        ins_cif_t;

//get_ins_cif()

//load_instrument_cif()

#pragma pack(pop)

#endif  // !defined(FORMATS_CIF_H)
