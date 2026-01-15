// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// This file is included in `adt2ext3.c'

///--- used in many loaders ---///

typedef struct
{
  const void *buf;
  size_t size;
  const void *curptr, *endptr;
} mem_stream_t;

#include "iloadins/set_mem_stream.c" // static
#include "iloadins/read_bytes.c" // static
#include "iloadins/read_string.c" // static

#include "iloadins/import_standard_instrument_alt.c"
#include "iloadins/import_hsc_instrument_alt.c"
#include "iloadins/set_default_ins_name_if_needed.c" // HINT: static
#include "iloadins/apply_instrument.c"

// A2I

#include "iloadins/a2i_file_loader_alt.c"

// A2F

#include "iloadins/a2f_file_loader_alt.c"

// CIF

#pragma pack(push, 1)
typedef struct
{
  char ident[20];
  tFM_INST_DATA idata;
  uint8_t resrv;
  char iname[20];
} tCIF_DATA;
#pragma pack(pop)

#include "iloadins/cif_file_loader_alt.c"

// FIN

#pragma pack(push, 1)
typedef struct
{
  char dname[FIN_DOS_NAME_LEN]; // DOS file name padded with spaces (not terminated by NUL)
  char iname[FIN_INS_NAME_LEN]; // instrument name padded with NUL (may not be terminated by NUL)
  tFM_INST_DATA idata;
  uint8_t perc_voice;
} tFIN_DATA;
#pragma pack(pop)

#include "iloadins/import_fin_instrument_alt.c"
#include "iloadins/fin_file_loader_alt.c"

// INS

#pragma pack(push, 1)
typedef struct
{
  tFM_INST_DATA idata;
  uint8_t slide;
  uint8_t _SAdT[19];
} tINS_DATA;
#pragma pack(pop)

#include "iloadins/import_sat_instrument_alt.c"
#include "iloadins/ins_file_loader_alt.c"

// SBI (Sound Blaster Instrument)

#pragma pack(push, 1)
typedef struct
{
  char ident[4];
  char iname[32];
  tFM_INST_DATA idata;
  uint8_t dummy[5];
} tSBI_DATA;
#pragma pack(pop)

#include "iloadins/import_sbi_instrument_alt.c" // static
#include "iloadins/sbi_file_loader_alt.c"

// SGI

#pragma pack(push, 1)
typedef struct
{
  struct
  {
    uint8_t attack, decay, sustain, release,
            waveform, mfmult, ksl, volume, ksr, tremolo, vibrato, eg_type;
  } m, c;
  uint8_t feedback, fm;
} tSGI_DATA;
#pragma pack(pop)

#include "iloadins/import_sgi_instrument.c" // static
#include "iloadins/sgi_file_loader_alt.c"

///--- Instrument banks ---///

#define MAX_TIMBRES 4096

// BNK (Bank)

static const char GCC_ATTRIBUTE((nonstring)) bnk_id[6] = { "ADLIB-" };

#pragma pack(push, 1)
typedef struct
{
  uint16_t data_index;
  bool usage_flag;
  char ins_name[8+1];
} tBNK_NAME_RECORD;

typedef struct
{
  uint8_t ksl;
  uint8_t freq_mult;
  uint8_t feedback;
  uint8_t attack;
  uint8_t sust_level;
  uint8_t sustain;
  uint8_t decay;
  uint8_t release;
  uint8_t output;
  uint8_t am;
  uint8_t vib;
  uint8_t ksr;
  uint8_t fm;
} tBNK_OPERATOR;

typedef struct
{
  uint8_t mode;
  uint8_t voice_num;
  tBNK_OPERATOR modulator;
  tBNK_OPERATOR carrier;
  uint8_t wform_mod;
  uint8_t wform_car;
} tBNK_DATA_RECORD;

typedef struct
{
  uint8_t fver_major;
  uint8_t fver_minor;
  char signature[6];
  uint16_t entries_used;
  uint16_t total_entries;
  int32_t name_offset;
  int32_t data_offset;
  uint8_t filler[8];
} tBNK_HEADER;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
  uint16_t capacity;
  uint16_t count;
  tBNK_NAME_RECORD *names;
  tBNK_DATA_RECORD *data;
} bnk_t;
#pragma pack(pop)

#include "iloadins/import_bnk_instrument.c" // static

#include "iloadins/bnk_new.c" // static
#include "iloadins/bnk_free.c" // static
#include "iloadins/load_bnk.c" // static

#include "iloadins/bnk_bank_new.c"
#include "iloadins/bnk_bank_free.c"
#include "iloadins/import_bnk_bank_from_bnk.c" // static
#include "iloadins/load_bnk_bank.c"

#include "iloadins/bnk_file_loader_alt.c"

// FIB (FIN Bank)

static const char GCC_ATTRIBUTE((nonstring)) fib_id[4] = { "FIB\xF4" };

#pragma pack(push, 1)
typedef struct
{
  char ident[4];
  uint16_t nmins;
} tFIB_HEADER;

typedef struct
{
  uint16_t capacity;
  uint16_t count;
  tFIN_DATA *instruments;
} fib_t;
#pragma pack(pop)

#include "iloadins/fib_new.c" // static
#include "iloadins/fib_free.c" // static
#include "iloadins/load_fib.c" // static

#include "iloadins/fin_bank_new.c"
#include "iloadins/fin_bank_free.c"
#include "iloadins/import_fin_bank_from_fib.c" // static
#include "iloadins/load_fin_bank.c"

#include "iloadins/fib_file_loader_alt.c"

// IBK (Sound Blaster Instrument Bank)

static const char GCC_ATTRIBUTE((nonstring)) ibk_id[4] = { "IBK\x1A" };

#pragma pack(push, 1)
typedef struct
{
  struct
  {
    tFM_INST_DATA idata;
    uint8_t perc_voice;
    uint8_t dummy[4];
  } instruments[SBI_BANK_CAPACITY];
  char names[SBI_BANK_CAPACITY][8+1];
} ibk_t;
#pragma pack(pop)

#include "iloadins/ibk_new.c" // static
#include "iloadins/ibk_free.c" // static
#include "iloadins/load_ibk.c" // static

#include "iloadins/sbi_bank_new.c"
#include "iloadins/sbi_bank_free.c"
#include "iloadins/import_sbi_bank_from_ibk.c" // static
#include "iloadins/load_sbi_bank.c"

#include "iloadins/ibk_file_loader_alt.c"
