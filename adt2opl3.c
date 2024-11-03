// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"
#include <string.h>
#include "debug.h"
#if GO32
#include <pc.h>
#include <stdlib.h> // atexit()
#include "go32/adt2dpmi.h"
#include "txtscrio.h"
#else // !GO32
//#include <math.h>
#include "platform.h"
#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <SDL/SDL_audio.h>
#if USE_FPC
#include "SDL/SDL_audio_fix.h"
#endif // USE_FPC
#include "opl3emu.h"
#endif // !GO32
#include "go32/PIT/PIT_consts.h"
#include "adt2unit.h"
#include "adt2opl3.h"

static void opl3out_proc (uint16_t reg, uint16_t data);

#if GO32

// IRQ/library data/code start {

static const char ___ADT2OPL3_CONST_START___ = 0;
static char       ___ADT2OPL3_DATA_START___ = 0;
static char       ___ADT2OPL3_BSS_START___;
static __NO_REORDER __ALIGNED_(1) __NAKED_RELAXED void
                  ___ADT2OPL3_CODE_START___ (void) { }

uint16_t opl3port = 0;
uint8_t  opl_latency = 0;

static uint16_t _opl_regs_cache[0x10000];

#include "adt2opl3/go32/opl2out.c"
#include "adt2opl3/go32/opl3out_proc.c"
#include "adt2opl3/go32/opl3exp.c"

#else // !GO32

bool renew_wav_files_flag = true;
bool opl3_channel_recording_mode = false;
bool opl3_record_channel[20] = { false, };

#include "adt2opl3/sdl/opl2out.c"
#include "adt2opl3/sdl/opl3out_proc.c"
#include "adt2opl3/sdl/opl3exp.c"

static int32_t sample_frame_size;
static SDL_AudioSpec sdl_audio_spec;

#include "adt2opl3/sdl/opl3_init.c"
#include "adt2opl3/sdl/opl3_done.c"
#include "adt2opl3/sdl/snd_SetTimer.c"
#include "adt2opl3/sdl/get_num_files.c" // HINT: static
#include "adt2opl3/sdl/update_recorded_channels.c"

bool opl3_flushmode = false;

int32_t wav_buffer_len;
uint8_t wav_buffer[19][18*WAV_BUFFER_SIZE]; // HINT: (FPC) start index 0,0

static void *opl3_sample_buffer_ptr = NULL;
static void *opl3_sample_buffer_chan_ptr[18] = { NULL, }; // HINT: (FPC) start index 1

#include "adt2opl3/sdl/playcallback.c" // HINT: static

/*static*/ extern void flush_WAV_data (void); // TODO: port to C

#include "adt2opl3/sdl/snd_init.c"
#include "adt2opl3/sdl/snd_done.c"

#endif // !GO32

tOPL3OUT_proc *opl3out = opl3out_proc;

#if GO32

static const char ___ADT2OPL3_CONST_END___ = 0;
static char       ___ADT2OPL3_DATA_END___ = 0;
static char       ___ADT2OPL3_BSS_END___;
static __NO_REORDER __ALIGNED_(1) __NAKED_RELAXED void
                  ___ADT2OPL3_CODE_END___ (void) { }

// } IRQ/library data/code end

#include "adt2opl3/go32/detect_OPL3.c"
#include "adt2opl3/go32/init_adt2opl3.c"

#endif // GO32
