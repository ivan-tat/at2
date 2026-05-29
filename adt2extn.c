// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#if GO32
#include <dos.h>
#else // !GO32
#include <SDL/SDL_timer.h>
#if USE_FPC
#include "SDL/SDL_timer_fix.h"
#endif // USE_FPC
#endif // !GO32
#include "debug.h"
#include "adt2sys.h"
#include "adt2unit.h"
#include "adt2ext2.h"
#include "adt2ext3.h"
#include "adt2pack.h"
#include "dialogio.h"
#include "parserio.h"
#include "stringio.h"
#include "txtscrio.h"
#include "adt2extn.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

uint8_t remap_mtype         = 1;
uint8_t remap_ins1          = 1;
uint8_t remap_ins2          = 1;
uint8_t remap_selection     = 1;
uint8_t rearrange_selection = 1;
uint8_t replace_selection   = 1;
bool    replace_prompt = false;
struct pattern_event_replace_data_s replace_data =
{
  .event_to_find =
  {
    .note = { "\x03" "???" },
    .inst = { "\x02" "??" },
    .fx_1 = { "\x03" "???" },
    .fx_2 = { "\x03" "???" }
  },
  .new_event =
  {
    .note = { "\x03" "???" },
    .inst = { "\x02" "??" },
    .fx_1 = { "\x03" "???" },
    .fx_2 = { "\x03" "???" }
  }
};

ExtKeyCode fkey;

uint8_t tracing_block_pattern;
uint8_t tracing_block_xend;
uint8_t tracing_block_yend;

uint8_t copypos1 = 1;
uint8_t copypos2 = 1;
uint8_t copypos3 = 1;
uint8_t copypos4 = 1;
uint8_t clearpos = 1;
uint8_t pattern_list__page = 1;
uint8_t pattern2use = UINT8_NULL;

#include "adt2extn/_patts_marked.c"

#include "adt2extn/nul_volume_bars.c"

/*static*/ bool    transp_menu2 = false;
/*static*/ uint8_t transp_pos1 = 1;
/*static*/ uint8_t transp_pos2 = 1;

/*static*/ String PASCALSTRING last_dir[4][DIR_SIZE] = // HINT: (FPC) start index 1
{
  "\x00""", "\x00""", "\x00""", "\x00"""
};
/*static*/ String PASCALSTRING last_file[4][FILENAME_SIZE] = // HINT: (FPC) start index 1
{
  "\x09""FNAME:EXT",
  "\x09""FNAME:EXT",
  "\x09""FNAME:EXT",
  "\x09""FNAME:EXT"
};

#include "adt2extn/fselect_external_proc.c" // HINT: static

#include "formats/next_saver_step.c" // static
#include "formats/calc_packed_limit.c" // static

/*** songs ***/

#include "adt2extn/show_song_description.c" // HINT: static

#include "formats/_block_sizes_crc32.c" // static

#include "formats/a2m.h"
#include "formats/a2m/put_string_a2m.c" // static
#include "formats/a2m/put_song_name_a2m_v1.c" // static
#include "formats/a2m/put_composer_a2m_v1.c" // static
#include "formats/a2m/put_ins_names_a2m_v9.c" // static
#include "formats/a2m/put_ins_fm_data_a2m_v1.c" // static
#include "formats/a2m/put_ins_data_a2m_v9.c" // static
#include "formats/a2m/put_ins_macro_a2m_v9.c" // static
#include "formats/a2m/put_arp_macro_a2m_v9.c" // static
#include "formats/a2m/put_vib_macro_a2m_v9.c" // static
#include "formats/a2m/put_macro_a2m_v9.c" // static
#include "formats/a2m/put_dis_fmreg_col_a2m_v11.c" // static
#include "formats/a2m/put_ins_4op_data_a2m_v12.c" // static
#include "formats/a2m/put_chan_4op_flags_a2m_v10.c" // static
#include "formats/a2m/put_chan_flags_a2m_v13.c" // static
#include "formats/a2m/put_pat_order_a2m_v1.c" // static
#include "formats/a2m/put_pat_names_a2m_v11.c" // static
#include "formats/a2m/put_pat_data_a2m_v9.c" // static
#include "formats/a2m/put_flags_a2m_v9.c" // static
#include "formats/a2m/pack_a2m.c" // static
#include "formats/a2m/save_block_0_a2m_v14.c" // static
#include "formats/a2m/save_patterns_a2m_v9.c" // static
#include "formats/a2m/save_song_a2m.c"

#include "formats/a2t.h"
#include "formats/a2t/save_pat_order_a2t_v1.c" // static
#include "formats/a2t/save_ins_macros_a2t_v9.c" // static
#include "formats/a2t/save_macros_a2t_v9.c" // static
#include "formats/a2t/save_dis_fmreg_col_a2t_v11.c" // static
#include "formats/a2t/save_block_0_a2t_v14.c" // static
#include "formats/a2t/save_song_a2t.c"

/*** instruments ***/

#include "formats/a2f.h"
#include "formats/a2f/put_ins_macro_a2f_v1.c" // static
#include "formats/a2f/put_ins_a2f_v1.c" // static
#include "formats/a2f/pack_a2f.c" // static
#include "formats/a2f/save_block_0_a2f_v2.c" // static
#include "formats/a2f/save_instrument_a2f.c"

#include "formats/a2i.h"
#include "formats/a2i/put_ins_a2i_v9.c" // static
#include "formats/a2i/pack_a2i.c" // static
#include "formats/a2i/save_block_0_a2i_v10.c" // static
#include "formats/a2i/save_instrument_a2i.c"

/*** instrument banks ***/

#include "formats/a2b.h"
#include "formats/a2b/pack_a2b.c" // static
#include "formats/a2b/save_block_0_a2b_v10.c" // static
#include "formats/a2b/save_instrument_bank_a2b.c"

#include "formats/a2w.h"
#include "formats/a2w/pack_a2w.c" // static
#include "formats/a2w/save_macros_a2w_v1.c" // static
#include "formats/a2w/save_dis_fmreg_col_a2w_v2.c" // static
#include "formats/a2w/save_block_0_a2w_v3.c" // static
#include "formats/a2w/save_instrument_bank_a2w.c"

/*** patterns ***/

#include "formats/a2p.h"
#include "formats/a2p/pack_a2p.c"
#include "formats/a2p/save_pattern_a2p_v10.c"
#include "formats/a2p/save_pattern_a2p.c"

typedef enum
{
  FTYPE_SONG,
  FTYPE_INSTRUMENT,
  FTYPE_INSTRUMENT_BANK,
  FTYPE_PATTERN
} ftype_t;

static const struct file_saver_t
{
  const char *ext;
  ftype_t type;
  String *path;   // default path
  bool alt_name;  // check `alt_ins_name' option
  union
  {
    int8_t (*song)           (const String *, tFIXED_SONGDATA *, progress_callback_t *, char **);
    int8_t (*instrument)     (const String *, tFIXED_SONGDATA *, unsigned, progress_callback_t *, char **);
    int8_t (*instrument_bank)(const String *, tFIXED_SONGDATA *, progress_callback_t *, char **);
    int8_t (*pattern)        (const String *, tFIXED_SONGDATA *, unsigned, progress_callback_t *, char **);
  } proc;
  const char *name;
} file_savers[] =
{
  {
    .ext = "a2m",
    .type = FTYPE_SONG,
    .path = a2m_default_path,
    .alt_name = false,
    .proc.song = save_song_a2m,
    .name = "A2M Saver"
  },
  {
    .ext = "a2t",
    .type = FTYPE_SONG,
    .path = a2t_default_path,
    .alt_name = false,
    .proc.song = save_song_a2t,
    .name = "A2T Saver"
  },
  {
    .ext = "a2f",
    .type = FTYPE_INSTRUMENT,
    .path = a2f_default_path,
    .alt_name = true,
    .proc.instrument = save_instrument_a2f,
    .name = "A2F Saver"
  },
  {
    .ext = "a2i",
    .type = FTYPE_INSTRUMENT,
    .path = a2i_default_path,
    .alt_name = true,
    .proc.instrument = save_instrument_a2i,
    .name = "A2I Saver"
  },
  {
    .ext = "a2b",
    .type = FTYPE_INSTRUMENT_BANK,
    .path = a2b_default_path,
    .alt_name = false,
    .proc.instrument_bank = save_instrument_bank_a2b,
    .name = "A2B Saver"
  },
  {
    .ext = "a2w",
    .type = FTYPE_INSTRUMENT_BANK,
    .path = a2w_default_path,
    .alt_name = false,
    .proc.instrument_bank = save_instrument_bank_a2w,
    .name = "A2W Saver"
  },
  {
    .ext = "a2p",
    .type = FTYPE_PATTERN,
    .path = a2p_default_path,
    .alt_name = false,
    .proc.pattern = save_pattern_a2p,
    .name = "A2P Saver"
  },
  {
    .ext = NULL
  }
};

#include "adt2extn/FILE_save.c"

#pragma pack(pop)
