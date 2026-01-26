// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if !GO32
#include <SDL/SDL_timer.h>
#if USE_FPC
#include "SDL/SDL_timer_fix.h"
#endif // USE_FPC
#endif // !GO32
#include "font.h"
#include "debug.h"
#include "stringio.h"
#include "txtscrio.h"
#include "dialogio.h"
#include "adt2sys.h"
#include "adt2ext3.h"
#include "adt2extn.h"
#include "adt2ext5.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

bool arp_tab_selected = false;
bool vib_tab_selected = false;

uint8_t ptr_arpeggio_table;
uint8_t ptr_vibrato_table;

/*static*/ char _4op_flag_column[255] = { 0, }; // HINT: (FPC) start index 1
/*static*/ uint8_t _a2b_lister_count = 0;
/*static*/ uint8_t _a2w_lister_count = 0;

#include "adt2ext5/check_4op_flag_temp.c" // HINT: static
#include "adt2ext5/get_4op_to_test_temp.c" // HINT: static

// A2B

#include "adt2ext5/a2b_lister_external_proc_callback.c" // HINT: static

// A2W

#include "adt2ext5/a2w_lister_external_proc_callback.c" // HINT: static
#include "adt2ext5/a2w_macro_lister_external_proc_callback.c" // HINT: static

// BNK (bank)

#include "adt2ext5/bnk_lister_external_proc.c" // static
#include "adt2ext5/bnk_file_loader_get_item_str.c" // static
#include "adt2ext5/bnk_file_loader.c"

// FIB (FIN Bank)

#include "adt2ext5/fib_lister_external_proc.c" // static
#include "adt2ext5/fib_file_loader_get_item_str.c" // static
#include "adt2ext5/fib_file_loader.c"

// IBK (Sound Blaster Instrument Bank)

#include "adt2ext5/ibk_lister_external_proc.c" // static
#include "adt2ext5/ibk_file_loader_get_item_str.c" // static
#include "adt2ext5/ibk_file_loader.c"

#pragma pack(pop)
