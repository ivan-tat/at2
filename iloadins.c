// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// This file is included in `adt2ext3.c'

// used in INS, FIB, SBI

#include "iloadins/import_standard_instrument_alt.c"

// used in CIF, INS

#include "iloadins/import_hsc_instrument_alt.c" // static

// A2I

#include "iloadins/a2i_file_loader_alt.c" // static

// A2F

#include "iloadins/a2f_file_loader_alt.c" // static

// CIF

#pragma pack(push, 1);
typedef struct
{
  char ident[20];
  tFM_INST_DATA idata;
  uint8_t resrv;
  char iname[20];
} tCIF_DATA;
#pragma pack(pop);

#include "iloadins/cif_file_loader_alt.c" // static

// FIN

#pragma pack(push, 1);
typedef struct
{
  char dname[12];
  char iname[27];
  tFM_INST_DATA idata;
} tFIN_DATA;
#pragma pack(pop);

#include "iloadins/import_fin_instrument_alt.c" // static
#include "iloadins/fin_file_loader_alt.c" // static

// INS

#pragma pack(push, 1);
typedef struct
{
  tFM_INST_DATA idata;
  uint8_t slide;
  uint8_t _SAdT[19];
} tINS_DATA;
#pragma pack(pop);

#include "iloadins/import_sat_instrument_alt.c" // static
#include "iloadins/ins_file_loader_alt.c" // static

// SBI

#pragma pack(push, 1);
typedef struct
{
  char ident[4];
  char iname[32];
  tFM_INST_DATA idata;
  uint8_t dummy[5];
} tSBI_DATA;
#pragma pack(pop);

#include "iloadins/sbi_file_loader_alt.c" // static

// SGI

#pragma pack(push, 1);
typedef struct
{
  struct
  {
    uint8_t attack, decay, sustain, release,
            waveform, mfmult, ksl, volume, ksr, tremolo, vibrato, eg_type;
  } m, c;
  uint8_t feedback, fm;
} tSGI_DATA;
#pragma pack(pop);

#include "iloadins/import_sgi_instrument.c" // HINT: static
#include "iloadins/sgi_file_loader_alt.c" //static

// IBK

#include "iloadins/import_sbi_instrument_alt.c"
#include "iloadins/ibk_file_loader_alt.c"

// test_instrument_alt

static tREGISTER_TABLE ins__fmreg_table_backup;
static tREGISTER_TABLE ins__fmreg_table_backup2;
static tDIS_FMREG_COL ins__dis_fmreg_col_backup;
static tDIS_FMREG_COL ins__dis_fmreg_col_backup2;
static tARP_VIB_MACRO_TABLE arp_vib_macro_table_backup;
static uint8_t chan_handle[20];

#include "iloadins/test_instrument_alt_output_note.c" // static
#include "iloadins/test_instrument_alt.c"

// test_instrument_alt2

#include "iloadins/test_instrument_alt2_output_note.c" // static
#include "iloadins/test_instrument_alt2.c"

#include "iloadins/fselect_external_proc.c"
