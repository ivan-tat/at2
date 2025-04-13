// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "debug.h"
#if GO32
#include "go32/adt2dpmi.h"
#include "go32/adt2vesa.h"
#else // !GO32
#include <SDL/SDL.h>
#include <SDL/SDL_rwops.h>
#include <SDL/SDL_timer.h>
#include <SDL/SDL_video.h>
#if USE_FPC
#include "SDL/SDL_fix.h"
#include "SDL/SDL_rwops_fix.h"
#include "SDL/SDL_timer_fix.h"
#include "SDL/SDL_video_fix.h"
#endif // USE_FPC
#include "adt2unit.h" // tRGB (=SDL_Color)
#include "adt2opl3.h" // snd_init
#endif // !GO32
#include "adt2data.h" // font8x16, adt2_icon_bitmap
#include "parserio.h" // Compare()
#include "txtscrio.h"
#include "adt2unit.h" // do_synchronize
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

#if !GO32
static SDL_Surface *screen;
static tRGB rgb_color_alt[16]; // HINT: (FPC) start index 0
#endif // !GO32

#include "adt2sys/sys_init.c"
#include "adt2sys/sys_done.c"

#if GO32

#include "adt2sys/go32/draw_VESA_screen_800x600_1.c" // static
#include "adt2sys/go32/draw_VESA_screen_800x600_2.c" // static
#include "adt2sys/go32/draw_VESA_screen_1024x768.c" // static

#include "adt2sys/go32/dump_VESA_buffer.c" // static
#include "adt2sys/go32/shift_text_screen.c" // static
#include "adt2sys/go32/draw_screen.c"

#else // !GO32

#if i386

#include "adt2sys/sdl/draw_SDL_screen_720x480.c" // static
#include "adt2sys/sdl/draw_SDL_screen_960x800.c" // static
#include "adt2sys/sdl/draw_SDL_screen_1440x960.c" // static

#else // !i386

#include "adt2sys/sdl/draw_SDL_screen.c" // static

#endif // !i386

#include "adt2sys/sdl/draw_screen_proc.c" // static
#include "adt2sys/sdl/vid_Init.c"
#include "adt2sys/sdl/vid_Deinit.c"
#include "adt2sys/sdl/vid_SetRGBPalette.c"
#include "adt2sys/sdl/draw_screen.c"
#include "adt2sys/sdl/vid_SetVideoMode.c"
#include "adt2sys/sdl/vid_FadeOut.c"

#endif // !GO32

#include "adt2sys/ResetF_RW.c"
#include "adt2sys/ResetF.c"
#include "adt2sys/RewriteF.c"
#include "adt2sys/BlockReadF.c"
#include "adt2sys/BlockWriteF.c"
#include "adt2sys/SeekF.c"
#include "adt2sys/EraseF.c"
#include "adt2sys/CloseF.c"
