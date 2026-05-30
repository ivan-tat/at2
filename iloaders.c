// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// This file is included in `adt2ext3.c'

#if !ADT2PLAY
#include "iloaders/adjust_macro_speedup_with_notice.c" // HINT: static
#include "formats/_save_instrument_marks.c" // static
#include "formats/_restore_instrument_marks.c" // static
#include "formats/_save_pattern_marks.c" // static
#include "formats/_restore_pattern_marks.c" // static
#endif // !ADT2PLAY

#include "formats/a2m/load_song_a2m.c"

#include "formats/a2t.h"
#include "formats/a2t/load_ins_data_a2t_v1.c" // static
#include "formats/a2t/load_ins_data_a2t_v5.c" // static
#include "formats/a2t/load_ins_data_a2t_v9.c" // static
#include "formats/a2t/load_ins_macros_a2t_v9.c" // static
#include "formats/a2t/load_macros_a2t_v9.c" // static
#include "formats/a2t/load_dis_fmreg_col_a2t_v11.c" // static
#include "formats/a2t/load_block_0_a2t_v12.c" // static
#include "formats/a2t/load_block_0_a2t_v14.c" // static
#include "formats/a2t/load_pat_order_a2t_v1.c" // static
#include "formats/a2t/load_song_a2t.c"

#if !ADT2PLAY
#include "formats/a2p.h"
#include "formats/a2p/unpack_a2p.c" // static
#include "formats/a2p/load_pattern_a2p_v1.c" // static
#include "formats/a2p/load_pattern_a2p_v5.c" // static
#include "formats/a2p/load_pattern_a2p_v9.c" // static
#include "formats/a2p/load_pattern_a2p_v10.c" // static
#include "formats/a2p/load_pattern_a2p.c"
#endif // !ADT2PLAY

static uint8_t dec2hex (uint8_t dec)
{
  return (dec / 10) * 16 + (dec % 10);
}

#include "iloaders/amd_file_loader.c"
#include "iloaders/cff_file_loader.c"
#include "iloaders/dfm_file_loader.c"
#include "iloaders/hsc_file_loader.c"
#include "iloaders/mtk_file_loader.c"
#include "iloaders/rad_file_loader.c"

#define temp_ef_Arpeggio 0xF0
#define temp_ef_rep      0xF1
#define temp_ef_XFVSlide 0xF2

#include "iloaders/s3m_file_loader.c"
#include "iloaders/fmk_file_loader.c"
#include "iloaders/sat_file_loader.c"
