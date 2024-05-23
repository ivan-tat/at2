// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String   home_dir_path[255+1] = { 0 };
String   a2b_default_path[255+1] = { 0 };
String   a2f_default_path[255+1] = { 0 };
String   a2i_default_path[255+1] = { 0 };
String   a2m_default_path[255+1] = { 0 };
String   a2p_default_path[255+1] = { 0 };
String   a2t_default_path[255+1] = { 0 };
String   a2w_default_path[255+1] = { 0 };

#if GO32

uint8_t  screen_mode = 0;
uint8_t  comp_text_mode = 0;
bool     custom_svga_mode = false;
uint8_t  fps_down_factor = 0;

uint8_t  typematic_rate = 0;
uint8_t  typematic_delay = 0;

bool     mouse_disabled = false;
uint16_t mouse_hspeed = 50;
uint16_t mouse_vspeed = 50;
uint16_t mouse_threshold = 0;

#else // !GO32

uint8_t  sdl_screen_mode = 0;
uint8_t  sdl_frame_rate = 100;

int32_t  sdl_typematic_delay = 250;
int32_t  sdl_typematic_rate = 30;

uint8_t  sdl_opl3_emulator = 0;
int32_t  sdl_sample_rate = 49716;
int32_t  sdl_sample_buffer = 2048;
uint8_t  sdl_timer_slowdown = 2;
String   sdl_wav_directory[255+1] = "\x0A" "wav_files" DIR_SEP_STR;

#endif // !GO32

uint8_t command_typing = 1;
int32_t ssaver_time = 300;
bool    use_large_cursor = false;

CharSet_t _valid_characters = { // 0, 32..127, 176..223, 254, 255
  MK_UINT8 (1,0,0,0,0,0,0,0), MK_UINT8 (0,0,0,0,0,0,0,0), // 0
  0x00, 0x00,
  0xFF, 0xFF, // 32..127
  0xFF, 0xFF,
  0xFF, 0xFF,
  0xFF, 0xFF,
  0xFF, 0xFF,
  0xFF, 0xFF,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0xFF, 0xFF, // 176..223
  0xFF, 0xFF,
  0xFF, 0xFF,
  0x00, 0x00,
  MK_UINT8 (0,0,0,0,0,0,0,0), MK_UINT8 (0,0,0,0,0,0,1,1) // 254, 255
};

CharSet_t _valid_characters_fname = { // 0, 32..127, 255
  MK_UINT8 (1,0,0,0,0,0,0,0), MK_UINT8 (0,0,0,0,0,0,0,0), // 0
  0x00, 0x00,
  0xFF, 0xFF, // 32..127
  0xFF, 0xFF,
  0xFF, 0xFF,
  0xFF, 0xFF,
  0xFF, 0xFF,
  0xFF, 0xFF,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  MK_UINT8 (0,0,0,0,0,0,0,0), MK_UINT8 (0,0,0,0,0,0,0,1) // 255
};
