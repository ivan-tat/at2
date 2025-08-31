// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef ADT2EXT4_H
#define ADT2EXT4_H

#include "defines.h"
#include <stdbool.h>
#include <stdint.h>

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

extern bool _arp_vib_loader;
extern bool _arp_vib_mode;
extern bool _macro_editor__pos[2]; // HINT: (FPC) index is `boolean'
extern bool _macro_editor__fmreg_hpos[2]; // HINT: (FPC) index is `boolean'
extern bool _macro_editor__fmreg_page[2]; // HINT: (FPC) index is `boolean'
extern bool _macro_editor__fmreg_left_margin[2]; // HINT: (FPC) index is `boolean'
extern bool _macro_editor__fmreg_cursor_pos[2]; // HINT: (FPC) index is `boolean'
extern bool _macro_editor__arpeggio_page[2]; // HINT: (FPC) index is `boolean'
extern bool _macro_editor__vibrato_hpos[2]; // HINT: (FPC) index is `boolean'
extern bool _macro_editor__vibrato_page[2]; // HINT: (FPC) index is `boolean'

extern uint8_t arpvib_arpeggio_table;
extern uint8_t arpvib_vibrato_table;

//void MACRO_EDITOR (uint8_t instr, bool arp_vib_mode);
//void MACRO_BROWSER (bool instrBrowser, bool updateCurInstr);

#pragma pack(pop)

#endif // !defined(ADT2EXT4_H)
