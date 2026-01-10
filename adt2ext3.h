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
#include "adt2unit.h"
#include "adt2keyb.h"

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

#include "iloadins.h"

void test_instrument_alt (uint8_t chan, ExtKeyCode fkey,
                          bool loadMacros, bool bankSelector, bool loadArpVib,
                          uint8_t test_ins1, uint8_t test_ins2);
void test_instrument_alt2 (temp_instrument_t *src, uint8_t chan, ExtKeyCode fkey);

#pragma pack(pop)

#endif // !defined(ADT2EXT3_H)
