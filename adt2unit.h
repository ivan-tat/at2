// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef ADT2UNIT_H
#define ADT2UNIT_H

#include "defines.h"

#include <stdint.h>
#if USE_FPC
#include "pascal.h"
#endif // USE_FPC
#include "common.h"

#define MIN_IRQ_FREQ 50
#define MAX_IRQ_FREQ 1000

#include "typcons1.h"
#include "typcons2.h"

extern uint16_t IRQ_freq;
extern int16_t  IRQ_freq_shift;
extern uint8_t  tempo;
extern uint8_t  speed;
extern uint16_t macro_speedup;
extern bool     timer_initialized;
extern bool     repeat_pattern;
extern bool     fast_forward;
extern bool     _rewind;
extern bool     pattern_break;
extern bool     pattern_delay;
extern uint8_t  next_line;
extern uint8_t  start_order;
extern uint8_t  start_pattern;
extern uint8_t  start_line;
extern bool     replay_forbidden;
extern bool     single_play;
extern bool     calibrating;
extern bool     no_status_refresh;
extern bool     do_synchronize;
extern bool     space_pressed;
extern bool     module_archived;
extern bool     force_scrollbars;
extern bool     no_sync_playing;
extern bool     no_step_debugging;
extern bool     play_single_patt;
extern bool     no_trace_pattord;
extern bool     skip_macro_flag;
extern uint8_t  max_patterns;
extern bool     jump_mark_mode;
extern bool     force_macro_keyon;
extern bool     ins_trailing_flag;

extern const uint8_t def_vibtrem_speed_factor;
extern const uint8_t def_vibtrem_table_size;
extern const uint8_t def_vibtrem_table[256]; // HINT: (FPC) start index 0

extern uint8_t vibtrem_speed_factor;
extern uint8_t vibtrem_table_size;
extern uint8_t vibtrem_table[256]; // HINT: (FPC) start index 0

#pragma pack(push, 1)
typedef struct {
  uint8_t pos;
  uint8_t dir;
  uint8_t speed;
  uint8_t depth;
  bool fine;
} tVIBRATO_TREMOLO_TABLE[20]; // HINT: (FPC) start index 1
#pragma pack(pop)

extern tVIBRATO_TREMOLO_TABLE vibr_table;
extern tVIBRATO_TREMOLO_TABLE vibr_table2;
extern tVIBRATO_TREMOLO_TABLE trem_table;
extern tVIBRATO_TREMOLO_TABLE trem_table2;

extern void (*macro_preview_indic_proc) (uint8_t state);

extern int32_t seconds_counter;
extern int32_t hundereds_counter;
extern bool    really_no_status_refresh;

#define keyoff_flag        0x80
#define fixed_note_flag    0x90
#define pattern_loop_flag  0xE0
#define pattern_break_flag 0xF0

#pragma pack(push, 1)
extern tFM_PARAMETER_TABLE
                  fmpar_table  [20]; // HINT: (FPC) start index 1
extern bool       volume_lock  [20]; // HINT: (FPC) start index 1
extern bool       vol4op_lock  [20]; // HINT: (FPC) start index 1
extern uint16_t   volume_table [20]; // HINT: (FPC) start index 1
extern uint16_t   vscale_table [20]; // HINT: (FPC) start index 1
extern bool       peak_lock    [20]; // HINT: (FPC) start index 1
extern bool       pan_lock     [20]; // HINT: (FPC) start index 1
extern uint8_t    modulator_vol[20]; // HINT: (FPC) start index 1
extern uint8_t    carrier_vol  [20]; // HINT: (FPC) start index 1
extern tDECAY_BAR decay_bar    [20]; // HINT: (FPC) start index 1
extern tVOLUM_BAR volum_bar    [20]; // HINT: (FPC) start index 1
extern bool       channel_flag [20]; // HINT: (FPC) start index 1
extern tCHUNK     event_table  [20]; // HINT: (FPC) start index 1
extern uint8_t    voice_table  [20]; // HINT: (FPC) start index 1
extern uint16_t   freq_table   [20]; // HINT: (FPC) start index 1
extern uint16_t   zero_fq_table[20]; // HINT: (FPC) start index 1
extern uint16_t   effect_table [20]; // HINT: (FPC) start index 1
extern uint16_t   effect_table2[20]; // HINT: (FPC) start index 1
extern uint8_t    fslide_table [20]; // HINT: (FPC) start index 1
extern uint8_t    fslide_table2[20]; // HINT: (FPC) start index 1
extern uint16_t   glfsld_table [20]; // HINT: (FPC) start index 1
extern uint16_t   glfsld_table2[20]; // HINT: (FPC) start index 1
extern struct porta_table_t { uint16_t freq; uint8_t speed; }
                  porta_table  [20], // HINT: (FPC) start index 1
                  porta_table2 [20]; // HINT: (FPC) start index 1
extern bool       portaFK_table[20]; // HINT: (FPC) start index 1
extern struct arpgg_table_t { uint8_t state, note, add1, add2; }
                  arpgg_table  [20], // HINT: (FPC) start index 1
                  arpgg_table2 [20]; // HINT: (FPC) start index 1
extern uint8_t    retrig_table [20]; // HINT: (FPC) start index 1
extern uint8_t    retrig_table2[20]; // HINT: (FPC) start index 1
extern struct tremor_table_t { int16_t pos; uint16_t volume; }
                  tremor_table [20], // HINT: (FPC) start index 1
                  tremor_table2[20]; // HINT: (FPC) start index 1
extern uint8_t    panning_table[20]; // HINT: (FPC) start index 1
extern uint16_t   last_effect  [20]; // HINT: (FPC) start index 1
extern uint16_t   last_effect2 [20]; // HINT: (FPC) start index 1
extern uint8_t    volslide_type[20]; // HINT: (FPC) start index 1
extern bool       event_new    [20]; // HINT: (FPC) start index 1
extern uint16_t   freqtable2   [20]; // HINT: (FPC) start index 1
extern uint8_t    notedel_table[20]; // HINT: (FPC) start index 1
extern uint8_t    notecut_table[20]; // HINT: (FPC) start index 1
extern int8_t     ftune_table  [20]; // HINT: (FPC) start index 1
extern bool       keyoff_loop  [20]; // HINT: (FPC) start index 1
extern struct macro_table_t {
  uint16_t fmreg_pos, arpg_pos, vib_pos;
  uint8_t  fmreg_count, fmreg_duration, arpg_count;
  uint8_t  vib_count, vib_delay;
  bool     vib_paused;
  uint8_t  fmreg_table, arpg_table, vib_table;
  uint8_t  arpg_note;
  uint16_t vib_freq;
}                 macro_table  [20]; // HINT: (FPC) start index 1
#pragma pack(pop)
extern uint8_t    loopbck_table[20]; // HINT: (FPC) start index 1
extern uint8_t    loop_table   [20][256]; // HINT: (FPC) start index 1,0

extern uint8_t misc_register;
extern uint8_t ai_table[255]; // HINT: (FPC) start index 1

extern uint8_t  overall_volume;
extern uint8_t  global_volume;
extern uint8_t  fade_out_volume;
extern int32_t  playback_speed_shift;
extern tPLAY_STATUS play_status;
extern uint8_t  chan_pos;
extern uint8_t  chpos;
extern uint8_t  transpos;
extern uint8_t  track_chan_start;
extern uint8_t  nm_track_chan;
extern uint16_t play_pos_buf[9]; // HINT: (FPC) start index 1
extern uint8_t  rec_correction;

#define MACRO_NOTE_RETRIG_FLAG 0x80
#define MACRO_ENVELOPE_RESTART_FLAG 0x40
#define MACRO_ZERO_FREQ_FLAG 0x20

extern uint8_t current_order;
extern uint8_t current_pattern;
extern uint8_t current_line;
extern uint8_t current_tremolo_depth;
extern uint8_t current_vibrato_depth;
extern uint8_t current_inst;
extern uint8_t current_octave;

extern String adt2_title[37][18+1]; // HINT: (FPC) start index 0

extern String songdata_source[255+1]; // HINT: (FPC) length not set (default)
extern String instdata_source[255+1]; // HINT: (FPC) length not set (default)
extern String songdata_title [255+1]; // HINT: (FPC) length not set (default)

extern int32_t songdata_crc, songdata_crc_ord;
extern tADTRACK2_INS temp_instrument;
extern tADTRACK2_INS temp_instrument2;
extern tREGISTER_TABLE temp_instrument_macro;
extern tREGISTER_TABLE temp_instrument_macro2;
extern tDIS_FMREG_COL temp_instrument_dis_fmreg_col;
extern tDIS_FMREG_COL temp_instrument_dis_fmreg_col2;
extern uint8_t temp_ins_type;
extern uint8_t pattord_page;
extern uint8_t pattord_hpos;
extern uint8_t pattord_vpos;
extern uint8_t instrum_page;
extern uint8_t pattern_patt;
extern uint8_t pattern_page;
extern uint8_t pattern_hpos;
extern bool    limit_exceeded;
extern uint8_t load_flag, load_flag_alt;
extern bool    reset_chan      [20]; // HINT: (FPC) start index 1
extern bool    reset_adsrw     [20]; // HINT: (FPC) start index 1
extern bool    ignore_note_once[20]; // HINT: (FPC) start index 1
extern bool    track_notes_ins;
extern bool    seek_pattern_break;

extern bool    speed_update, lockvol, panlock, lockVP;
extern uint8_t tremolo_depth, vibrato_depth;
extern bool    volume_scaling, percussion_mode;
extern uint8_t last_order;

extern tGENERIC_IO_BUFFER buf1;
extern tGENERIC_IO_BUFFER buf2;
extern uint8_t buf3[0x10000];
extern uint8_t buf4[0x10000];

extern tPATTERN_DATA *pattdata;

extern tOLD_VARIABLE_DATA1 old_hash_buffer;
extern tOLD_VARIABLE_DATA2 hash_buffer;
extern tOLD_FIXED_SONGDATA old_songdata;
extern uint16_t dos_memavail;

extern tFIXED_SONGDATA songdata;
extern tFIXED_SONGDATA songdata_bak;
extern tFIXED_SONGDATA temp_songdata;
extern tCLIPBOARD      clipboard;
/*
extern void *ptr_songdata;
extern void *ptr_songdata_bak;
extern void *ptr_temp_songdata;
extern void *ptr_clipboard;
*/
extern int32_t song_timer, timer_temp;
extern int32_t song_timer_tenths;
extern int32_t ticks, tick0, tickD, tickXF;
extern double  time_playing, time_playing0;

#if GO32
extern int32_t  timer_determinator;
extern int32_t  timer_det2;
extern uint16_t scr_scroll_x;
extern uint16_t old_scr_scroll_x;
extern uint16_t scr_scroll_y;
extern uint16_t old_scr_scroll_y;
#else // !GO32
extern uint16_t screen_scroll_offset;
#endif // !GO32

extern uint8_t  common_flag_backup;
extern bool     volume_scaling_backup;
extern tCHUNK   event_table_backup  [20]; // HINT: (FPC) start index 1
extern uint16_t freq_table_backup   [20]; // HINT: (FPC) start index 1
extern uint16_t freqtable2_backup   [20]; // HINT: (FPC) start index 1
extern bool     keyoff_loop_backup  [20]; // HINT: (FPC) start index 1
extern bool     channel_flag_backup [20]; // HINT: (FPC) start index 1
extern tFM_PARAMETER_TABLE
                fmpar_table_backup  [20]; // HINT: (FPC) start index 1
extern uint16_t volume_table_backup [20]; // HINT: (FPC) start index 1
extern bool     pan_lock_backup     [20]; // HINT: (FPC) start index 1
extern bool     volume_lock_backup  [20]; // HINT: (FPC) start index 1
extern bool     peak_lock_backup    [20]; // HINT: (FPC) start index 1
extern uint8_t  panning_table_backup[20]; // HINT: (FPC) start index 1
extern uint8_t  voice_table_backup  [20]; // HINT: (FPC) start index 1
extern uint8_t  flag_4op_backup;
#pragma pack(push, 1)
extern struct status_backup_t {
  bool replay_forbidden;
  tPLAY_STATUS play_status;
} status_backup;
#pragma pack(pop)

uint16_t nFreq (uint8_t note);
void     change_freq (uint8_t chan, uint16_t freq);
uint8_t  calc_pattern_pos (uint8_t pattern);
uint16_t concw (uint8_t lo, uint8_t hi);
uint8_t  ins_parameter (uint8_t ins, uint8_t param);
bool     is_chan_adsr_data_empty (uint8_t chan);
bool     is_ins_adsr_data_empty (uint8_t ins);
uint8_t  scale_volume (uint8_t volume, uint8_t scale_factor);
uint16_t _macro_speedup (void);
//procedure calibrate_player(order,line: Byte; status_filter: Boolean; line_dependent: Boolean);
void update_timer (uint16_t Hz);
void key_on (uint8_t chan);
void key_off (uint8_t chan);
void release_sustaining_sound (uint8_t chan);
void init_macro_table (uint8_t chan, uint8_t note, uint8_t ins, uint16_t freq);
void set_ins_volume (uint8_t modulator, uint8_t carrier, uint8_t chan);
void update_modulator_adsrw (uint8_t chan);
void update_carrier_adsrw (uint8_t chan);
//procedure update_fmpar(chan: Byte);
//procedure reset_chan_data(chan: Byte);
void poll_proc (void);
void macro_poll_proc (void);
//procedure init_buffers;
//procedure init_player;
//procedure reset_player;
//procedure start_playing;
//procedure stop_playing;
//procedure update_song_position;
void change_frequency (uint8_t chan, uint16_t freq);
//procedure set_global_volume;
void set_ins_data (uint8_t ins, uint8_t chan);
void init_timer_proc (void);
void done_timer_proc (void);
//procedure realtime_gfx_poll_proc; cdecl;
//procedure decay_bars_refresh; cdecl;
//procedure status_refresh; cdecl;
//procedure trace_update_proc;
//
//function  hscroll_bar(x,y: Byte; size: Byte; len1,len2,pos: Word;
//                      atr1,atr2: Byte): Byte;
//function  vscroll_bar(x,y: Byte; size: Byte; len1,len2,pos: Word;
//                      atr1,atr2: Byte): Byte;
//
//procedure centered_frame(var xstart,ystart: Byte; hsize,vsize: Byte;
//                         name: String; atr1,atr2: Byte; border: String);
//
void get_chunk (uint8_t pattern, uint8_t line, uint8_t channel, tCHUNK *chunk);
void put_chunk (uint8_t pattern, uint8_t line, uint8_t channel, const tCHUNK *chunk);
//
//function  get_chanpos(var data; channels,scancode: Byte): Byte;
//function  get_chanpos2(var data; channels,scancode: Byte): Byte;
//function  count_channel(hpos: Byte): Byte;
//function  count_pos(hpos: Byte): Byte;
//function  calc_max_speedup(tempo: Byte): Word;
//function  calc_bpm_speed(tempo,speed,rows_per_beat: Byte): Real;
//function  calc_realtime_bpm_speed(tempo,speed,rows_per_beat: Byte): Real;
//function  calc_order_jump: Integer;
int16_t calc_following_order (uint8_t order);
bool    is_4op_chan (uint8_t chan);

#include "realtime.h"

int32_t min (int32_t value, int32_t minimum);
int32_t max (int32_t value, int32_t maximum);
void    TimerSetup (uint32_t Hz);

extern uint8_t block_xstart;
extern uint8_t block_ystart;

extern uint8_t block_x0;
extern uint8_t block_y0;
extern uint8_t block_x1;
extern uint8_t block_y1;

extern int32_t ticklooper;
extern int32_t macro_ticklooper;

#if GO32
#define MAX_NUM_BANK_POSITIONS 1000
#else // !GO32
#define MAX_NUM_BANK_POSITIONS 500
#endif // !GO32

extern int32_t bank_position_list_size;
#pragma pack(push, 1)
extern struct bank_position_list_t {
#if GO32
  String bank_name[50+1];
#else // !GO32
  String bank_name[255+1]; // HINT: (FPC) length not set (default)
#endif // !GO32
  int32_t bank_size;
  int32_t bank_position;
} bank_position_list[MAX_NUM_BANK_POSITIONS]; // HINT: (FPC) start index 1
#pragma pack(pop)

#if GO32
void init_adt2unit (void);
#endif // GO32

void realtime_gfx_poll_proc (void);

#endif // !defined(ADT2UNIT_H)
