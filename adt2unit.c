// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"
#include "platform.h"
#include "debug.h"
#include <math.h>
#include <time.h>
#if GO32
#include <stdlib.h>
#include "go32/adt2dpmi.h"
#include "go32/iss_tim.h"
#else // !GO32
#include <SDL/SDL_timer.h>
#if USE_FPC
#include "SDL/SDL_timer_fix.h"
#endif // USE_FPC
#endif // !GO32
#include "go32/PIT/PIT_consts.h"
#include "adt2keyb.h"
#include "adt2opl3.h"
#include "adt2sys.h"
#include "adt2unit.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

#if GO32
static const char ___ADT2UNIT_CONST_START___ = 0;
static char       ___ADT2UNIT_DATA_START___ = 0;
static char       ___ADT2UNIT_BSS_START___;
static __NO_REORDER __ALIGNED_(1) __NAKED_RELAXED void
                  ___ADT2UNIT_CODE_START___ (void) { }
#endif // GO32

#include "typcons1.c"
#include "typcons2.c"

uint16_t IRQ_freq          = MIN_IRQ_FREQ;
int16_t  IRQ_freq_shift    = 0;
uint8_t  tempo             = 50;
uint8_t  speed             = 6;
uint16_t macro_speedup     = 1;
bool     timer_initialized = false;
bool     repeat_pattern    = false;
bool     fast_forward      = false;
bool     _rewind           = false;
bool     pattern_break     = false;
bool     pattern_delay     = false;
uint8_t  next_line         = 0;
uint8_t  start_order       = BYTE_NULL;
uint8_t  start_pattern     = BYTE_NULL;
uint8_t  start_line        = BYTE_NULL;
bool     replay_forbidden  = true;
bool     single_play       = false;
bool     calibrating       = false;
bool     no_status_refresh = false;
bool     do_synchronize    = false;
bool     space_pressed     = false;
bool     module_archived   = false;
bool     force_scrollbars  = false;
bool     no_sync_playing   = false;
bool     no_step_debugging = false;
bool     play_single_patt  = false;
bool     no_trace_pattord  = false;
bool     skip_macro_flag   = false;
uint8_t  max_patterns      = 128;
bool     jump_mark_mode    = false;
bool     force_macro_keyon = false;
bool     ins_trailing_flag = false;

const uint8_t def_vibtrem_speed_factor = 1;
const uint8_t def_vibtrem_table_size = 32;
const uint8_t def_vibtrem_table[256] = {
    0,  24,  49,  74,  97, 120, 141, 161, 180, 197, 212, 224, 235, 244, 250, 253,
  255, 253, 250, 244, 235, 224, 212, 197, 180, 161, 141, 120,  97,  74,  49,  24,
    0,  24,  49,  74,  97, 120, 141, 161, 180, 197, 212, 224, 235, 244, 250, 253,
  255, 253, 250, 244, 235, 224, 212, 197, 180, 161, 141, 120,  97,  74,  49,  24,
    0,  24,  49,  74,  97, 120, 141, 161, 180, 197, 212, 224, 235, 244, 250, 253,
  255, 253, 250, 244, 235, 224, 212, 197, 180, 161, 141, 120,  97,  74,  49,  24,
    0,  24,  49,  74,  97, 120, 141, 161, 180, 197, 212, 224, 235, 244, 250, 253,
  255, 253, 250, 244, 235, 224, 212, 197, 180, 161, 141, 120,  97,  74,  49,  24,
    0,  24,  49,  74,  97, 120, 141, 161, 180, 197, 212, 224, 235, 244, 250, 253,
  255, 253, 250, 244, 235, 224, 212, 197, 180, 161, 141, 120,  97,  74,  49,  24,
    0,  24,  49,  74,  97, 120, 141, 161, 180, 197, 212, 224, 235, 244, 250, 253,
  255, 253, 250, 244, 235, 224, 212, 197, 180, 161, 141, 120,  97,  74,  49,  24,
    0,  24,  49,  74,  97, 120, 141, 161, 180, 197, 212, 224, 235, 244, 250, 253,
  255, 253, 250, 244, 235, 224, 212, 197, 180, 161, 141, 120,  97,  74,  49,  24,
    0,  24,  49,  74,  97, 120, 141, 161, 180, 197, 212, 224, 235, 244, 250, 253,
  255, 253, 250, 244, 235, 224, 212, 197, 180, 161, 141, 120,  97,  74,  49,  24
};

uint8_t vibtrem_speed_factor;
uint8_t vibtrem_table_size;
uint8_t vibtrem_table[256];

tVIBRATO_TREMOLO_TABLE vibr_table;
tVIBRATO_TREMOLO_TABLE vibr_table2;
tVIBRATO_TREMOLO_TABLE trem_table;
tVIBRATO_TREMOLO_TABLE trem_table2;

void (*macro_preview_indic_proc) (uint8_t state) = NULL;

int32_t seconds_counter = 0;
int32_t hundereds_counter = 0;
bool    really_no_status_refresh = false;

tFM_PARAMETER_TABLE
           fmpar_table  [20];
bool       volume_lock  [20];
bool       vol4op_lock  [20];
uint16_t   volume_table [20];
uint16_t   vscale_table [20];
bool       peak_lock    [20];
bool       pan_lock     [20];
uint8_t    modulator_vol[20];
uint8_t    carrier_vol  [20];
tDECAY_BAR decay_bar    [20];
tVOLUM_BAR volum_bar    [20];
bool       channel_flag [20];
tCHUNK     event_table  [20];
uint8_t    voice_table  [20];
uint16_t   freq_table   [20];
uint16_t   zero_fq_table[20];
uint16_t   effect_table [20];
uint16_t   effect_table2[20];
uint8_t    fslide_table [20];
uint8_t    fslide_table2[20];
uint16_t   glfsld_table [20];
uint16_t   glfsld_table2[20];
struct porta_table_t
           porta_table  [20],
           porta_table2 [20];
bool       portaFK_table[20];
struct arpgg_table_t
           arpgg_table  [20],
           arpgg_table2 [20];
uint8_t    retrig_table [20];
uint8_t    retrig_table2[20];
struct tremor_table_t
           tremor_table [20],
           tremor_table2[20];
uint8_t    panning_table[20];
uint16_t   last_effect  [20];
uint16_t   last_effect2 [20];
uint8_t    volslide_type[20];
bool       event_new    [20];
uint16_t   freqtable2   [20];
uint8_t    notedel_table[20];
uint8_t    notecut_table[20];
int8_t     ftune_table  [20];
bool       keyoff_loop  [20];
struct macro_table_t
           macro_table  [20];
uint8_t    loopbck_table[20];
uint8_t    loop_table   [20][256];

uint8_t misc_register;
uint8_t ai_table[255];

uint8_t  overall_volume = 63;
uint8_t  global_volume = 63;
uint8_t  fade_out_volume = 63;
int32_t  playback_speed_shift = 0;
tPLAY_STATUS play_status = isStopped;
uint8_t  chan_pos = 1;
uint8_t  chpos = 1;
uint8_t  transpos = 1;
uint8_t  track_chan_start = 1;
uint8_t  nm_track_chan = 1;
uint16_t play_pos_buf[9] = { 0, };
uint8_t  rec_correction = 0;

uint8_t current_order = 0;
uint8_t current_pattern = 0;
uint8_t current_line = 0;
uint8_t current_tremolo_depth = 0;
uint8_t current_vibrato_depth = 0;
uint8_t current_inst = 1;
uint8_t current_octave = 4;

String adt2_title[37][18+1];

String songdata_source[255+1];
String instdata_source[255+1];
String songdata_title [255+1];

int32_t songdata_crc, songdata_crc_ord;
tADTRACK2_INS temp_instrument;
tADTRACK2_INS temp_instrument2;
tREGISTER_TABLE temp_instrument_macro;
tREGISTER_TABLE temp_instrument_macro2;
tDIS_FMREG_COL temp_instrument_dis_fmreg_col;
tDIS_FMREG_COL temp_instrument_dis_fmreg_col2;
uint8_t temp_ins_type;
uint8_t pattord_page;
uint8_t pattord_hpos;
uint8_t pattord_vpos;
uint8_t instrum_page;
uint8_t pattern_patt;
uint8_t pattern_page;
uint8_t pattern_hpos;
bool    limit_exceeded;
uint8_t load_flag, load_flag_alt;
bool    reset_chan      [20];
bool    reset_adsrw     [20];
bool    ignore_note_once[20];
bool    track_notes_ins;
bool    seek_pattern_break;

bool    speed_update, lockvol, panlock, lockVP;
uint8_t tremolo_depth, vibrato_depth;
bool    volume_scaling, percussion_mode;
uint8_t last_order;

tGENERIC_IO_BUFFER buf1;
tGENERIC_IO_BUFFER buf2;
uint8_t buf3[0x10000];
uint8_t buf4[0x10000];

tPATTERN_DATA *pattdata = NULL;

tOLD_VARIABLE_DATA1 old_hash_buffer;
tOLD_VARIABLE_DATA2 hash_buffer;
tOLD_FIXED_SONGDATA old_songdata;
uint16_t dos_memavail;

tFIXED_SONGDATA songdata;
tFIXED_SONGDATA songdata_bak;
tFIXED_SONGDATA temp_songdata;
tCLIPBOARD      clipboard;
/*
void *ptr_songdata      = &songdata;
void *ptr_songdata_bak  = &songdata_bak;
void *ptr_temp_songdata = &temp_songdata;
void *ptr_clipboard     = &clipboard;
*/
int32_t song_timer, timer_temp;
int32_t song_timer_tenths;
int32_t ticks, tick0, tickD, tickXF;
double  time_playing, time_playing0;

#if GO32
int32_t  timer_determinator = 1;
int32_t  timer_det2 = 1;
uint16_t scr_scroll_x = 0;
uint16_t old_scr_scroll_x = 0;
uint16_t scr_scroll_y = 0;
uint16_t old_scr_scroll_y = 0;
#else // !GO32
uint16_t screen_scroll_offset = 0;
#endif // !GO32

uint8_t  common_flag_backup;
bool     volume_scaling_backup;
tCHUNK   event_table_backup  [20];
uint16_t freq_table_backup   [20];
uint16_t freqtable2_backup   [20];
bool     keyoff_loop_backup  [20];
bool     channel_flag_backup [20];
tFM_PARAMETER_TABLE
         fmpar_table_backup  [20];
uint16_t volume_table_backup [20];
bool     pan_lock_backup     [20];
bool     volume_lock_backup  [20];
bool     peak_lock_backup    [20];
uint8_t  panning_table_backup[20];
uint8_t  voice_table_backup  [20];
uint8_t  flag_4op_backup;
struct status_backup_t status_backup;

#define FreqStart 0x156
#define FreqEnd   0x2AE
#define FreqRange (FreqEnd - FreqStart)

#include "adt2unit/nFreq.c"
#include "adt2unit/calc_freq_shift_up.c" // HINT: static
#include "adt2unit/calc_freq_shift_down.c" // HINT: static
#include "adt2unit/calc_vibtrem_shift.c" // HINT: static
#include "adt2unit/is_data_empty.c" // HINT: static
#include "adt2unit/change_freq.c"
#include "adt2unit/synchronize_song_timer.c" // HINT: static
#include "adt2unit/concw.c"
#include "adt2unit/ins_parameter.c"
#include "adt2unit/is_chan_adsr_data_empty.c"
#include "adt2unit/is_ins_adsr_data_empty.c"

#include "adt2unit/is_4op_chan.c"

#include "realtime.c"

#include "adt2unit/min.c"
#include "adt2unit/max.c"
#include "adt2unit/change_frequency.c"
#include "adt2unit/update_timer.c"
#include "adt2unit/update_playback_speed.c" // HINT: static
#include "adt2unit/key_on.c"
#include "adt2unit/key_off.c"
#include "adt2unit/release_sustaining_sound.c"
#include "adt2unit/scale_volume.c"
#include "adt2unit/_4op_data_flag.c" // HINT: static
#include "adt2unit/_4op_vol_valid_chan.c" // HINT: static
#include "adt2unit/set_ins_volume.c"
#include "adt2unit/set_volume.c" // HINT: static
#include "adt2unit/set_ins_volume_4op.c" // HINT: static
#include "adt2unit/reset_ins_volume.c" // HINT: static
#include "adt2unit/set_ins_data.c"
#include "adt2unit/update_modulator_adsrw.c"
#include "adt2unit/update_carrier_adsrw.c"

uint8_t block_xstart = 1;
uint8_t block_ystart = 0;

uint8_t block_x0 = 0;
uint8_t block_y0 = 1;
uint8_t block_x1 = 0;
uint8_t block_y1 = 1;

int32_t ticklooper = 0;
int32_t macro_ticklooper = 0;

int32_t bank_position_list_size = 0;
struct bank_position_list_t bank_position_list[MAX_NUM_BANK_POSITIONS];

#include "adt2unit/_macro_speedup.c"

#if GO32

static const char ___ADT2UNIT_CONST_END___ = 0;
static char       ___ADT2UNIT_DATA_END___ = 0;
static char       ___ADT2UNIT_BSS_END___;
static __NO_REORDER __ALIGNED_(1) __NAKED_RELAXED void
                  ___ADT2UNIT_CODE_END___ (void) { }

/*static*/ extern void timer_poll_proc (void); // TODO: port to C
static void (*timer_poll_proc_ptr) (void) = NULL;

#include "adt2unit/go32/TimerSetup.c"
#include "adt2unit/go32/TimerDone.c" // HINT: static
#include "adt2unit/go32/TimerInstallHandler.c" // HINT: static
#include "adt2unit/go32/TimerRemoveHandler.c" // HINT: static
#include "adt2unit/go32/init_adt2unit.c"

#else // !GO32

void (*timer_handler) (void) = NULL;
SDL_TimerID TimerID = NULL;
int32_t _interval = 1000 / 50; // 1000 ms / Hz

/*static*/ extern void timer_poll_proc (void); // TODO: port to C

#include "adt2unit/sdl/TimerSetup.c"
#include "adt2unit/sdl/TimerDone.c" // HINT: static
#include "adt2unit/sdl/TimerInstallHandler.c" // HINT: static
#include "adt2unit/sdl/TimerRemoveHandler.c" // HINT: static

#endif // !GO32

#include "adt2unit/init_timer_proc.c"
#include "adt2unit/done_timer_proc.c"

//#include "realtime.c"

#include "adt2unit/calc_pattern_pos.c"
