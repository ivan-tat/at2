// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef ADT2EXT2_H
#define ADT2EXT2_H

#include <stdbool.h>
#include <stdint.h>

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

extern uint8_t quick_mark_type;
extern bool    discard_block;
extern uint8_t old_chan_pos;
extern uint8_t old_hpos;
extern uint8_t old_page;
extern uint8_t old_block_chan_pos;
extern uint8_t old_block_patt_hpos;
extern uint8_t old_block_patt_page;

#include "ipattord.h"
#include "ipattern.h"

void process_global_keys (void);

#pragma pack(pop)

#endif // !defined(ADT2EXT2_H)
