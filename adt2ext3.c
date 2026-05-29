// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include <inttypes.h>
#include <stdlib.h>
#include "platform.h"
#include "go32/PIT/PIT_consts.h"
#include "common.h"
#include "debug.h"
#include "adt2pack.h"
#include "adt2keyb.h"
#include "adt2opl3.h"
#if !ADT2PLAY
#include "adt2sys.h"
#endif // !ADT2PLAY
#include "adt2unit.h"
#include "parserio.h"
#include "stringio.h"
#if !ADT2PLAY
#include "dialogio.h"
#include "adt2extn.h"
#include "adt2ext2.h"
#include "adt2ext4.h"
#include "adt2ext5.h"
#endif // !ADT2PLAY
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

/*** used in many loaders ***/

#include "iloaders/next_progress_step.c" // static

#include "formats/_block_sizes_crc16.c" // static
#include "formats/_block_sizes_crc32.c" // static

#include "formats/a2m.h"
#include "formats/a2m/check_crc32_a2m.c" // static
#include "formats/a2m/is_packed_a2m.c" // static
#include "formats/a2m/unpack_a2m.c" // static
#include "formats/a2m/get_song_name_a2m_v1.c" // static
#include "formats/a2m/get_composer_a2m_v1.c" // static
#include "formats/a2m/get_ins_names_a2m_v1.c" // static
#include "formats/a2m/get_ins_names_a2m_v9.c" // static
#include "formats/a2m/get_ins_fm_data_a2m_v1.c" // static
#include "formats/a2m/get_ins_data_a2m_v1.c" // static
#include "formats/a2m/get_ins_data_a2m_v5.c" // static
#include "formats/a2m/get_ins_data_a2m_v9.c" // static
#include "formats/a2m/get_ins_macro_a2m_v9.c" // static
#include "formats/a2m/get_arp_macro_a2m_v9.c" // static
#include "formats/a2m/get_vib_macro_a2m_v9.c" // static
#include "formats/a2m/get_macro_a2m_v9.c" // static
#include "formats/a2m/get_flags_a2m_v5.c" // static
#include "formats/a2m/get_flags_a2m_v9.c" // static
#include "formats/a2m/get_chan_4op_flags_a2m_v10.c" // static
#include "formats/a2m/get_chan_flags_a2m_v10.c" // static
#include "formats/a2m/get_chan_flags_a2m_v13.c" // static
#include "formats/a2m/get_dis_fmreg_col_a2m_v11.c" // static
#include "formats/a2m/get_ins_4op_data_a2m_v12.c" // static
#include "formats/a2m/get_pat_names_a2m_v11.c" // static
#include "formats/a2m/get_pat_order_a2m_v1.c" // static
#include "formats/a2m/get_pat_event_a2m_v1.c" // static
#include "formats/a2m/get_pat_event_a2m_v5.c" // static
#include "formats/a2m/get_pat_event_a2m_v9.c" // static
#include "formats/a2m/get_pat_data_a2m_v1.c" // static
#include "formats/a2m/get_pat_data_a2m_v5.c" // static
#include "formats/a2m/get_pat_data_a2m_v9.c" // static
#include "formats/a2m/load_block_0_a2m_v1.c" // static
#include "formats/a2m/load_block_0_a2m_v5.c" // static
#include "formats/a2m/load_block_0_a2m_v9.c" // static
#include "formats/a2m/load_block_0_a2m_v10.c" // static
#include "formats/a2m/load_block_0_a2m_v11.c" // static
#include "formats/a2m/load_block_0_a2m_v12.c" // static
#include "formats/a2m/load_block_0_a2m_v14.c" // static
#include "formats/a2m/replace_old_adsr_a2m_v1.c" // static, used in `load_patterns_a2m_v1()'
#include "formats/a2m/load_patterns_a2m_v1.c" // static
#include "formats/a2m/load_patterns_a2m_v5.c" // static
#include "formats/a2m/load_patterns_a2m_v9.c" // static
#include "formats/apply_song_flags.c" // static

#include "iloadins.c"

// test_instrument_alt, test_instrument_alt2

#if !ADT2PLAY
static tREGISTER_TABLE ins__fmreg_table_backup;
static tREGISTER_TABLE ins__fmreg_table_backup2;
static tDIS_FMREG_COL ins__dis_fmreg_col_backup;
static tDIS_FMREG_COL ins__dis_fmreg_col_backup2;
static tARP_VIB_MACRO_TABLE arp_vib_macro_table_backup;
static uint8_t chan_handle[20];

#include "adt2ext3/test_instrument_alt_output_note.c" // static
#include "adt2ext3/test_instrument_alt.c"
#include "adt2ext3/test_instrument_alt2.c"
#endif // !ADT2PLAY

#include "iloaders.c"

#pragma pack(pop)
