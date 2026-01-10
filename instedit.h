// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef INSTEDIT_H
#define INSTEDIT_H

#include <stdbool.h>
#include <stdint.h>
#include "adt2keyb.h"

void INSTRUMENT_test (uint8_t instr, uint8_t instr2, uint8_t chan, ExtKeyCode fkey, bool process_macros);
void reset_instrument_preview (void); // HINT: static

uint8_t _1st_marked (void);
uint8_t _2nd_marked (void);
uint8_t marked_instruments (void);
void reset_marked_instruments (void);
uint16_t get_4op_to_test (void);
bool check_4op_flag (uint8_t ins);
uint16_t check_4op_instrument (uint8_t ins);
//#define check_4op_to_test() check_4op_instrument (current_inst)
uint16_t check_4op_to_test (void); // HINT: it seems that it was a `#define' before
void reset_4op_flag (uint8_t ins);
void set_4op_flag (uint8_t ins);
void update_4op_flag_marks (void);

#endif // !DEFINED(INSTEDIT_H)
