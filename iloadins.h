// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// This file is included in `adt2ext3.h'

#pragma once

#ifndef ILOADINS_H
#define ILOADINS_H

///--- used in many loaders ---///

void import_standard_instrument_alt (tADTRACK2_INS *ins, const void *data);
void import_hsc_instrument_alt (tADTRACK2_INS *ins, const void *data);
void set_default_ins_name_if_needed (temp_instrument_t *dst, const String *fname);
void apply_instrument (uint8_t idx, temp_instrument_t *src);

// A2I

int8_t a2i_file_loader_alt (temp_instrument_t *dst, const String *fname, bool swap_ins, progress_callback_t *progress, char **error);

// A2F

int8_t a2f_file_loader_alt (temp_instrument_t *dst, const String *fname, bool swap_ins, progress_callback_t *progress, char **error);

// CIF

int8_t cif_file_loader_alt (temp_instrument_t *dst, const String *fname, char **error);

// FIN

void import_fin_instrument_alt (tADTRACK2_INS *ins, const void *data);
int8_t fin_file_loader_alt (temp_instrument_t *dst, const String *fname, char **error);

// INS

void import_sat_instrument_alt (tADTRACK2_INS *ins, const void *data);
int8_t ins_file_loader_alt (temp_instrument_t *dst, const String *fname, char **error);

// SBI (Sound Blaster Instrument)

//void import_sbi_instrument_alt (tADTRACK2_INS *ins, const void *data);
int8_t sbi_file_loader_alt (temp_instrument_t *dst, const String *fname, char **error);

// SGI

int8_t sgi_file_loader_alt (temp_instrument_t *dst, const String *fname, char **error);

///--- Instrument banks ---///

// BNK (bank)

typedef struct
{
  uint16_t capacity;
  uint16_t count;
  struct bnk_bank_item_t
  {
    bool available;
    char name[8+1];
    tADTRACK2_INS ins;
  } *items;
} bnk_bank_t;

bnk_bank_t *bnk_bank_new (uint16_t capacity, char **error);
void bnk_bank_free (bnk_bank_t *bnk_bank);
bnk_bank_t *load_bnk_bank (const String *fname, char **error);

int8_t bnk_file_loader_alt (temp_instrument_t *dst, const String *fname, uint16_t idx, char **error);

// FIB (FIN Bank)

#define FIN_DOS_NAME_LEN 12 // w/o terminating NUL
#define FIN_INS_NAME_LEN 26 // w/o terminating NUL

typedef struct
{
  uint16_t capacity;
  uint16_t count;
  struct fin_bank_item_t
  {
    bool available;
    char dname[FIN_DOS_NAME_LEN+1];
    char iname[FIN_INS_NAME_LEN+1];
    tADTRACK2_INS ins;
  } *items;
} fin_bank_t;

fin_bank_t *fin_bank_new (uint16_t capacity, char **error);
void fin_bank_free (fin_bank_t *fin_bank);
fin_bank_t *load_fin_bank (const String *fname, char **error);

int8_t fib_file_loader_alt (temp_instrument_t *dst, const String *fname, uint16_t idx, char **error);

// IBK (Sound Blaster Instrument Bank)

#define SBI_BANK_CAPACITY 128

typedef struct
{
  uint8_t count;
  struct sbi_bank_item_t
  {
    bool available;
    char name[8+1];
    tADTRACK2_INS ins;
  } items[SBI_BANK_CAPACITY];
} sbi_bank_t;

sbi_bank_t *sbi_bank_new (char **error);
void sbi_bank_free (sbi_bank_t *sbi_bank);
sbi_bank_t *load_sbi_bank (const String *fname, char **error);

int8_t ibk_file_loader_alt (temp_instrument_t *dst, const String *fname, uint16_t idx, char **error);

#endif // !DEFINED(ILOADINS_H)
