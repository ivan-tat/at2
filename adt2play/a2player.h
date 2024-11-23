// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef A2PLAYER_H
#define A2PLAYER_H

#include "defines.h"
#include <stdint.h>

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

#define MAX_IRQ_FREQ 1000
#define MIN_IRQ_FREQ 50

extern void (*timer_poll_proc_ptr) (void);
extern bool  timer_initialized;

#include "typcons1.h"

extern int16_t  error_code;
extern uint8_t  current_order;
extern uint8_t  current_pattern;
extern uint8_t  current_line;
extern uint8_t  tempo;
extern uint8_t  speed;
extern uint16_t macro_speedup;
extern uint8_t  max_patterns;
extern bool     fast_forward;
extern uint8_t  overall_volume;
extern uint8_t  global_volume;
extern uint8_t  fade_out_volume;

extern uint16_t song_timer;
extern uint16_t song_timer_tenths;
extern int32_t  ticklooper;
extern int32_t  macro_ticklooper;
extern int32_t  timer_ticklooper;
extern int32_t  timer_200hz_counter;
extern int32_t  timer_50hz_counter;
extern int32_t  timer_20hz_counter;
extern bool     timer_200hz_flag;
extern bool     timer_50hz_flag;
extern bool     timer_20hz_flag;

extern uint16_t timer_temp;
extern int32_t  ticks, tick0, tickD;
extern int32_t  tickXF;
extern bool     limit_exceeded;

extern double time_playing;
extern tPATTERN_DATA *pattdata;
extern tFIXED_SONGDATA songdata;
extern tOLD_FIXED_SONGDATA old_songdata;
extern tOLD_VARIABLE_DATA1 old_hash_buffer;
extern tOLD_VARIABLE_DATA2 hash_buffer;

extern uint16_t IRQ_freq;
extern int16_t  IRQ_freq_shift;
extern int32_t  playback_speed_shift;
extern bool     timer_fix;
extern bool     pattern_break;
extern bool     pattern_break_loop;
extern bool     pattern_break_docmd;
extern uint8_t  pattern_break_oldord;
extern bool     pattern_delay;
extern uint8_t  next_line;
extern tPLAY_STATUS play_status;
extern bool     replay_forbidden;
extern bool     force_macro_keyon;

// TODO: rename `decay_bar' into `spectrum_bar'

#define DECAY_BARS 25 // to map playing note into the range 1..DECAY_BARS

extern tDECAY_BAR decay_bar[DECAY_BARS]; // HINT: (FPC) start index 1

void     start_playing (void);
void     set_overall_volume (uint8_t level);
void     stop_playing (void);
void     init_old_songdata (void);
void     init_songdata (void);
void     init_timer_proc (void);
void     done_timer_proc (void);
void     get_chunk (uint8_t pattern, uint8_t line, uint8_t chan, tCHUNK *chunk);
void     put_chunk (uint8_t pattern, uint8_t line, uint8_t chan, const tCHUNK *chunk);
void     count_order (uint8_t *entries);
int16_t  calc_following_order (uint8_t order);
bool     is_4op_chan (uint8_t chan);
int32_t  min (int32_t value, int32_t minimum);
int32_t  max (int32_t value, int32_t maximum);

// adt2opl3 {
extern uint16_t opl3port;

typedef void tOPLOUT_proc (uint16_t reg, uint16_t data);

void opl2out (uint16_t reg, uint16_t data);
void opl3exp (uint16_t data);

extern tOPLOUT_proc *opl3out;
// } adt2opl3

#if GO32
void init_a2player (void);
#endif // GO32

#pragma pack(pop)

#endif // !defined(A2PLAYER_H)
