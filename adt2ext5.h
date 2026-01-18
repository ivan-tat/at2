// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef ADT2EXT5_H
#define ADT2EXT5_H

#include "defines.h"
#include <stdbool.h>
#include <stdint.h>
#include "adt2unit.h"

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

extern bool arp_tab_selected;
extern bool vib_tab_selected;

extern uint8_t ptr_arpeggio_table;
extern uint8_t ptr_vibrato_table;

///--- Instruments banks ---///

// A2B

//void a2b_file_loader (bool bankSelector, bool loadBankPossible, char **error);

// A2W

//void a2w_file_loader (bool loadFromFile, bool loadMacros, bool bankSelector,
//                      bool loadBankPossible, bool updateCurInstr, char **error);

// BNK (bank)

int8_t bnk_file_loader (temp_instrument_t *dst, const String *fname, char **error);

// FIB (FIN Bank)

int8_t fib_file_loader (temp_instrument_t *dst, const String *fname, char **error);

// IBK (Sound Blaster Instrument Bank)

int8_t ibk_file_loader (temp_instrument_t *dst, const String *fname, char **error);

#pragma pack(pop)

#endif // !defined(ADT2EXT5_H)
