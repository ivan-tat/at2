// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define UINT8_NULL 0xFF
#define UINT16_NULL 0xFFFF
#define UINT32_NULL 0xFFFFFFFF
#define INT8_NULL (-1)
#define INT16_NULL (-1)
#define INT32_NULL (-1)
#define BYTE_NULL UINT8_NULL
#define WORD_NULL UINT16_NULL
#define DWORD_NULL UINT32_NULL

extern String   home_dir_path[255+1];
extern String   a2b_default_path[255+1];
extern String   a2f_default_path[255+1];
extern String   a2i_default_path[255+1];
extern String   a2m_default_path[255+1];
extern String   a2p_default_path[255+1];
extern String   a2t_default_path[255+1];
extern String   a2w_default_path[255+1];

#if GO32

extern uint8_t  screen_mode;
extern uint8_t  comp_text_mode;
extern bool     custom_svga_mode;
extern uint8_t  fps_down_factor;

extern uint8_t  typematic_rate;
extern uint8_t  typematic_delay;

extern bool     mouse_disabled;
extern uint16_t mouse_hspeed;
extern uint16_t mouse_vspeed;
extern uint16_t mouse_threshold;

#else // !GO32

extern uint8_t  sdl_screen_mode;
extern uint8_t  sdl_frame_rate;

extern int32_t  sdl_typematic_delay;
extern int32_t  sdl_typematic_rate;

extern uint8_t  sdl_opl3_emulator;
extern int32_t  sdl_sample_rate;
extern int32_t  sdl_sample_buffer;
extern uint8_t  sdl_timer_slowdown;
extern String   sdl_wav_directory[255+1];

#endif // !GO32

extern uint8_t command_typing;
extern int32_t ssaver_time;
extern bool    use_large_cursor;

extern CharSet_t _valid_characters; // HINT: (FPC) set of char
extern CharSet_t _valid_characters_fname; // HINT: (FPC) set of char
