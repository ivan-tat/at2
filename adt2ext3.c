// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdlib.h>
#include "debug.h"
#include "depackio.h"
#include "adt2pack.h"
#include "adt2keyb.h"
#include "adt2opl3.h"
#include "adt2sys.h"
#include "adt2unit.h"
#include "parserio.h"
#include "dialogio.h"
#include "adt2extn.h"
#include "adt2ext2.h"
#include "adt2ext4.h"
#include "adt2ext5.h"
#include "adt2ext3.h"
#include <errno.h> // last to fix macro redefinition

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

#include "iloadins.c"

// test_instrument_alt, test_instrument_alt2

static tREGISTER_TABLE ins__fmreg_table_backup;
static tREGISTER_TABLE ins__fmreg_table_backup2;
static tDIS_FMREG_COL ins__dis_fmreg_col_backup;
static tDIS_FMREG_COL ins__dis_fmreg_col_backup2;
static tARP_VIB_MACRO_TABLE arp_vib_macro_table_backup;
static uint8_t chan_handle[20];

#include "adt2ext3/test_instrument_alt_output_note.c" // static
#include "adt2ext3/test_instrument_alt.c"
#include "adt2ext3/test_instrument_alt2.c"

#pragma pack(pop)
