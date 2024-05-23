// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define SYMTABSIZE (sizeof (symtab) / sizeof (struct symtabrec_t))

static const struct symtabrec_t {
  SDLKey SDLK;
  AT2ExtKeyCode key;
  AT2ExtKeyCode shift;
  AT2ExtKeyCode caps;
  AT2ExtKeyCode caps_sh;
  AT2ExtKeyCode num;
  AT2ExtKeyCode num_sh;
  AT2ExtKeyCode ctrl;
  AT2ExtKeyCode alt;
  uint8_t scan;
} symtab[] = {
  {
    .SDLK    = SDLK_UNKNOWN,
    .key     = kNP5,
    .shift   = kNP5,
    .caps    = kNP5,
    .caps_sh = kNP5,
    .num     = kNP5,
    .num_sh  = kNP5,
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_NP_5
  }, {
    .SDLK    = SDLK_BACKSPACE,
    .key     = '\b',
    .shift   = '\b',
    .caps    = '\b',
    .caps_sh = '\b',
    .num     = '\b',
    .num_sh  = '\b',
    .ctrl    = 0x7F,
    .alt     = kNULL,
    .scan    = SC_BACKSPACE
  }, {
    .SDLK    = SDLK_TAB,
    .key     = '\t',
    .shift   = kShTAB,
    .caps    = '\t',
    .caps_sh = kShTAB,
    .num     = '\t',
    .num_sh  = kShTAB,
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_TAB
  },
  // SDLK_CLEAR
  {
    .SDLK    = SDLK_RETURN,
    .key     = '\r',
    .shift   = '\r',
    .caps    = '\r',
    .caps_sh = '\r',
    .num     = '\r',
    .num_sh  = '\r',
    .ctrl    = '\n',
    .alt     = kNULL,
    .scan    = SC_ENTER
  },
  // SDLK_PAUSE
  {
    .SDLK    = SDLK_ESCAPE,
    .key     = '\x1B',
    .shift   = '\x1B',
    .caps    = '\x1B',
    .caps_sh = '\x1B',
    .num     = '\x1B',
    .num_sh  = '\x1B',
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_ESCAPE
  }, {
    .SDLK    = SDLK_SPACE,
    .key     = ' ',
    .shift   = ' ',
    .caps    = ' ',
    .caps_sh = ' ',
    .num     = ' ',
    .num_sh  = ' ',
    .ctrl    = ' ',
    .alt     = ' ',
    .scan    = SC_SPACE
  },
  // SDLK_EXCLAIM
  // SDLK_QUOTEDBL
  // SDLK_HASH
  // SDLK_DOLLAR
  // SDLK_AMPERSAND
  {
    .SDLK    = SDLK_QUOTE,
    .key     = '\'',
    .shift   = '"',
    .caps    = '\'',
    .caps_sh = '"',
    .num     = '\'',
    .num_sh  = '"',
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_QUOTE
  },
  // SDLK_LEFTPAREN
  // SDLK_RIGHTPAREN
  // SDLK_ASTERISK
  // SDLK_PLUS
  {
    .SDLK    = SDLK_COMMA,
    .key     = ',',
    .shift   = '<',
    .caps    = ',',
    .caps_sh = '<',
    .num     = ',',
    .num_sh  = '<',
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_COMMA
  }, {
    .SDLK    = SDLK_MINUS,
    .key     = '-',
    .shift   = '_',
    .caps    = '-',
    .caps_sh = '_',
    .num     = '-',
    .num_sh  = '_',
    .ctrl    = 0x1F,
    .alt     = kAltMin,
    .scan    = SC_MINUS
  }, {
    .SDLK    = SDLK_PERIOD,
    .key     = '.',
    .shift   = '>',
    .caps    = '.',
    .caps_sh = '>',
    .num     = '.',
    .num_sh  = '>',
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_PERIOD
  }, {
    .SDLK    = SDLK_SLASH,
    .key     = '/',
    .shift   = '?',
    .caps    = '/',
    .caps_sh = '?',
    .num     = '/',
    .num_sh  = '?',
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_SLASH
  }, {
    .SDLK    = SDLK_0,
    .key     = '0',
    .shift   = ')',
    .caps    = '0',
    .caps_sh = ')',
    .num     = '0',
    .num_sh  = ')',
    .ctrl    = kCtrl0,
    .alt     = kAlt0,
    .scan    = SC_0
  }, {
    .SDLK    = SDLK_1,
    .key     = '1',
    .shift   = '!',
    .caps    = '1',
    .caps_sh = '!',
    .num     = '1',
    .num_sh  = '!',
    .ctrl    = kCtrl1,
    .alt     = kAlt1,
    .scan    = SC_1
  }, {
    .SDLK    = SDLK_2,
    .key     = '2',
    .shift   = '@',
    .caps    = '2',
    .caps_sh = '@',
    .num     = '2',
    .num_sh  = '@',
    .ctrl    = kCtrl2,
    .alt     = kAlt2,
    .scan    = SC_2
  }, {
    .SDLK    = SDLK_3,
    .key     = '3',
    .shift   = '#',
    .caps    = '3',
    .caps_sh = '#',
    .num     = '3',
    .num_sh  = '#',
    .ctrl    = kCtrl3,
    .alt     = kAlt3,
    .scan    = SC_3
  }, {
    .SDLK    = SDLK_4,
    .key     = '4',
    .shift   = '$',
    .caps    = '4',
    .caps_sh = '$',
    .num     = '4',
    .num_sh  = '$',
    .ctrl    = kCtrl4,
    .alt     = kAlt4,
    .scan    = SC_4
  }, {
    .SDLK    = SDLK_5,
    .key     = '5',
    .shift   = '%',
    .caps    = '5',
    .caps_sh = '%',
    .num     = '5',
    .num_sh  = '%',
    .ctrl    = kCtrl5,
    .alt     = kAlt5,
    .scan    = SC_5
  }, {
    .SDLK    = SDLK_6,
    .key     = '6',
    .shift   = '^',
    .caps    = '6',
    .caps_sh = '^',
    .num     = '6',
    .num_sh  = '^',
    .ctrl    = kCtrl6,
    .alt     = kAlt6,
    .scan    = SC_6
  }, {
    .SDLK    = SDLK_7,
    .key     = '7',
    .shift   = '&',
    .caps    = '7',
    .caps_sh = '&',
    .num     = '7',
    .num_sh  = '&',
    .ctrl    = kCtrl7,
    .alt     = kAlt7,
    .scan    = SC_7
  }, {
    .SDLK    = SDLK_8,
    .key     = '8',
    .shift   = '*',
    .caps    = '8',
    .caps_sh = '*',
    .num     = '8',
    .num_sh  = '*',
    .ctrl    = kCtrl8,
    .alt     = kAlt8,
    .scan    = SC_8
  }, {
    .SDLK    = SDLK_9,
    .key     = '9',
    .shift   = '(',
    .caps    = '9',
    .caps_sh = '(',
    .num     = '9',
    .num_sh  = '(',
    .ctrl    = kCtrl9,
    .alt     = kAlt9,
    .scan    = SC_9
  },
  // SDLK_COLON
  {
    .SDLK    = SDLK_SEMICOLON,
    .key     = ';',
    .shift   = ':',
    .caps    = ';',
    .caps_sh = ':',
    .num     = ';',
    .num_sh  = ':',
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_SEMICOLON
  },
  // SDLK_LESS
  {
    .SDLK    = SDLK_EQUALS,
    .key     = '=',
    .shift   = '+',
    .caps    = '=',
    .caps_sh = '+',
    .num     = '=',
    .num_sh  = '+',
    .ctrl    = kNULL,
    .alt     = kAltEqu,
    .scan    = SC_EQUALS
  },
  // SDLK_GREATER
  // SDLK_QUESTION
  // SDLK_AT
  {
    .SDLK    = SDLK_LEFTBRACKET,
    .key     = '[',
    .shift   = '{',
    .caps    = '[',
    .caps_sh = '{',
    .num     = '[',
    .num_sh  = '{',
    .ctrl    = '\x1B',
    .alt     = kNULL,
    .scan    = SC_LEFTBRACKET
  }, {
    .SDLK    = SDLK_BACKSLASH,
    .key     = '\\',
    .shift   = '|',
    .caps    = '\\',
    .caps_sh = '|',
    .num     = '\\',
    .num_sh  = '|',
    .ctrl    = '\x1C',
    .alt     = kNULL,
    .scan    = SC_BACKSLASH
  }, {
    .SDLK    = SDLK_RIGHTBRACKET,
    .key     = ']',
    .shift   = '}',
    .caps    = ']',
    .caps_sh = '}',
    .num     = ']',
    .num_sh  = '}',
    .ctrl    = '\x1D',
    .alt     = kNULL,
    .scan    = SC_RIGHTBRACKET
  },
  // SDLK_CARET
  // SDLK_UNDERSCORE
  {
    .SDLK    = SDLK_BACKQUOTE,
    .key     = '`',
    .shift   = '~',
    .caps    = '`',
    .caps_sh = '~',
    .num     = '`',
    .num_sh  = '~',
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_BACKQUOTE
  }, {
    .SDLK    = SDLK_a,
    .key     = 'a',
    .shift   = 'A',
    .caps    = 'A',
    .caps_sh = 'a',
    .num     = 'a',
    .num_sh  = 'A',
    .ctrl    = 'A'-'A'+1,
    .alt     = kAltA,
    .scan    = SC_A
  }, {
    .SDLK    = SDLK_b,
    .key     = 'b',
    .shift   = 'B',
    .caps    = 'B',
    .caps_sh = 'b',
    .num     = 'b',
    .num_sh  = 'B',
    .ctrl    = 'B'-'A'+1,
    .alt     = kAltB,
    .scan    = SC_B
  }, {
    .SDLK    = SDLK_c,
    .key     = 'c',
    .shift   = 'C',
    .caps    = 'C',
    .caps_sh = 'c',
    .num     = 'c',
    .num_sh  = 'C',
    .ctrl    = 'C'-'A'+1,
    .alt     = kAltC,
    .scan    = SC_C
  }, {
    .SDLK    = SDLK_d,
    .key     = 'd',
    .shift   = 'D',
    .caps    = 'D',
    .caps_sh = 'd',
    .num     = 'd',
    .num_sh  = 'D',
    .ctrl    = 'D'-'A'+1,
    .alt     = kAltD,
    .scan    = SC_D
  }, {
    .SDLK    = SDLK_e,
    .key     = 'e',
    .shift   = 'E',
    .caps    = 'E',
    .caps_sh = 'e',
    .num     = 'e',
    .num_sh  = 'E',
    .ctrl    = 'E'-'A'+1,
    .alt     = kAltE,
    .scan    = SC_E
  }, {
    .SDLK    = SDLK_f,
    .key     = 'f',
    .shift   = 'F',
    .caps    = 'F',
    .caps_sh = 'f',
    .num     = 'f',
    .num_sh  = 'F',
    .ctrl    = 'F'-'A'+1,
    .alt     = kAltF,
    .scan    = SC_F
  }, {
    .SDLK    = SDLK_g,
    .key     = 'g',
    .shift   = 'G',
    .caps    = 'G',
    .caps_sh = 'g',
    .num     = 'g',
    .num_sh  = 'G',
    .ctrl    = 'G'-'A'+1,
    .alt     = kAltG,
    .scan    = SC_G
  }, {
    .SDLK    = SDLK_h,
    .key     = 'h',
    .shift   = 'H',
    .caps    = 'H',
    .caps_sh = 'h',
    .num     = 'h',
    .num_sh  = 'H',
    .ctrl    = 'H'-'A'+1,
    .alt     = kAltH,
    .scan    = SC_H
  }, {
    .SDLK    = SDLK_i,
    .key     = 'i',
    .shift   = 'I',
    .caps    = 'I',
    .caps_sh = 'i',
    .num     = 'i',
    .num_sh  = 'I',
    .ctrl    = 'I'-'A'+1,
    .alt     = kAltI,
    .scan    = SC_I
  }, {
    .SDLK    = SDLK_j,
    .key     = 'j',
    .shift   = 'J',
    .caps    = 'J',
    .caps_sh = 'j',
    .num     = 'j',
    .num_sh  = 'J',
    .ctrl    = 'J'-'A'+1,
    .alt     = kAltJ,
    .scan    = SC_J
  }, {
    .SDLK    = SDLK_k,
    .key     = 'k',
    .shift   = 'K',
    .caps    = 'K',
    .caps_sh = 'k',
    .num     = 'k',
    .num_sh  = 'K',
    .ctrl    = 'K'-'A'+1,
    .alt     = kAltK,
    .scan    = SC_K
  }, {
    .SDLK    = SDLK_l,
    .key     = 'l',
    .shift   = 'L',
    .caps    = 'L',
    .caps_sh = 'l',
    .num     = 'l',
    .num_sh  = 'L',
    .ctrl    = 'L'-'A'+1,
    .alt     = kAltL,
    .scan    = SC_L
  }, {
    .SDLK    = SDLK_m,
    .key     = 'm',
    .shift   = 'M',
    .caps    = 'M',
    .caps_sh = 'm',
    .num     = 'm',
    .num_sh  = 'M',
    .ctrl    = 'M'-'A'+1,
    .alt     = kAltM,
    .scan    = SC_M
  }, {
    .SDLK    = SDLK_n,
    .key     = 'n',
    .shift   = 'N',
    .caps    = 'N',
    .caps_sh = 'n',
    .num     = 'n',
    .num_sh  = 'N',
    .ctrl    = 'N'-'A'+1,
    .alt     = kAltN,
    .scan    = SC_N
  }, {
    .SDLK    = SDLK_o,
    .key     = 'o',
    .shift   = 'O',
    .caps    = 'O',
    .caps_sh = 'o',
    .num     = 'o',
    .num_sh  = 'O',
    .ctrl    = 'O'-'A'+1,
    .alt     = kAltO,
    .scan    = SC_O
  }, {
    .SDLK    = SDLK_p,
    .key     = 'p',
    .shift   = 'P',
    .caps    = 'P',
    .caps_sh = 'p',
    .num     = 'p',
    .num_sh  = 'P',
    .ctrl    = 'P'-'A'+1,
    .alt     = kAltP,
    .scan    = SC_P
  }, {
    .SDLK    = SDLK_q,
    .key     = 'q',
    .shift   = 'Q',
    .caps    = 'Q',
    .caps_sh = 'q',
    .num     = 'q',
    .num_sh  = 'Q',
    .ctrl    = 'Q'-'A'+1,
    .alt     = kAltQ,
    .scan    = SC_Q
  }, {
    .SDLK    = SDLK_r,
    .key     = 'r',
    .shift   = 'R',
    .caps    = 'R',
    .caps_sh = 'r',
    .num     = 'r',
    .num_sh  = 'R',
    .ctrl    = 'R'-'A'+1,
    .alt     = kAltR,
    .scan    = SC_R
  }, {
    .SDLK    = SDLK_s,
    .key     = 's',
    .shift   = 'S',
    .caps    = 'S',
    .caps_sh = 's',
    .num     = 's',
    .num_sh  = 'S',
    .ctrl    = 'S'-'A'+1,
    .alt     = kAltS,
    .scan    = SC_S
  }, {
    .SDLK    = SDLK_t,
    .key     = 't',
    .shift   = 'T',
    .caps    = 'T',
    .caps_sh = 't',
    .num     = 't',
    .num_sh  = 'T',
    .ctrl    = 'T'-'A'+1,
    .alt     = kAltT,
    .scan    = SC_T
  }, {
    .SDLK    = SDLK_u,
    .key     = 'u',
    .shift   = 'U',
    .caps    = 'U',
    .caps_sh = 'u',
    .num     = 'u',
    .num_sh  = 'U',
    .ctrl    = 'U'-'A'+1,
    .alt     = kAltU,
    .scan    = SC_U
  }, {
    .SDLK    = SDLK_v,
    .key     = 'v',
    .shift   = 'V',
    .caps    = 'V',
    .caps_sh = 'v',
    .num     = 'v',
    .num_sh  = 'V',
    .ctrl    = 'V'-'A'+1,
    .alt     = kAltV,
    .scan    = SC_V
  }, {
    .SDLK    = SDLK_w,
    .key     = 'w',
    .shift   = 'W',
    .caps    = 'W',
    .caps_sh = 'w',
    .num     = 'w',
    .num_sh  = 'W',
    .ctrl    = 'W'-'A'+1,
    .alt     = kAltW,
    .scan    = SC_W
  }, {
    .SDLK    = SDLK_x,
    .key     = 'x',
    .shift   = 'X',
    .caps    = 'X',
    .caps_sh = 'x',
    .num     = 'x',
    .num_sh  = 'X',
    .ctrl    = 'X'-'A'+1,
    .alt     = kAltX,
    .scan    = SC_X
  }, {
    .SDLK    = SDLK_y,
    .key     = 'y',
    .shift   = 'Y',
    .caps    = 'Y',
    .caps_sh = 'y',
    .num     = 'y',
    .num_sh  = 'Y',
    .ctrl    = 'Y'-'A'+1,
    .alt     = kAltY,
    .scan    = SC_Y
  }, {
    .SDLK    = SDLK_z,
    .key     = 'z',
    .shift   = 'Z',
    .caps    = 'Z',
    .caps_sh = 'z',
    .num     = 'z',
    .num_sh  = 'Z',
    .ctrl    = 'Z'-'A'+1,
    .alt     = kAltZ,
    .scan    = SC_Z
  }, {
    .SDLK    = SDLK_DELETE,
    .key     = kDELETE,
    .shift   = kDELETE,
    .caps    = kDELETE,
    .caps_sh = kDELETE,
    .num     = kDELETE,
    .num_sh  = kDELETE,
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_DELETE
  },
  // SDLK_WORLD_* (0-95)
  {
    .SDLK    = SDLK_KP0,
    .key     = kINSERT,
    .shift   = kINSERT,
    .caps    = '0',
    .caps_sh = ')',
    .num     = '0',
    .num_sh  = '0',
    .ctrl    = kCtNP0,
    .alt     = kNULL,
    .scan    = SC_NP_0
  }, {
    .SDLK    = SDLK_KP1,
    .key     = kEND,
    .shift   = kEND,
    .caps    = '1',
    .caps_sh = '!',
    .num     = '1',
    .num_sh  = '1',
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_NP_1
  }, {
    .SDLK    = SDLK_KP2,
    .key     = kDOWN,
    .shift   = kDOWN,
    .caps    = '2',
    .caps_sh = '@',
    .num     = '2',
    .num_sh  = '2',
    .ctrl    = kCtNP2,
    .alt     = kNULL,
    .scan    = SC_NP_2
  }, {
    .SDLK    = SDLK_KP3,
    .key     = kPgDOWN,
    .shift   = kPgDOWN,
    .caps    = '3',
    .caps_sh = '#',
    .num     = '3',
    .num_sh  = '3',
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_NP_3
  }, {
    .SDLK    = SDLK_KP4,
    .key     = kLEFT,
    .shift   = kLEFT,
    .caps    = '4',
    .caps_sh = '$',
    .num     = '4',
    .num_sh  = '4',
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_NP_4
  }, {
    .SDLK    = SDLK_KP5,
    .key     = kNP5,
    .shift   = kNP5,
    .caps    = '5',
    .caps_sh = '%',
    .num     = '5',
    .num_sh  = '5',
    .ctrl    = kCtNP5,
    .alt     = kNULL,
    .scan    = SC_NP_5
  }, {
    .SDLK    = SDLK_KP6,
    .key     = kRIGHT,
    .shift   = kRIGHT,
    .caps    = '6',
    .caps_sh = '^',
    .num     = '6',
    .num_sh  = '6',
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_NP_6
  }, {
    .SDLK    = SDLK_KP7,
    .key     = kHOME,
    .shift   = kHOME,
    .caps    = '7',
    .caps_sh = '&',
    .num     = '7',
    .num_sh  = '7',
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_NP_7
  }, {
    .SDLK    = SDLK_KP8,
    .key     = kUP,
    .shift   = kUP,
    .caps    = '8',
    .caps_sh = '*',
    .num     = '8',
    .num_sh  = '8',
    .ctrl    = kCtNP8,
    .alt     = kNULL,
    .scan    = SC_NP_8
  }, {
    .SDLK    = SDLK_KP9,
    .key     = kPgUP,
    .shift   = kPgUP,
    .caps    = '9',
    .caps_sh = '(',
    .num     = '9',
    .num_sh  = '9',
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    =  SC_NP_9
  }, {
    .SDLK    = SDLK_KP_PERIOD,
    .key     = kDELETE,
    .shift   = kDELETE,
    .caps    = '.',
    .caps_sh = '>',
    .num     = '.',
    .num_sh  = '>',
    .ctrl    = kCtNPper,
    .alt     = kNULL,
    .scan    = SC_NP_PERIOD
  }, {
    .SDLK    = SDLK_KP_DIVIDE,
    .key     = '/',
    .shift   = '/',
    .caps    = '/',
    .caps_sh = '/',
    .num     = '/',
    .num_sh  = '/',
    .ctrl    = kCtNPdiv,
    .alt     = kNULL,
    .scan    = SC_NP_DIVIDE
  }, {
    .SDLK    = SDLK_KP_MULTIPLY,
    .key     = '*',
    .shift   = '*',
    .caps    = '*',
    .caps_sh = '*',
    .num     = '*',
    .num_sh  = '*',
    .ctrl    = kCtNPmul,
    .alt     = kNULL,
    .scan    = SC_NP_MULTIPLY
  }, {
    .SDLK    = SDLK_KP_MINUS,
    .key     = '-',
    .shift   = '-',
    .caps    = '-',
    .caps_sh = '-',
    .num     = '-',
    .num_sh  = '-',
    .ctrl    = kCtNPmin,
    .alt     = kNULL,
    .scan    = SC_NP_MINUS
  }, {
    .SDLK    = SDLK_KP_PLUS,
    .key     = '+',
    .shift   = '+',
    .caps    = '+',
    .caps_sh = '+',
    .num     = '+',
    .num_sh  = '+',
    .ctrl    = kCtNPpls,
    .alt     = kNULL,
    .scan    = SC_NP_PLUS
  }, {
    .SDLK    = SDLK_KP_ENTER,
    .key     = '\r',
    .shift   = '\r',
    .caps    = '\r',
    .caps_sh = '\n',
    .num     = '\r',
    .num_sh  = '\n',
    .ctrl    = '\n',
    .alt     = kNULL,
    .scan    = SC_NP_ENTER
  },
  // SDLK_KP_EQUALS
  {
    .SDLK    = SDLK_UP,
    .key     = 0x0000,
    .shift   = kUP,
    .caps    = kUP,
    .caps_sh = kUP,
    .num     = kUP,
    .num_sh  = kUP,
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_UP
  }, {
    .SDLK    = SDLK_DOWN,
    .key     = 0x0000,
    .shift   = kDOWN,
    .caps    = kDOWN,
    .caps_sh = kDOWN,
    .num     = kDOWN,
    .num_sh  = kDOWN,
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_DOWN
  }, {
    .SDLK    = SDLK_RIGHT,
    .key     = kRIGHT,
    .shift   = kRIGHT,
    .caps    = kRIGHT,
    .caps_sh = kRIGHT,
    .num     = kRIGHT,
    .num_sh  = kRIGHT,
    .ctrl    = kCtRGHT,
    .alt     = kNULL,
    .scan    = SC_RIGHT
  }, {
    .SDLK    = SDLK_LEFT,
    .key     = kLEFT,
    .shift   = kLEFT,
    .caps    = kLEFT,
    .caps_sh = kLEFT,
    .num     = kLEFT,
    .num_sh  = kLEFT,
    .ctrl    = kCtLEFT,
    .alt     = kNULL,
    .scan    = SC_LEFT
  }, {
    .SDLK    = SDLK_INSERT,
    .key     = kINSERT,
    .shift   = kINSERT,
    .caps    = kINSERT,
    .caps_sh = kINSERT,
    .num     = kINSERT,
    .num_sh  = kINSERT,
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_INSERT
  }, {
    .SDLK    = SDLK_HOME,
    .key     = kHOME,
    .shift   = kHOME,
    .caps    = kHOME,
    .caps_sh = kHOME,
    .num     = kHOME,
    .num_sh  = kHOME,
    .ctrl    = kCtHOME,
    .alt     = kAltHOME,
    .scan    = SC_HOME
  }, {
    .SDLK    = SDLK_END,
    .key     = kEND,
    .shift   = kEND,
    .caps    = kEND,
    .caps_sh = kEND,
    .num     = kEND,
    .num_sh  = kEND,
    .ctrl    = kCtEND,
    .alt     = kNULL,
    .scan    = SC_END
  }, {
    .SDLK    = SDLK_PAGEUP,
    .key     = kPgUP,
    .shift   = kPgUP,
    .caps    = kPgUP,
    .caps_sh = kPgUP,
    .num     = kPgUP,
    .num_sh  = kPgUP,
    .ctrl    = kCtPgUP,
    .alt     = kNULL,
    .scan    = SC_PAGEUP
  }, {
    .SDLK    = SDLK_PAGEDOWN,
    .key     = kPgDOWN,
    .shift   = kPgDOWN,
    .caps    = kPgDOWN,
    .caps_sh = kPgDOWN,
    .num     = kPgDOWN,
    .num_sh  = kPgDOWN,
    .ctrl    = kCtPgDN,
    .alt     = kNULL,
    .scan    = SC_PAGEDOWN
  }, {
    .SDLK    = SDLK_F1,
    .key     = kF1,
    .shift   = kShF1,
    .caps    = kF1,
    .caps_sh = kShF1,
    .num     = kF1,
    .num_sh  = kShF1,
    .ctrl    = kCtrlF1,
    .alt     = kAltF1,
    .scan    = SC_F1
  }, {
    .SDLK    = SDLK_F2,
    .key     = kF2,
    .shift   = kShF2,
    .caps    = kF2,
    .caps_sh = kShF2,
    .num     = kF2,
    .num_sh  = kShF2,
    .ctrl    = kCtrlF2,
    .alt     = kAltF2,
    .scan    = SC_F2
  }, {
    .SDLK    = SDLK_F3,
    .key     = kF3,
    .shift   = kShF3,
    .caps    = kF3,
    .caps_sh = kShF3,
    .num     = kF3,
    .num_sh  = kShF3,
    .ctrl    = kCtrlF3,
    .alt     = kAltF3,
    .scan    = SC_F3
  }, {
    .SDLK    = SDLK_F4,
    .key     = kF4,
    .shift   = kShF4,
    .caps    = kF4,
    .caps_sh = kShF4,
    .num     = kF4,
    .num_sh  = kShF4,
    .ctrl    = kCtrlF4,
    .alt     = kAltF4,
    .scan    = SC_F4
  }, {
    .SDLK    = SDLK_F5,
    .key     = kF5,
    .shift   = kShF5,
    .caps    = kF5,
    .caps_sh = kShF5,
    .num     = kF5,
    .num_sh  = kShF5,
    .ctrl    = kCtrlF5,
    .alt     = kAltF5,
    .scan    = SC_F5
  }, {
    .SDLK    = SDLK_F6,
    .key     = kF6,
    .shift   = kShF6,
    .caps    = kF6,
    .caps_sh = kShF6,
    .num     = kF6,
    .num_sh  = kShF6,
    .ctrl    = kCtrlF6,
    .alt     = kAltF6,
    .scan    = SC_F6
  }, {
    .SDLK    = SDLK_F7,
    .key     = kF7,
    .shift   = kShF7,
    .caps    = kF7,
    .caps_sh = kShF7,
    .num     = kF7,
    .num_sh  = kShF7,
    .ctrl    = kCtrlF7,
    .alt     = kAltF7,
    .scan    = SC_F7
  }, {
    .SDLK    = SDLK_F8,
    .key     = kF8,
    .shift   = kShF8,
    .caps    = kF8,
    .caps_sh = kShF8,
    .num     = kF8,
    .num_sh  = kShF8,
    .ctrl    = kCtrlF8,
    .alt     = kAltF8,
    .scan    = SC_F8
  }, {
    .SDLK    = SDLK_F9,
    .key     = kF9,
    .shift   = kShF9,
    .caps    = kF9,
    .caps_sh = kShF9,
    .num     = kF9,
    .num_sh  = kShF9,
    .ctrl    = kCtrlF9,
    .alt     = kAltF9,
    .scan    = SC_F9
  }, {
    .SDLK    = SDLK_F10,
    .key     = kF10,
    .shift   = kShF10,
    .caps    = kF10,
    .caps_sh = kShF10,
    .num     = kF10,
    .num_sh  = kShF10,
    .ctrl    = kCtrlF10,
    .alt     = kAltF10,
    .scan    = SC_F10
  }, {
    .SDLK    = SDLK_F11,
    .key     = kF11,
    .shift   = kShF11,
    .caps    = kF11,
    .caps_sh = kShF11,
    .num     = kF11,
    .num_sh  = kShF11,
    .ctrl    = kCtrlF11,
    .alt     = kAltF11,
    .scan    = SC_F11
  }, {
    .SDLK    = SDLK_F12,
    .key     = kF12,
    .shift   = kShF12,
    .caps    = kF12,
    .caps_sh = kShF12,
    .num     = kF12,
    .num_sh  = kShF12,
    .ctrl    = kCtrlF12,
    .alt     = kAltF12,
    .scan    = SC_F12
  },
  // SDLK_F13 => SC_F13
  // SDLK_F14 => SC_F14
  // SDLK_F15 => SC_F15
  {
    .SDLK    = SDLK_NUMLOCK,
    .key     = kUNKN,
    .shift   = kUNKN,
    .caps    = kUNKN,
    .caps_sh = kUNKN,
    .num     = kUNKN,
    .num_sh  = kUNKN,
    .ctrl    = kUNKN,
    .alt     = kUNKN,
    .scan    = SC_NUMLOCK
  }, {
    .SDLK    = SDLK_CAPSLOCK,
    .key     = kUNKN,
    .shift   = kUNKN,
    .caps    = kUNKN,
    .caps_sh = kUNKN,
    .num     = kUNKN,
    .num_sh  = kUNKN,
    .ctrl    = kUNKN,
    .alt     = kUNKN,
    .scan    = SC_CAPSLOCK
  }, {
    .SDLK    = SDLK_SCROLLOCK,
    .key     = kUNKN,
    .shift   = kUNKN,
    .caps    = kUNKN,
    .caps_sh = kUNKN,
    .num     = kUNKN,
    .num_sh  = kUNKN,
    .ctrl    = kUNKN,
    .alt     = kUNKN,
    .scan    = SC_SCROLLLOCK
  }, {
    .SDLK    = SDLK_RSHIFT,
    .key     = kUNKN,
    .shift   = kUNKN,
    .caps    = kUNKN,
    .caps_sh = kUNKN,
    .num     = kUNKN,
    .num_sh  = kUNKN,
    .ctrl    = kUNKN,
    .alt     = kUNKN,
    .scan    = SC_RSHIFT
  }, {
    .SDLK    = SDLK_LSHIFT,
    .key     = kUNKN,
    .shift   = kUNKN,
    .caps    = kUNKN,
    .caps_sh = kUNKN,
    .num     = kUNKN,
    .num_sh  = kUNKN,
    .ctrl    = kUNKN,
    .alt     = kUNKN,
    .scan    = SC_LSHIFT
  }, {
    .SDLK    = SDLK_RCTRL,
    .key     = kUNKN,
    .shift   = kUNKN,
    .caps    = kUNKN,
    .caps_sh = kUNKN,
    .num     = kUNKN,
    .num_sh  = kUNKN,
    .ctrl    = kUNKN,
    .alt     = kUNKN,
    .scan    = SC_RCTRL // SC_RCTRL_REAL
  }, {
    .SDLK    = SDLK_LCTRL,
    .key     = kUNKN,
    .shift   = kUNKN,
    .caps    = kUNKN,
    .caps_sh = kUNKN,
    .num     = kUNKN,
    .num_sh  = kUNKN,
    .ctrl    = kUNKN,
    .alt     = kUNKN,
    .scan    = SC_LCTRL // SC_LCTRL_REAL
  }, {
    .SDLK    = SDLK_RALT,
    .key     = kUNKN,
    .shift   = kUNKN,
    .caps    = kUNKN,
    .caps_sh = kUNKN,
    .num     = kUNKN,
    .num_sh  = kUNKN,
    .ctrl    = kUNKN,
    .alt     = kUNKN,
    .scan    = SC_RALT // SC_RALT_REAL
  }, {
    .SDLK    = SDLK_LALT,
    .key     = kUNKN,
    .shift   = kUNKN,
    .caps    = kUNKN,
    .caps_sh = kUNKN,
    .num     = kUNKN,
    .num_sh  = kUNKN,
    .ctrl    = kUNKN,
    .alt     = kUNKN,
    .scan    = SC_LALT // SC_LALT_REAL
  },
  // SDLK_RMETA
  // SDLK_LMETA
  // SDLK_LSUPER
  // SDLK_RSUPER
  // SDLK_MODE
  // SDLK_COMPOSE
  // SDLK_HELP
  {
    .SDLK    = SDLK_PRINT,
    .key     = '*',
    .shift   = kNULL,
    .caps    = '*',
    .caps_sh = kNULL,
    .num     = '*',
    .num_sh  = kNULL,
    .ctrl    = kNULL,
    .alt     = kNULL,
    .scan    = SC_PRTSCR
  }
  // SDLK_SYSREQ => SC_SYSRQ
  // SDLK_BREAK => SC_BREAK
  // SDLK_MENU => SC_MENU
  // SDLK_POWER
  // SDLK_EURO
  // SDLK_UNDO
};
