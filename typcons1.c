// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// This file is included in `adt2unit.c'

#if !ADT2PLAY

PASCALSTRING String status_layout[3][10+1] = { // HINT: (FPC) start index 0 (see tPLAY_STATUS)
  "\x0A" "PLAYiNG   ",
  "\x0A" "PAUSED    ",
  "\x0A" "STOPPED   "
};

uint8_t inst_hpos[inst_vpos_max][inst_hpos_max] = { // HINT: (FPC) start index 1,1
  { 15,  0,  0,  0,  0,  0,  0 }, //0
  { 15, 52,  0,  0,  0,  0,  0 }, //1
  { 52,  0,  0,  0,  0,  0,  0 }, //2
  { 15, 52,  0,  0,  0,  0,  0 }, //3
  { 15, 52,  0,  0,  0,  0,  0 }, //4
  { 52,  0,  0,  0,  0,  0,  0 }, //5
  { 15, 52,  0,  0,  0,  0,  0 }, //6
  { 52,  0,  0,  0,  0,  0,  0 }, //7
  { 52,  0,  0,  0,  0,  0,  0 }, //8
  {  2,  0,  0,  0,  0,  0,  0 }, //9
  {  2,  0,  0,  0,  0,  0,  0 }, //10
  {  0,  0,  0,  0,  0,  0,  0 }, //11
  {  0,  0,  0,  0,  0,  0,  0 }, //12
  {  0,  0,  0,  0,  0,  0,  0 }, //13
  {  0,  0,  0,  0,  0,  0,  0 }, //14
  {  0,  0,  0,  0,  0,  0,  0 }, //15
  {  0,  0,  0,  0,  0,  0,  0 }, //16
  {  2,  0,  0,  0,  0,  0,  0 }, //17
  {  2, 29, 36, 43, 50, 57, 64 }, //18
  {  2, 29, 36, 43, 50, 57, 64 }, //19
  {  2, 29, 36, 43, 50,  0,  0 }  //20
};

const uint8_t inst_hpos_ext_2op[6][inst_hpos_max] = { // HINT: (FPC) start index 1,1
  { 2, 22, 51, 0, 0, 0, 0 }, //11
  { 2, 22,  0, 0, 0, 0, 0 }, //12
  { 0,  0,  0, 0, 0, 0, 0 }, //13
  { 0,  0,  0, 0, 0, 0, 0 }, //14
  { 0, 51,  0, 0, 0, 0, 0 }, //15
  { 1,  9, 17, 0, 0, 0, 0 }  //16
};

const uint8_t inst_hpos_ext_perc[6][inst_hpos_max] = { // HINT: (FPC) start index 1,1
  { 2,  0,  0, 0, 0, 0, 0 }, //11
  { 2,  0,  0, 0, 0, 0, 0 }, //12
  { 0,  0,  0, 0, 0, 0, 0 }, //13
  { 0,  0,  0, 0, 0, 0, 0 }, //14
  { 0, 51,  0, 0, 0, 0, 0 }, //15
  { 1,  9, 17, 0, 0, 0, 0 }  //16
};

const uint8_t inst_hpos_ext_4op[6][inst_hpos_max] = { // HINT: (FPC) start index 1,1
  {  2,  0, 51,  0, 0, 0, 0 }, //11
  {  2,  0,  0,  0, 0, 0, 0 }, //12
  { 22,  0,  0,  0, 0, 0, 0 }, //13
  { 22,  0,  0,  0, 0, 0, 0 }, //14
  { 22, 51,  0,  0, 0, 0, 0 }, //15
  {  1,  9, 17, 22, 0, 0, 0 }  //16
};

const uint8_t inst_vpos[inst_vpos_max] = { // HINT: (FPC) start index 1
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 20, 21, 22, 23
};

PASCALSTRING String note_layout[12*8+2][3+1] = { // HINT: (FPC) start index 0
  "\x03" "   ",
  "\x03" "C-1", "\x03" "C#1", "\x03" "D-1", "\x03" "D#1", "\x03" "E-1", "\x03" "F-1", "\x03" "F#1", "\x03" "G-1", "\x03" "G#1", "\x03" "A-1", "\x03" "A#1", "\x03" "%-1",
  "\x03" "C-2", "\x03" "C#2", "\x03" "D-2", "\x03" "D#2", "\x03" "E-2", "\x03" "F-2", "\x03" "F#2", "\x03" "G-2", "\x03" "G#2", "\x03" "A-2", "\x03" "A#2", "\x03" "%-2",
  "\x03" "C-3", "\x03" "C#3", "\x03" "D-3", "\x03" "D#3", "\x03" "E-3", "\x03" "F-3", "\x03" "F#3", "\x03" "G-3", "\x03" "G#3", "\x03" "A-3", "\x03" "A#3", "\x03" "%-3",
  "\x03" "C-4", "\x03" "C#4", "\x03" "D-4", "\x03" "D#4", "\x03" "E-4", "\x03" "F-4", "\x03" "F#4", "\x03" "G-4", "\x03" "G#4", "\x03" "A-4", "\x03" "A#4", "\x03" "%-4",
  "\x03" "C-5", "\x03" "C#5", "\x03" "D-5", "\x03" "D#5", "\x03" "E-5", "\x03" "F-5", "\x03" "F#5", "\x03" "G-5", "\x03" "G#5", "\x03" "A-5", "\x03" "A#5", "\x03" "%-5",
  "\x03" "C-6", "\x03" "C#6", "\x03" "D-6", "\x03" "D#6", "\x03" "E-6", "\x03" "F-6", "\x03" "F#6", "\x03" "G-6", "\x03" "G#6", "\x03" "A-6", "\x03" "A#6", "\x03" "%-6",
  "\x03" "C-7", "\x03" "C#7", "\x03" "D-7", "\x03" "D#7", "\x03" "E-7", "\x03" "F-7", "\x03" "F#7", "\x03" "G-7", "\x03" "G#7", "\x03" "A-7", "\x03" "A#7", "\x03" "%-7",
  "\x03" "C-8", "\x03" "C#8", "\x03" "D-8", "\x03" "D#8", "\x03" "E-8", "\x03" "F-8", "\x03" "F#8", "\x03" "G-8", "\x03" "G#8", "\x03" "A-8", "\x03" "A#8", "\x03" "%-8",
  "\x03" "C-9"
};

const uint8_t board_scancodes[29] = { // HINT: (FPC) start index 1
//C     C#    D     D#    E     F     F#    G     G#    A     A#    B
  SC_Z, SC_S, SC_X, SC_D, SC_C, SC_V, SC_G, SC_B, SC_H, SC_N, SC_J, SC_M, // octave 0
  SC_Q, SC_2, SC_W, SC_3, SC_E, SC_R, SC_5, SC_T, SC_6, SC_Y, SC_7, SC_U, // octave 1
  SC_I, SC_9, SC_O, SC_0, SC_P // octave 2
};

const GCC_ATTRIBUTE((nonstring)) char fx_digits[48] = { // HINT: (FPC) start index 0
  "0123456789" //0-9
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ" //10-35
  "&" //36
  "%" //37
  "!" //38
  "@" //39
  "=" //40
  "#" //41
  "$" //42
  "~" //43
  "^" //44
  "`" //45
  ">" //46
  "<" //47
};

char b_note = 'b';

const PASCALSTRING String connection_str[2][2+1] = { // HINT: (FPC) start index 0
  "\x02" "FM",
  "\x02" "AM"
};

const PASCALSTRING String macro_retrig_str[8][1+1] = { // HINT: (FPC) start index 0, original string size was 255
  "\x01" "\xFA", // HINT: (AT2) custom glyph - center dot
  "\x01" "\x9F", // HINT: (AT2) custom glyph - 'x' (multiply operator)
  "\x01" "\x0B", // HINT: (AT2) custom glyph - medium black circle
  "\x01" "\xFA", // HINT: (AT2) custom glyph - center dot
  "\x01" "\x0D", // HINT: (AT2) custom glyph - 1/16th note
  "\x01" "\xFA", // HINT: (AT2) custom glyph - center dot
  "\x01" "\xFA", // HINT: (AT2) custom glyph - center dot
  "\x01" "\xFA"  // HINT: (AT2) custom glyph - center dot
};

const uint8_t pos1[22] = { // HINT: (FPC) start index 1
  7* 1, 7* 2, 7* 3, 7* 4, 7* 5, 7* 6, 7* 7, 7* 8, 7* 9, 7*10,
  7*11, 7*12, 7*13, 7*14, 7*15, 7*16, 7*17, 7*18, 7*19, 7*20,
  7*21, 7*22
};

const uint8_t pos2[13] = { // HINT: (FPC) start index 1
  0, 1, 3, 4, 6, 7, 9, 10, 12, 13, 14, 15, 16
};

const uint8_t pos3[44] = { // HINT: (FPC) start index 1
  // 0123456789ABCDEF
  // ... 00 000 000
  // ^   ^  ^   ^
  15* 0+0, 15* 0+4, 15* 0+7, 15* 0+11, //0
  15* 1+0, 15* 1+4, 15* 1+7, 15* 1+11, //1
  15* 2+0, 15* 2+4, 15* 2+7, 15* 2+11, //2
  15* 3+0, 15* 3+4, 15* 3+7, 15* 3+11, //3
  15* 4+0, 15* 4+4, 15* 4+7, 15* 4+11, //4
  15* 5+0, 15* 5+4, 15* 5+7, 15* 5+11, //5
  15* 6+0, 15* 6+4, 15* 6+7, 15* 6+11, //6
  15* 7+0, 15* 7+4, 15* 7+7, 15* 7+11, //7
  15* 8+0, 15* 8+4, 15* 8+7, 15* 8+11, //8
  15* 9+0, 15* 9+4, 15* 9+7, 15* 9+11, //9
  15*10+0, 15*10+4, 15*10+7, 15*10+11  //10
};

const uint8_t pos4[110] = { // HINT: (FPC) start index 1
  // 0123456789ABCDEF
  // ... 00 000 000
  // ^ ^ ^^ ^^^ ^^^
  15* 0+0, 15* 0+2, 15* 0+4, 15* 0+5, 15* 0+7, 15* 0+8, 15* 0+9, 15* 0+11, 15* 0+12, 15* 0+13, //0
  15* 1+0, 15* 1+2, 15* 1+4, 15* 1+5, 15* 1+7, 15* 1+8, 15* 1+9, 15* 1+11, 15* 1+12, 15* 1+13, //1
  15* 2+0, 15* 2+2, 15* 2+4, 15* 2+5, 15* 2+7, 15* 2+8, 15* 2+9, 15* 2+11, 15* 2+12, 15* 2+13, //2
  15* 3+0, 15* 3+2, 15* 3+4, 15* 3+5, 15* 3+7, 15* 3+8, 15* 3+9, 15* 3+11, 15* 3+12, 15* 3+13, //3
  15* 4+0, 15* 4+2, 15* 4+4, 15* 4+5, 15* 4+7, 15* 4+8, 15* 4+9, 15* 4+11, 15* 4+12, 15* 4+13, //4
  15* 5+0, 15* 5+2, 15* 5+4, 15* 5+5, 15* 5+7, 15* 5+8, 15* 5+9, 15* 5+11, 15* 5+12, 15* 5+13, //5
  15* 6+0, 15* 6+2, 15* 6+4, 15* 6+5, 15* 6+7, 15* 6+8, 15* 6+9, 15* 6+11, 15* 6+12, 15* 6+13, //6
  15* 7+0, 15* 7+2, 15* 7+4, 15* 7+5, 15* 7+7, 15* 7+8, 15* 7+9, 15* 7+11, 15* 7+12, 15* 7+13, //7
  15* 8+0, 15* 8+2, 15* 8+4, 15* 8+5, 15* 8+7, 15* 8+8, 15* 8+9, 15* 8+11, 15* 8+12, 15* 8+13, //8
  15* 9+0, 15* 9+2, 15* 9+4, 15* 9+5, 15* 9+7, 15* 9+8, 15* 9+9, 15* 9+11, 15* 9+12, 15* 9+13, //9
  15*10+0, 15*10+2, 15*10+4, 15*10+5, 15*10+7, 15*10+8, 15*10+9, 15*10+11, 15*10+12, 15*10+13  //10
};

const uint8_t pos5[35] = { // HINT: (FPC) start index 1
   1,  3,  5,  7,  9, 11, 12, 14, 16, 18,
  19, 20, 21, 23, 25, 27, 29, 31, 33, 34,
  36, 38, 40, 41, 42, 43, 45, 47,
  49, 50, 51, 52, 54, 56, 57
};

const uint8_t pos5vw[35] = { // HINT: (FPC) start index 1
   1,  3,  5,  7,  9, 11, 11, 14, 16, 18,
  19, 20, 21, 23, 25, 27, 29, 31, 33, 33,
  36, 38, 40, 41, 42, 43, 45, 47,
  49, 49, 49, 49, 54, 56, 56
};

const uint8_t pos6[11] = { // HINT: (FPC) start index 1
  1, 2, 3, 5, 6, 8, 9, 10, 12, 13, 14
};

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

#endif // !ADT2PLAY

const uint8_t _panning[3] = { // HINT: (FPC) start index 0
  0x30, 0x10, 0x20
};

const uint8_t _instr_data_ofs[13] = { // HINT: (FPC) start index 1
  1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 10, 11, 12
};

const uint8_t _instr[12] = { // HINT: (FPC) start index 0
  0x20, 0x20,
  0x40, 0x40,
  0x60, 0x60,
  0x80, 0x80,
  0xE0, 0xE0,
  0xC0,
  0xBD
};

const tTRACK_ADDR _chmm_n = { // HINT: (FPC) start index 1
  0x003, 0x000, 0x004, 0x001, 0x005, 0x002, 0x006, 0x007, 0x008, // 01-09
  0x103, 0x100, 0x104, 0x101, 0x105, 0x102, 0x106, 0x107, 0x108, // 10-18
  BYTE_NULL, // 19
  BYTE_NULL  // 20
};

const tTRACK_ADDR _chmm_m = { // HINT: (FPC) start index 1
  0x008, 0x000, 0x009, 0x001, 0x00A, 0x002, 0x010, 0x011, 0x012, // 01-09
  0x108, 0x100, 0x109, 0x101, 0x10A, 0x102, 0x110, 0x111, 0x112, // 10-18
  BYTE_NULL, // 19
  BYTE_NULL  // 20
};

const tTRACK_ADDR _chmm_c = { // HINT: (FPC) start index 1
  0x00B, 0x003, 0x00C, 0x004, 0x00D, 0x005, 0x013, 0x014, 0x015, // 01-09
  0x10B, 0x103, 0x10C, 0x104, 0x10D, 0x105, 0x113, 0x114, 0x115, // 10-18
  BYTE_NULL, // 19
  BYTE_NULL  // 20
};

const tTRACK_ADDR _chpm_n = { // HINT: (FPC) start index 1
  0x003, 0x000, 0x004, 0x001, 0x005, 0x002, 0x106, 0x107, 0x108, // 01-09
  0x103, 0x100, 0x104, 0x101, 0x105, 0x102, // 10-15
  0x006, // 16 - BD
  0x007, // 17 - SD
  0x008, // 18 - TT
  0x008, // 19 - TC
  0x007  // 20 - HH
};

const tTRACK_ADDR _chpm_m = { // HINT: (FPC) start index 1
  0x008, 0x000, 0x009, 0x001, 0x00A, 0x002, 0x110, 0x111, 0x112, // 01-09
  0x108, 0x100, 0x109, 0x101, 0x10A, 0x102, // 10-15
  0x010, // 16 - BD
  0x014, // 17 - SD
  0x012, // 18 - TT
  0x015, // 19 - TC
  0x011  // 20 - HH
};

const tTRACK_ADDR _chpm_c = { // HINT: (FPC) start index 1
  0x00B, 0x003, 0x00C, 0x004, 0x00D, 0x005, 0x113, 0x114, 0x115, // 01-09
  0x10B, 0x103, 0x10C, 0x104, 0x10D, 0x105, // 10-15
  0x013,     // 16 - BD
  BYTE_NULL, // 17 - SD
  BYTE_NULL, // 18 - TT
  BYTE_NULL, // 19 - TC
  BYTE_NULL  // 20 - HH
};

const uint8_t _4op_tracks_hi[256/8] = { // HINT: (FPC) set of byte, chan: 1..20
  MK_UINT8 (0,1,0,1,0,1,0,0), MK_UINT8 (0,0,1,0,1,0,1,0),  // 1,3,5,10,12,14
  0,
};

const uint8_t _4op_tracks_lo[256/8] = { // HINT: (FPC) set of byte, chan: 1..20
  MK_UINT8 (0,0,1,0,1,0,1,0), MK_UINT8 (0,0,0,1,0,1,0,1),  // 2,4,6,11,13,15
  0,
};

const uint8_t _4op_main_chan[6] = { // chan: 1..20, HINT: (FPC) start index 1
  2, 4, 6, 11, 13, 15
};

#if !ADT2PLAY
const uint8_t _perc_sim_chan[2] = { // HINT: (FPC) start index 19
  18, 17
};
#endif // !ADT2PLAY

tTRACK_ADDR _chan_n; // HINT: (FPC) start index 1
tTRACK_ADDR _chan_m; // HINT: (FPC) start index 1
tTRACK_ADDR _chan_c; // HINT: (FPC) start index 1
#if !ADT2PLAY
tTRACK_ADDR _ch_tmp; // HINT: (FPC) start index 1
#endif // !ADT2PLAY

#if !ADT2PLAY

const uint8_t fade_delay_tab[64] = { // HINT: (FPC) start index 0
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  4, 4, 4, 4, 4, 4, 4,
  5, 5, 5, 5
};

bool tracing     = false;
bool track_notes = false;
bool marking     = false;
bool debugging   = false;
bool quick_cmd   = false;

PASCALSTRING String home_dir_path[255+1] = { 0 };
PASCALSTRING String a2b_default_path[255+1] = { 0 };
PASCALSTRING String a2f_default_path[255+1] = { 0 };
PASCALSTRING String a2i_default_path[255+1] = { 0 };
PASCALSTRING String a2m_default_path[255+1] = { 0 };
PASCALSTRING String a2p_default_path[255+1] = { 0 };
PASCALSTRING String a2t_default_path[255+1] = { 0 };
PASCALSTRING String a2w_default_path[255+1] = { 0 };

#if GO32

uint8_t  screen_mode = 0;
uint8_t  comp_text_mode = 0;
bool     custom_svga_mode = false;
uint8_t  fps_down_factor = 0;

uint8_t  typematic_delay = 0;
uint8_t  typematic_rate = 0;

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
PASCALSTRING String sdl_wav_directory[255+1] = "\x0A" "wav_files" DIR_SEP_STR;

#endif // !GO32

uint8_t  init_tempo          = 50;
uint8_t  init_speed          = 6;
uint16_t init_macro_speedup  = 1;
bool     midiboard           = true;
uint8_t  default_octave      = 4;
uint16_t patt_len            = 64;
uint8_t  nm_tracks           = 18;
bool     mod_description     = false;
bool     highlight_controls  = true;
bool     use_H_for_B         = false;
bool     linefeed            = true;
bool     lf_in_mboard_mode   = false;
bool     update_ins          = false;
bool     cycle_pattern       = true;
bool     keep_track_pos      = false;
bool     remember_ins_pos    = true;
uint8_t  command_typing      = 1;
uint8_t  backspace_dir       = 1;
uint8_t  mark_line           = 4;
bool     scroll_bars         = true;
uint8_t  fforward_factor     = 1;
uint8_t  rewind_factor       = 1;
int32_t  ssaver_time         = 300;
bool     timer_fix           = true;
float    decay_bar_rise      = 3.00f;
float    decay_bar_fall      = 0.30f;
uint8_t  force_ins           = 0;
uint8_t  pattern_layout      = 0;
bool     trace_by_default    = true;
bool     nosync_by_default   = false;
bool     keep_position       = true;
bool     alt_ins_name        = true;
bool     adjust_tracks       = true;
bool     mark_lines          = false;
bool     use_large_cursor    = false;
bool     accurate_conv       = false;
bool     fix_c_note_bug      = true;

uint8_t  analyzer_bckg       = 0x10;
uint8_t  analyzer            = 0x03;
uint8_t  analyzer_ovrllvol   = 0x03;
uint8_t  analyzer_volumelvl  = 0x07;
uint8_t  analyzer_modulator  = 0x02;
uint8_t  analyzer_carrier    = 0x03;

uint8_t  pattern_bckg        = 0x10;
uint8_t  pattern_border      = 0x0F;
uint8_t  pattern_pos_indic   = 0x0B;
uint8_t  pattern_pan_indic   = 0x03;
uint8_t  pattern_gpan_indic  = 0x0F;
uint8_t  pattern_lock_indic  = 0x07;
uint8_t  pattern_4op_indic   = 0x0E;
uint8_t  pattern_perc_indic  = 0x0F;
uint8_t  pattern_chan_indic  = 0x0E;
uint8_t  pattern_row_bckg    = 0x50;
uint8_t  pattern_row_bckg_p  = 0xC0;
uint8_t  pattern_row_bckg_m  = 0xB0;
uint8_t  pattern_block_bckg  = 0x20;
uint8_t  pattern_line        = 0x03;
uint8_t  pattern_line_p      = 0x00;
uint8_t  pattern_line_m      = 0x0B;
uint8_t  pattern_hi_line     = 0x0B;
uint8_t  pattern_hi_line_m   = 0x03;
uint8_t  pattern_note        = 0x07;
uint8_t  pattern_hi_note     = 0x0F;
uint8_t  pattern_note0       = 0x03;
uint8_t  pattern_hi_note0    = 0x0C;
uint8_t  pattern_note_hid    = 0x09;
uint8_t  pattern_hi_note_h   = 0x07;
uint8_t  pattern_inst        = 0x02;
uint8_t  pattern_hi_inst     = 0x0A;
uint8_t  pattern_inst0       = 0x02;
uint8_t  pattern_hi_inst0    = 0x0A;
uint8_t  pattern_cmnd        = 0x07;
uint8_t  pattern_hi_cmnd     = 0x0F;
uint8_t  pattern_cmnd0       = 0x03;
uint8_t  pattern_hi_cmnd0    = 0x0B;
uint8_t  pattern_note_m      = 0x09;
uint8_t  pattern_note0_m     = 0x09;
uint8_t  pattern_note_hid_m  = 0x0D;
uint8_t  pattern_inst_m      = 0x02;
uint8_t  pattern_inst0_m     = 0x02;
uint8_t  pattern_cmnd_m      = 0x09;
uint8_t  pattern_cmnd0_m     = 0x09;
uint8_t  pattern_note_b      = 0x0F;
uint8_t  pattern_note0_b     = 0x0B;
uint8_t  pattern_note_hid_b  = 0x05;
uint8_t  pattern_inst_b      = 0x0A;
uint8_t  pattern_inst0_b     = 0x0A;
uint8_t  pattern_cmnd_b      = 0x0F;
uint8_t  pattern_cmnd0_b     = 0x0B;
uint8_t  pattern_fix_note    = 0x0B;
uint8_t  pattern_hi_fx_note  = 0x0E;
uint8_t  pattern_fix_note_m  = 0x0E;
uint8_t  pattern_fix_note_b  = 0x0E;
uint8_t  pattern_cmnd_ctrl   = 0x0D;
uint8_t  pattern_input_bckg  = 0xB0;
uint8_t  pattern_input       = 0x00;
uint8_t  pattern_input_warn  = 0x80;

uint8_t  debug_info_bckg     = 0x40;
uint8_t  debug_info_bckg2    = 0x90;
uint8_t  debug_info_border   = 0x05;
uint8_t  debug_info_border2  = 0x0F;
uint8_t  debug_info_title    = 0x0B;
uint8_t  debug_info_topic    = 0x0E;
uint8_t  debug_info_txt      = 0x07;
uint8_t  debug_info_hi_txt   = 0x0F;
uint8_t  debug_info_txt_hid  = 0x06;
uint8_t  debug_info_mod      = 0x02;
uint8_t  debug_info_hi_mod   = 0x0A;
uint8_t  debug_info_car      = 0x03;
uint8_t  debug_info_hi_car   = 0x0B;
uint8_t  debug_info_4op      = 0x08;
uint8_t  debug_info_perc     = 0x08;
uint8_t  debug_info_bpm      = 0x08;

uint8_t  help_background     = 0x40;
uint8_t  help_title          = 0x0B;
uint8_t  help_border         = 0x0F;
uint8_t  help_topic          = 0x0C;
uint8_t  help_text           = 0x07;
uint8_t  help_hi_text        = 0x0C;
uint8_t  help_keys           = 0x0F;
uint8_t  help_indicators     = 0x0E;

uint8_t  dialog_background   = 0x40;
uint8_t  dialog_title        = 0x0B;
uint8_t  dialog_border       = 0x0F;
uint8_t  dialog_text         = 0x0D;
uint8_t  dialog_hi_text      = 0x0E;
uint8_t  dialog_hid          = 0x06;
uint8_t  dialog_item         = 0x0F;
uint8_t  dialog_short        = 0x0E;
uint8_t  dialog_item_dis     = 0x05;
uint8_t  dialog_sel_itm_bck  = 0x70;
uint8_t  dialog_sel_itm      = 0x00;
uint8_t  dialog_sel_short    = 0x04;
uint8_t  dialog_context      = 0x0E;
uint8_t  dialog_context_dis  = 0x07;
uint8_t  dialog_contxt_dis2  = 0x09;
uint8_t  dialog_input_bckg   = 0x70;
uint8_t  dialog_input        = 0x00;
uint8_t  dialog_def_bckg     = 0xC0;
uint8_t  dialog_def          = 0x00;
uint8_t  dialog_prog_bar1    = 0x09;
uint8_t  dialog_prog_bar2    = 0x0B;
uint8_t  dialog_topic        = 0x0D;
uint8_t  dialog_hi_topic     = 0x0E;
uint8_t  dialog_mod_text     = 0x0A;
uint8_t  dialog_car_text     = 0x0B;
uint8_t  dialog_misc_indic   = 0x08;

uint8_t  macro_background    = 0x40;
uint8_t  macro_title         = 0x0B;
uint8_t  macro_border        = 0x0F;
uint8_t  macro_topic         = 0x0C;
uint8_t  macro_topic2        = 0x0D;
uint8_t  macro_hi_topic      = 0x0E;
uint8_t  macro_text          = 0x0D;
uint8_t  macro_hi_text       = 0x0E;
uint8_t  macro_text_dis      = 0x06;
uint8_t  macro_text_loop     = 0x0C;
uint8_t  macro_text_keyoff   = 0x03;
uint8_t  macro_current_bckg  = 0x50;
uint8_t  macro_current       = 0x0D;
uint8_t  macro_current_dis   = 0x00;
uint8_t  macro_current_loop  = 0x0C;
uint8_t  macro_current_koff  = 0x0B;
uint8_t  macro_input_bckg    = 0x70;
uint8_t  macro_input         = 0x00;
uint8_t  macro_def_bckg      = 0xC0;
uint8_t  macro_def           = 0x00;
uint8_t  macro_scrbar_bckg   = 0x70;
uint8_t  macro_scrbar_text   = 0x00;
uint8_t  macro_scrbar_mark   = 0x0F;
uint8_t  macro_hint          = 0x07;
uint8_t  macro_item          = 0x0F;
uint8_t  macro_short         = 0x0E;
uint8_t  macro_item_dis      = 0x07;
uint8_t  macro_sel_itm_bck   = 0x70;
uint8_t  macro_sel_itm       = 0x00;
uint8_t  macro_sel_short     = 0x04;
uint8_t  macro_context       = 0x0E;
uint8_t  macro_context_dis   = 0x07;

uint8_t  scrollbar_bckg      = 0x30;
uint8_t  scrollbar_text      = 0x00;
uint8_t  scrollbar_mark      = 0x0B;
uint8_t  scrollbar_2nd_mark  = 0x0F;

uint8_t  main_background     = 0x10;
uint8_t  main_title          = 0x0B;
uint8_t  main_border         = 0x0B;
uint8_t  main_stat_line      = 0x03;
uint8_t  main_hi_stat_line   = 0x0B;
uint8_t  main_dis_stat_line  = 0x05;
uint8_t  main_behavior       = 0x0D;
uint8_t  main_behavior_dis   = 0x05;

uint8_t  status_background   = 0x10;
uint8_t  status_border       = 0x0E;
uint8_t  status_static_txt   = 0x0B;
uint8_t  status_dynamic_txt  = 0x0F;
uint8_t  status_play_state   = 0x07;
uint8_t  status_text_dis     = 0x05;

uint8_t  order_background    = 0x10;
uint8_t  order_hi_bckg       = 0x50;
uint8_t  order_border        = 0x0A;
uint8_t  order_entry         = 0x03;
uint8_t  order_hi_entry      = 0x0B;
uint8_t  order_pattn         = 0x07;
uint8_t  order_pattn_jump    = 0x02;
uint8_t  order_hi_pattn      = 0x0F;
uint8_t  order_played_b      = 0xC0;
uint8_t  order_played        = 0x00;
uint8_t  order_input_bckg    = 0xB0;
uint8_t  order_input         = 0x00;
uint8_t  order_input_warn    = 0xC0;

uint8_t  instrument_bckg     = 0x40;
uint8_t  instrument_title    = 0x0B;
uint8_t  instrument_border   = 0x0F;
uint8_t  instrument_text     = 0x07;
uint8_t  instrument_hi_text  = 0x0F;
uint8_t  instrument_glob     = 0x0D;
uint8_t  instrument_hi_glob  = 0x0F;
uint8_t  instrument_hid      = 0x06;
uint8_t  instrument_mod      = 0x02;
uint8_t  instrument_car      = 0x03;
uint8_t  instrument_hi_mod   = 0x0A;
uint8_t  instrument_hi_car   = 0x0B;
uint8_t  instrument_context  = 0x0E;
uint8_t  instrument_con_dis  = 0x05;
uint8_t  instrument_adsr     = 0x60;
uint8_t  instrument_ai_off   = 0x06;
uint8_t  instrument_ai_on    = 0x0D;
uint8_t  instrument_ai_trig  = 0x0E;

#endif // !ADT2PLAY

#if !ADT2PLAY
#if GO32

tRGB rgb_color[16] = { // HINT: (FPC) start index 0
  { .r =  0, .g =  0, .b =  0 },
  { .r =  5, .g = 10, .b = 30 },
  { .r =  0, .g = 30, .b = 30 },
  { .r =  0, .g = 30, .b = 40 },
  { .r =  0, .g =  0, .b = 20 },
  { .r = 15, .g = 25, .b = 35 },
  { .r =  0, .g = 10, .b = 25 },
  { .r = 20, .g = 30, .b = 40 },
  { .r = 45, .g = 35, .b = 50 },
  { .r = 10, .g = 20, .b = 30 },
  { .r = 20, .g = 40, .b = 40 },
  { .r = 20, .g = 40, .b = 50 },
  { .r = 25, .g = 35, .b = 50 },
  { .r = 30, .g = 35, .b = 45 },
  { .r = 35, .g = 60, .b = 60 },
  { .r = 40, .g = 50, .b = 55 }
};

#else // !GO32

tRGB rgb_color[16] = { // HINT: (FPC) start index 0
  { .r =  0 << 2, .g =  0 << 2, .b =  0 << 2, .a = 0 },
  { .r =  5 << 2, .g = 10 << 2, .b = 30 << 2, .a = 0 },
  { .r =  0 << 2, .g = 30 << 2, .b = 30 << 2, .a = 0 },
  { .r =  0 << 2, .g = 30 << 2, .b = 40 << 2, .a = 0 },
  { .r =  0 << 2, .g =  0 << 2, .b = 20 << 2, .a = 0 },
  { .r = 15 << 2, .g = 25 << 2, .b = 35 << 2, .a = 0 },
  { .r =  0 << 2, .g = 10 << 2, .b = 25 << 2, .a = 0 },
  { .r = 20 << 2, .g = 30 << 2, .b = 40 << 2, .a = 0 },
  { .r = 45 << 2, .g = 35 << 2, .b = 50 << 2, .a = 0 },
  { .r = 10 << 2, .g = 20 << 2, .b = 30 << 2, .a = 0 },
  { .r = 20 << 2, .g = 40 << 2, .b = 40 << 2, .a = 0 },
  { .r = 20 << 2, .g = 40 << 2, .b = 50 << 2, .a = 0 },
  { .r = 25 << 2, .g = 35 << 2, .b = 50 << 2, .a = 0 },
  { .r = 30 << 2, .g = 35 << 2, .b = 45 << 2, .a = 0 },
  { .r = 35 << 2, .g = 60 << 2, .b = 60 << 2, .a = 0 },
  { .r = 40 << 2, .g = 50 << 2, .b = 55 << 2, .a = 0 }
};

const tRGB vga_rgb_color[16] = { // HINT: (FPC) start index 0
  { .r =    0, .g =    0, .b =    0, .a = 0 },
  { .r =    0, .g =    0, .b = 0xAA, .a = 0 },
  { .r =    0, .g = 0xAA, .b =    0, .a = 0 },
  { .r =    0, .g = 0xAA, .b = 0xAA, .a = 0 },
  { .r = 0xAA, .g =    0, .b =    0, .a = 0 },
  { .r = 0xAA, .g =    0, .b = 0xAA, .a = 0 },
  { .r = 0xAA, .g = 0x55, .b =    0, .a = 0 },
  { .r = 0xAA, .g = 0xAA, .b = 0xAA, .a = 0 },
  { .r = 0x55, .g = 0x55, .b = 0x55, .a = 0 },
  { .r = 0x55, .g = 0x55, .b = 0xFF, .a = 0 },
  { .r = 0x55, .g = 0xFF, .b = 0x55, .a = 0 },
  { .r = 0x55, .g = 0xFF, .b = 0xFF, .a = 0 },
  { .r = 0xFF, .g = 0x55, .b = 0x55, .a = 0 },
  { .r = 0xFF, .g = 0x55, .b = 0xFF, .a = 0 },
  { .r = 0xFF, .g = 0xFF, .b = 0x55, .a = 0 },
  { .r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0 }
};

#endif // !GO32
#endif // !ADT2PLAY
