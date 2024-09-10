// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef ADT2OPL3_H
#define ADT2OPL3_H

#include "defines.h"
#include <stdbool.h>
#include <stdint.h>

void opl2out (uint16_t reg, uint16_t data);
void opl3exp (uint16_t data);

typedef void tOPL3OUT_proc (uint16_t reg, uint16_t data);

extern tOPL3OUT_proc *opl3out;

#if GO32

extern uint16_t opl3port;
extern uint8_t  opl_latency;

bool detect_OPL3 (void);

void init_adt2opl3 (void);

#else // !GO32

extern bool renew_wav_files_flag;
extern bool opl3_channel_recording_mode;
extern bool opl3_record_channel[20]; // HINT: (FPC) start index 1

void flush_WAV_data (void);
void opl3_init (void);
void opl3_done (void);
void snd_init (void);
void snd_done (void);
void snd_SetTimer (int32_t value);
void update_recorded_channels (void);

extern bool opl3_flushmode;

#define WAV_BUFFER_SIZE 512*1024 // cache buffer size -> 512k per file

extern int32_t wav_buffer_len;
extern uint8_t wav_buffer[19][18*WAV_BUFFER_SIZE]; // HINT: (FPC) start index 0,0

#endif // !GO32

#endif // !defined(ADT2OPL3_H)
