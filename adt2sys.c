// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"

#include <stddef.h>
#include <string.h>
#include "debug.h"
#if GO32
#include "go32/adt2dpmi.h"
#include "go32/adt2vesa.h"
#include "txtscrio.h"
#else // !GO32
#endif // !GO32
#include "adt2sys.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

int32_t  virtual_screen__first_row = 0;
uint16_t virtual_cur_shape = 0;
uint16_t virtual_cur_pos = 0;
int32_t  slide_ticks = 0;
bool     reset_slide_ticks = false;
#if GO32
int32_t  gfx_ticks = 0;
bool     reset_gfx_ticks = false;
float    scroll_ticks = 0.0f;
bool     mouse_active = false;
#endif // GO32
#if !GO32
bool     _WAVREC_blink_flag = false;
int32_t  _WAVREC_blink_ticks = 0;
#endif // !GO32
bool     _NRECM_blink_flag = false;
int32_t  _NRECM_blink_ticks = 0;
bool     _IRQ_freq_shift_reset_flag = false;
bool     _IRQFREQ_update_event = false;
bool     _IRQFREQ_blink_flag = false;
int32_t  _IRQFREQ_blink_ticks = 0;
bool     blink_flag = false;
int32_t  blink_ticks = 0;
bool     cursor_sync = false;
void   (*_show_bpm_realtime_proc) (void) = NULL;
#if GO32
bool     _draw_screen_without_vsync = false;
bool     _draw_screen_without_delay = false;
#else // !GO32
bool     _draw_screen_without_delay = false;
bool     _update_sdl_screen = false;
int8_t   _name_scrl_shift_ctr = 1;
uint8_t  _name_scrl_shift = 0;
int32_t  _name_scrl_pending_frames = 0;
#endif // !GO32
int32_t  _cursor_blink_factor = 13;
int32_t  _cursor_blink_pending_frames = 0;
bool     _realtime_gfx_no_update = false;
#if GO32
int32_t  _screen_refresh_pending_frames = 0;
struct _custom_svga_cfg_rec_t _custom_svga_cfg[31] = {
 { .flag = false, .value = -1 }, { .flag = false, .value = -1 },
 { .flag = false, .value = -1 }, { .flag = false, .value = -1 },
 { .flag = false, .value = -1 }, { .flag = false, .value = -1 },
 { .flag = false, .value = -1 }, { .flag = false, .value = -1 },
 { .flag = false, .value = -1 }, { .flag = false, .value = -1 },
 { .flag = false, .value = -1 }, { .flag = false, .value = -1 },
 { .flag = false, .value = -1 }, { .flag = false, .value = -1 },
 { .flag = false, .value = -1 }, { .flag = false, .value = -1 },
 { .flag = false, .value = -1 }, { .flag = false, .value = -1 },
 { .flag = false, .value = -1 }, { .flag = false, .value = -1 },
 { .flag = false, .value = -1 }, { .flag = false, .value = -1 },
 { .flag = false, .value = -1 }, { .flag = false, .value = -1 },
 { .flag = false, .value = -1 }, { .flag = false, .value = -1 },
 { .flag = false, .value = -1 }, { .flag = false, .value = -1 },
 { .flag = false, .value = -1 }, { .flag = false, .value = -1 },
 { .flag = false, .value = -1 }
};
#endif // GO32

bool    _force_program_quit = false;
uint8_t _traceprc_last_order = 0;
uint8_t _traceprc_last_pattern = 0;
uint8_t _traceprc_last_line = 0;
uint8_t _pattedit_lastpos = 0;

#if !GO32
void   *_FrameBuffer = NULL;
#else // GO32
uint8_t _FrameBuffer_mirror[1024*768];
#endif // GO32

#if GO32
#include "adt2sys/go32/dump_VESA_buffer.c"
#endif // !GO32
