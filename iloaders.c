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

#include "formats/amd.h"
#include "formats/amd/get_title_amd.c" // static
#include "formats/amd/get_composer_amd.c" // static
#include "formats/amd/get_ins_name_amd.c" // static
#include "formats/amd/get_ins_fm_data_amd.c" // static
#include "formats/amd/get_ins_data_amd.c" // static
#include "formats/amd/get_pat_order_amd.c" // static
#include "formats/amd/get_pat_event_amd.c" // static
#include "formats/amd/load_patterns_amd_v16.c" // static
#include "formats/amd/load_patterns_amd_v17.c" // static
#include "formats/amd/load_song_amd.c"

#include "formats/cff/unpack_cff.c" // static
#include "formats/cff/get_title_cff.c" // static
#include "formats/cff/get_composer_cff.c" // static
#include "formats/cff/get_ins_cff.c" // static
#include "formats/cff/get_pat_order_cff.c" // static
#include "formats/cff/is_empty_event_cff.c" // static
#include "formats/cff/get_pat_event_cff.c" // static
#include "formats/cff/update_pat_event_cff.c" // static
#include "formats/cff/get_patterns_cff.c" // static
#include "formats/cff/load_song_cff.c"

#include "formats/dfm.h"
#include "formats/dfm/get_title_dfm.c" // static
#include "formats/dfm/get_ins_name_dfm.c" // static
#include "formats/dfm/get_ins_fm_data_dfm.c" // static
#include "formats/dfm/get_ins_data_dfm.c" // static
#include "formats/dfm/get_pat_order_dfm.c" // static
#include "formats/dfm/get_pat_event_dfm.c" // static
#include "formats/dfm/load_patterns_dfm.c" // static
#include "formats/dfm/load_song_dfm.c"

#define temp_ef_Arpeggio 0xF0
#define temp_ef_rep      0xF1
#define temp_ef_XFVSlide 0xF2

#include "formats/fmk.h"
#include "formats/fmk/get_title_fmk.c" // static
#include "formats/fmk/get_composer_fmk.c" // static
#include "formats/fmk/get_pat_order_fmk.c" // static
#include "formats/fmk/truncate_string_fmk.c" // static
#include "formats/fmk/get_description_fmk.c" // static
#include "formats/fmk/load_description_fmk.c" // static
#include "formats/fmk/get_ins_name_fmk.c" // static
#include "formats/fmk/get_ins_fm_data_fmk.c" // static
#include "formats/fmk/get_ins_data_fmk.c" // static
#include "formats/fmk/load_instruments_fmk.c" // static
#include "formats/fmk/get_pat_event_fmk.c" // static
#include "formats/fmk/fix_patterns_fmk.c" // static
#include "formats/fmk/load_patterns_fmk.c" // static
#include "formats/fmk/load_song_fmk.c"

#include "formats/hsc.h"
#include "formats/hsc/get_ins_fm_data_hsc.c" // static
#include "formats/hsc/get_ins_data_hsc.c" // static
#include "formats/hsc/get_instruments_hsc.c" // static
#include "formats/hsc/get_pat_order_hsc.c" // static
#include "formats/hsc/get_pat_event_hsc.c" // static
#include "formats/hsc/fix_patterns_hsc.c" // static
#include "formats/hsc/get_patterns_hsc.c" // static
#include "formats/hsc/load_patterns_hsc.c" // static
#include "formats/hsc/load_song_hsc.c"

#include "formats/mtk.h"
#include "formats/mtk/get_title_mtk.c" // static
#include "formats/mtk/get_composer_mtk.c" // static
#include "formats/mtk/get_ins_name_mtk.c" // static
#include "formats/mtk/get_ins_fm_data_mtk.c" // static
#include "formats/mtk/get_ins_data_mtk.c" // static
#include "formats/mtk/get_instruments_mtk.c" // static
#include "formats/mtk/get_pat_order_mtk.c" // static
#include "formats/mtk/load_song_mtk.c"

#include "iloaders/rad_file_loader.c"

#include "iloaders/s3m_file_loader.c"

#include "iloaders/sat_file_loader.c"
