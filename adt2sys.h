// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef ADT2SYS_H
#define ADT2SYS_H

#include "defines.h"

#include <stdbool.h>
#include <stdint.h>
#include "pascal.h"

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

extern int32_t  virtual_screen__first_row;
extern uint16_t virtual_cur_shape;
extern uint16_t virtual_cur_pos;
extern int32_t  slide_ticks;
extern bool     reset_slide_ticks;
#if GO32
extern int32_t  gfx_ticks;
extern bool     reset_gfx_ticks;
extern float    scroll_ticks;
extern bool     mouse_active;
#endif // GO32
#if !GO32
extern bool     _WAVREC_blink_flag;
extern int32_t  _WAVREC_blink_ticks;
#endif // !GO32
extern bool     _NRECM_blink_flag;
extern int32_t  _NRECM_blink_ticks;
extern bool     _IRQ_freq_shift_reset_flag;
extern bool     _IRQFREQ_update_event;
extern bool     _IRQFREQ_blink_flag;
extern int32_t  _IRQFREQ_blink_ticks;
extern bool     blink_flag;
extern int32_t  blink_ticks;
extern bool     cursor_sync;
extern void   (*_show_bpm_realtime_proc) (void);
#if GO32
extern bool     _draw_screen_without_vsync;
extern bool     _draw_screen_without_delay;
#else // !GO32
extern bool     _draw_screen_without_delay;
extern bool     _update_sdl_screen;
extern int8_t   _name_scrl_shift_ctr;
extern uint8_t  _name_scrl_shift;
extern int32_t  _name_scrl_pending_frames;
#endif // !GO32
extern int32_t  _cursor_blink_factor;
extern int32_t  _cursor_blink_pending_frames;
extern bool     _realtime_gfx_no_update;
#if GO32
extern int32_t  _screen_refresh_pending_frames;
#pragma pack(push, 1)
extern struct _custom_svga_cfg_rec_t {
  bool flag;
  int32_t value;
} _custom_svga_cfg[31]; // HINT: (FPC) start index 1
#pragma pack(pop)
#endif // GO32

extern bool    _force_program_quit;
extern uint8_t _traceprc_last_order;
extern uint8_t _traceprc_last_pattern;
extern uint8_t _traceprc_last_line;
extern uint8_t _pattedit_lastpos;

extern void draw_screen (void);

#if !GO32

extern void   *_FrameBuffer;

void vid_FadeOut (void);

#else // GO32

extern uint8_t _FrameBuffer_mirror[1024*768]; // HINT: (FPC) start index 0

#endif // GO32

#endif // !defined(ADT2SYS_H)
