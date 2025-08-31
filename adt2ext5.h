// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef ADT2EXT5_H
#define ADT2EXT5_H

#include "defines.h"
#include <stdbool.h>
#include <stdint.h>

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

extern bool arp_tab_selected;
extern bool vib_tab_selected;

extern uint8_t ptr_arpeggio_table;
extern uint8_t ptr_vibrato_table;

//void bnk_file_loader (void);
//void fib_file_loader (void);
//void ibk_file_loader (void);
void bnk_file_loader_alt (uint16_t instr);
void fib_file_loader_alt (uint16_t instr);
void ibk_file_loader_alt (uint16_t instr);
//void a2b_file_loader (bool bankSelector, bool loadBankPossible);
//void a2w_file_loader (bool loadFromFile, bool loadMacros, bool bankSelector,
//                      bool loadBankPossible, bool updateCurInstr);

#pragma pack(pop)

#endif // !defined(ADT2EXT5_H)
