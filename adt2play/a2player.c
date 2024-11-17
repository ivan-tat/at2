// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdlib.h>
#include <math.h>
#include "common.h"
#include "debug.h"
#if GO32
#include "go32/adt2dpmi.h"
#include "go32/iss_tim.h"
#endif // !GO32
#include "go32/PIT/PIT_consts.h"
#include "a2player.h"
// adt2opl3 {
#include <pc.h>
// } adt2opl3

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

#if GO32
#define __PAREA A2PLAYER
__PAREA_START (CONST)
__PAREA_START (DATA)
__PAREA_START (BSS)
__PAREA_START (CODE)
#endif // !GO32

void (*timer_poll_proc_ptr) (void) = NULL;
bool  timer_initialized = false;

#include "typcons1.c"

int16_t  error_code = 0;
uint8_t  current_order = 0;
uint8_t  current_pattern = 0;
uint8_t  current_line = 0;
uint8_t  tempo = 50;
uint8_t  speed = 6;
uint16_t macro_speedup = 1;
uint8_t  max_patterns = 128;
bool     fast_forward = false;
uint8_t  overall_volume = 63;
uint8_t  global_volume = 63;
uint8_t  fade_out_volume = 63;

uint16_t song_timer = 0;
uint16_t song_timer_tenths = 0;
int32_t  ticklooper = 0;
int32_t  macro_ticklooper = 0;
int32_t  timer_ticklooper = 0;
int32_t  timer_200hz_counter = 0;
int32_t  timer_50hz_counter = 0;
int32_t  timer_20hz_counter = 0;
bool     timer_200hz_flag = false;
bool     timer_50hz_flag = false;
bool     timer_20hz_flag = false;

uint16_t timer_temp;
int32_t  ticks, tick0, tickD;
int32_t  tickXF;
bool     limit_exceeded;

double time_playing;
tPATTERN_DATA *pattdata;
tFIXED_SONGDATA songdata;
tOLD_FIXED_SONGDATA old_songdata;
tOLD_VARIABLE_DATA1 old_hash_buffer;
tOLD_VARIABLE_DATA2 hash_buffer;

uint16_t IRQ_freq = 50;
int16_t  IRQ_freq_shift = 0;
int32_t  playback_speed_shift = 0;
bool     timer_fix = true;
bool     pattern_break = false;
bool     pattern_break_loop = false;
bool     pattern_break_docmd = false;
uint8_t  pattern_break_oldord = BYTE_NULL;
bool     pattern_delay = false;
uint8_t  next_line = 0;
tPLAY_STATUS play_status = isStopped;
bool     replay_forbidden = true;
bool     force_macro_keyon = false;

tDECAY_BAR decay_bar[DECAY_BARS]; // HINT: (FPC) start index 1

// adt2opl3 {
uint16_t opl3port = 0x388;
/*st*/ uint16_t _opl_regs_cache[0x10000]; // HINT: (FPC) start index 0

#include "adt2opl3/go32/opl2out.c"
#include "adt2opl3/go32/opl3out_proc.c" // static
#include "adt2opl3/go32/opl3exp.c"

tOPLOUT_proc *opl3out = opl3out_proc;
// } adt2opl3

/* Static data */

#define keyoff_flag        0x80
#define fixed_note_flag    0x90
#define pattern_loop_flag  0xE0
#define pattern_break_flag 0xF0

#define MACRO_NOTE_RETRIG_FLAG      0x80
#define MACRO_ENVELOPE_RESTART_FLAG 0x40
#define MACRO_ZERO_FREQ_FLAG        0x20

/*st*/ const uint8_t def_vibtrem_speed_factor = 1;
/*st*/ const uint8_t def_vibtrem_table_size = 32;
/*st*/ const uint8_t def_vibtrem_table[256] = { // HINT: (FPC) start index 0
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

/*st*/ uint8_t vibtrem_speed_factor;
/*st*/ uint8_t vibtrem_table_size;
/*st*/ uint8_t vibtrem_table[256]; // HINT: (FPC) start index 0

typedef struct {
  uint8_t pos;
  uint8_t dir;
  uint8_t speed;
  uint8_t depth;
  bool fine;
} tVIBRATO_TREMOLO_TABLE[20]; // HINT: (FPC) start index 1

/*st*/ tVIBRATO_TREMOLO_TABLE vibr_table;
/*st*/ tVIBRATO_TREMOLO_TABLE vibr_table2;
/*st*/ tVIBRATO_TREMOLO_TABLE trem_table;
/*st*/ tVIBRATO_TREMOLO_TABLE trem_table2;

/*st*/ tFM_PARAMETER_TABLE
                fmpar_table   [20]; // HINT: (FPC) start index 1
/*st*/ bool     volume_lock   [20]; // HINT: (FPC) start index 1
/*st*/ bool     vol4op_lock   [20]; // HINT: (FPC) start index 1
/*st*/ uint16_t volume_table  [20]; // HINT: (FPC) start index 1
/*st*/ uint16_t vscale_table  [20]; // HINT: (FPC) start index 1
/*st*/ bool     peak_lock     [20]; // HINT: (FPC) start index 1
/*st*/ bool     pan_lock      [20]; // HINT: (FPC) start index 1
/*st*/ tCHUNK   event_table   [20]; // HINT: (FPC) start index 1
/*st*/ uint8_t  voice_table   [20]; // HINT: (FPC) start index 1
/*st*/ uint8_t  modulator_vol [20]; // HINT: (FPC) start index 1
/*st*/ uint8_t  carrier_vol   [20]; // HINT: (FPC) start index 1
/*st*/ uint16_t freq_table    [20]; // HINT: (FPC) start index 1
/*st*/ uint16_t zero_fq_table [20]; // HINT: (FPC) start index 1
/*st*/ uint16_t effect_table  [20]; // HINT: (FPC) start index 1
/*st*/ uint16_t effect_table2 [20]; // HINT: (FPC) start index 1
/*st*/ uint8_t  fslide_table  [20]; // HINT: (FPC) start index 1
/*st*/ uint8_t  fslide_table2 [20]; // HINT: (FPC) start index 1
/*st*/ uint16_t glfsld_table  [20]; // HINT: (FPC) start index 1
/*st*/ uint16_t glfsld_table2 [20]; // HINT: (FPC) start index 1
/*st*/ struct { uint16_t freq; uint8_t speed; }
                porta_table   [20], // HINT: (FPC) start index 1
                porta_table2  [20]; // HINT: (FPC) start index 1
/*st*/ bool     portaFK_table [20]; // HINT: (FPC) start index 1
/*st*/ struct { uint8_t state, note, add1, add2; }
                arpgg_table   [20], // HINT: (FPC) start index 1
                arpgg_table2  [20]; // HINT: (FPC) start index 1
/*st*/ uint8_t  retrig_table  [20]; // HINT: (FPC) start index 1
/*st*/ uint8_t  retrig_table2 [20]; // HINT: (FPC) start index 1
/*st*/ struct { int16_t pos; uint16_t volume; }
                tremor_table  [20], // HINT: (FPC) start index 1
                tremor_table2 [20]; // HINT: (FPC) start index 1
/*st*/ uint8_t  panning_table [20]; // HINT: (FPC) start index 1
/*st*/ uint16_t last_effect   [20]; // HINT: (FPC) start index 1
/*st*/ uint16_t last_effect2  [20]; // HINT: (FPC) start index 1
/*st*/ uint8_t  volslide_type [20]; // HINT: (FPC) start index 1
/*st*/ bool     event_new     [20]; // HINT: (FPC) start index 1
/*st*/ uint8_t  notedel_table [20]; // HINT: (FPC) start index 1
/*st*/ uint8_t  notecut_table [20]; // HINT: (FPC) start index 1
/*st*/ int8_t   ftune_table   [20]; // HINT: (FPC) start index 1
/*st*/ bool     keyoff_loop   [20]; // HINT: (FPC) start index 1
/*st*/ struct {
  uint16_t fmreg_pos, arpg_pos, vib_pos;
  uint8_t  fmreg_count, fmreg_duration, arpg_count,
           vib_count, vib_delay;
  bool     vib_paused;
  uint8_t  fmreg_table, arpg_table, vib_table;
  uint8_t  arpg_note;
  uint16_t vib_freq;
}               macro_table   [20]; // HINT: (FPC) start index 1

/*st*/ uint8_t  loopbck_table [20]; // HINT: (FPC) start index 1
/*st*/ uint8_t  loop_table    [20][256]; // HINT: (FPC) start index 1,0
/*st*/ uint8_t  misc_register;

/*st*/ uint8_t  current_tremolo_depth = 0;
/*st*/ uint8_t  current_vibrato_depth = 0;

/*st*/ bool     speed_update, lockvol, panlock, lockVP;
/*st*/ uint8_t  tremolo_depth, vibrato_depth;
/*st*/ bool     volume_scaling, percussion_mode;
/*st*/ uint8_t  last_order;
/*st*/ bool     reset_chan[20]; // HINT: (FPC) start index 1

#define FreqStart 0x156
#define FreqEnd   0x2AE
#define FreqRange (FreqEnd - FreqStart)

#include "adt2unit/nFreq.c"
#include "adt2unit/calc_freq_shift_up.c" // HINT: static
#include "adt2unit/calc_freq_shift_down.c" // HINT: static
#include "adt2unit/calc_vibtrem_shift.c" // HINT: static
#include "adt2unit/change_freq.c"
#include "adt2unit/ins_parameter.c"
#include "adt2unit/is_chan_adsr_data_empty.c"
#include "adt2unit/is_ins_adsr_data_empty.c"
#include "adt2unit/is_data_empty.c" // HINT: static
#include "adt2unit/min.c"
#include "adt2unit/max.c"
//#include "adt2unit/concw.c"
#include "adt2unit/synchronize_song_timer.c" // HINT: static
#include "adt2unit/change_frequency.c"
#include "adt2unit/_macro_speedup.c"
void TimerSetup (uint32_t Hz); // forward
#include "adt2unit/update_timer.c"
#include "adt2unit/update_playback_speed.c" // HINT: static
#include "adt2unit/key_on.c"
#include "adt2unit/key_off.c"
#include "adt2unit/release_sustaining_sound.c"
#include "adt2unit/scale_volume.c"
#include "adt2unit/_4op_data_flag.c" // HINT: static
#include "adt2unit/_4op_vol_valid_chan.c" // HINT: static
#include "adt2unit/set_ins_volume.c"
#include "adt2unit/set_volume.c" // static (used in `set_ins_volume_4op')
#include "adt2unit/set_ins_volume_4op.c" // HINT: static
#include "adt2unit/reset_ins_volume.c" // HINT: static
#include "adt2unit/set_global_volume.c"
#include "adt2unit/set_overall_volume.c"
#include "adt2unit/init_macro_table.c"
#include "adt2unit/set_ins_data.c"
#include "adt2unit/update_modulator_adsrw.c"
#include "adt2unit/update_carrier_adsrw.c"
#include "adt2unit/update_fmpar.c"
#include "adt2unit/is_4op_chan.c"
#include "adt2unit/output_note.c"
#include "adt2unit/generate_custom_vibrato.c" // HINT: static
//procedure update_fine_effects(chan: Byte); forward;
//procedure play_line;
#include "adt2unit/portamento_up.c" // HINT: static
#include "adt2unit/portamento_down.c" // HINT: static
#include "adt2unit/macro_vibrato__porta_up.c" // HINT: static
#include "adt2unit/macro_vibrato__porta_down.c" // HINT: static
#include "adt2unit/tone_portamento.c" // HINT: static
#include "adt2unit/tone_portamento2.c" // HINT: static
#include "adt2unit/slide_carrier_volume_up.c" // static, used in `slide_volume_up'
#include "adt2unit/slide_modulator_volume_up.c" // static, used in `slide_volume_up'
#include "adt2unit/slide_volume_up.c" // HINT: static
#include "adt2unit/slide_carrier_volume_down.c" // static, used in `slide_volume_down'
#include "adt2unit/slide_modulator_volume_down.c" // static, used in `slide_volume_down'
#include "adt2unit/slide_volume_down.c" // HINT: static
#include "adt2unit/volume_slide.c" // HINT: static
#include "adt2unit/global_volume_slide.c" // HINT: static
#include "adt2unit/arpeggio.c" // HINT: static, defines `arpgg_state[]'
#include "adt2unit/arpeggio2.c" // HINT: static, uses `arpgg_state[]'
#include "adt2unit/vibrato.c" // HINT: static
#include "adt2unit/vibrato2.c" // HINT: static
#include "adt2unit/tremolo.c" // HINT: static
#include "adt2unit/tremolo2.c" // HINT: static
//procedure update_effects;
//procedure update_fine_effects(chan: Byte);
//procedure update_extra_fine_effects;
#include "adt2unit/calc_following_order.c"
#include "adt2unit/calc_order_jump.c"
//procedure update_song_position;
//procedure poll_proc;
//procedure macro_poll_proc;
//procedure timer_poll_proc;

#if GO32
__PAREA_END (CONST)
__PAREA_END (DATA)
__PAREA_END (BSS)
__PAREA_END (CODE)
#endif // GO32

#if GO32
#include "adt2unit/go32/TimerSetup.c"
#include "adt2unit/go32/TimerDone.c" // HINT: static
#endif // GO32
#include "adt2unit/init_timer_proc.c"
#include "adt2unit/done_timer_proc.c"
#include "adt2unit/calc_pattern_pos.c"
#include "adt2unit/init_buffers.c"
#include "adt2unit/init_player.c"
#include "adt2unit/stop_playing.c"
#include "adt2unit/init_old_songdata.c"
#include "adt2unit/init_songdata.c"
#include "adt2unit/start_playing.c"
#include "adt2unit/get_chunk.c"
#include "adt2unit/put_chunk.c"
#include "adt2unit/count_order.c"

#if GO32
#include "a2player/go32/init_a2player.c"
#endif // GO32

#pragma pack(pop)
