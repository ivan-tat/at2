// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef ADT2EXT3_H
#define ADT2EXT3_H

#include "defines.h"
#include <stdbool.h>
#include <stdint.h>
#include "adt2keyb.h"

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

void import_standard_instrument_alt (tADTRACK2_INS *ins, const void *data);
void import_sbi_instrument_alt (tADTRACK2_INS *ins, const void *data);

void ibk_file_loader_alt (uint16_t instr);

void test_instrument_alt (uint8_t chan, AT2ExtKeyCode fkey,
                          bool loadMacros, bool bankSelector, bool loadArpVib,
                          uint8_t test_ins1, uint8_t test_ins2);
void test_instrument_alt2 (uint8_t chan, AT2ExtKeyCode fkey);

void fselect_external_proc (void);

#pragma pack(pop)

#endif // !defined(ADT2EXT3_H)
