// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef FONT_H
#define FONT_H

#include "defines.h"
#include <stdint.h>

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

extern uint8_t font_height;

extern struct charmap_t
{
  uint8_t nul;                              // 0x00 U+0000 NUL,                                                 CP437 '\x00'
  uint8_t one_op_left_half;                 // 0x01 '1(' left half of "1OP"
  uint8_t two_op_left_half;                 // 0x02 '2(' left half of "2OP"
  uint8_t op_right_half;                    // 0x03 ')P' right half of "1OP", "2OP" and "4OP"
  uint8_t four_op_left_half;                // 0x04 '4(' left half of "4OP"
  uint8_t point_two;                        // 0x05 '.2' point two (two tenths in decimal, 1/5)
  uint8_t point_five;                       // 0x06 '.5' point five (five tenths in decimal, 1/2)
  // x07
  // x08
  // x09
  uint8_t up_down_arrow;                    // 0x0A U+2195 '↕' UP DOWN ARROW,                                   CP437 '\x12'
  uint8_t black_circle;                     // 0x0B U+25CF '●' BLACK CIRCLE
  uint8_t eighth_note_off;                  // 0x0C crossed-out eighth note (~U+266A '♪' EIGHTH NOTE, CP437 '\x0D' but crossed-out)
  uint8_t eighth_note;                      // 0x0D U+266A '♪' EIGHTH NOTE,                                     CP437 '\x0D'
  // x0E
  // x0F
  uint8_t black_right_triangle;             // 0x10 U+25BA '►' BLACK RIGHT-POINTING POINTER,                    CP437 '\x10'
  uint8_t black_left_triangle;              // 0x11 U+25C4 '◄' BLACK LEFT-POINTING POINTER,                     CP437 '\x11'
  // x12
  uint8_t loop_flag;                        // 0x13 loop flag (~U+21BB '↻' CLOCKWISE OPEN CIRCLE ARROW but rotated and with rectangular corners)
  // x14
  uint8_t pan_left;                         // 0x15 panning left (~U+0028 '(' LEFT PARENTHESIS, CP437 '\x28' but doubled)
  uint8_t pan_right;                        // 0x16 panning right (~U+0029 ')' RIGHT PARENTHESIS, CP437 '\x29' but doubled)
  // x17
  uint8_t up_arrow;                         // 0x18 U+2191 '↑' UPWARDS ARROW,                                   CP437 '\x18'
  uint8_t down_arrow;                       // 0x19 U+2193 '↓' DOWNWARDS ARROW,                                 CP437 '\x19'
  uint8_t right_arrow;                      // 0x1A U+2192 '→' RIGHTWARDS ARROW,                                CP437 '\x1A'
  uint8_t left_arrow;                       // 0x1B U+2190 '←' LEFTWARDS ARROW,                                 CP437 '\x1B'
  uint8_t black_small_circle;               // 0x1C U+2022 '•' BULLET (BLACK SMALL CIRCLE),                     CP437 '\x07'
  uint8_t floppy_disk;                      // 0x1D floppy disk (~U+1F5AB '🖫' WHITE HARD SHELL FLOPPY DISK but rotated 180 degrees)
  uint8_t black_up_triangle;                // 0x1E U+25B2 '▲' BLACK UP-POINTING TRIANGLE,                      CP437 '\x1E'
  uint8_t black_down_triangle;              // 0x1F U+25BC '▼' BLACK DOWN-POINTING TRIANGLE,                    CP437 '\x1F'
  // x20..x7E standard ASCII
  // x7F
  // x80..x8E vertical lower filled blocks in ascending order (1/16..15/16)
  // x8F..x9D vertical upper filled blocks in descending order (15/16..1/16)
  // x9E
  // x9F
  uint8_t perc_bass_drum;                   // 0xA0 'BD' percussion: bass drum
  uint8_t perc_snare_drum;                  // 0xA1 'SD' percussion: snare drum
  uint8_t perc_tom_tom;                     // 0xA2 'TT' percussion: tom-tom (tom drum)
  uint8_t perc_top_cymbal;                  // 0xA3 'TC' percussion: top cymbal
  uint8_t perc_hi_hat;                      // 0xA4 'HH' percussion: hi-hat
  // xA5..xA8 decay bars: horizontal left filled blocks in ascending order (1/4..4/4) on a background
  // xA9      decay bars: horizontal full filled block (4/4) on a background #2
  // xAA
  // xAB
  uint8_t four_op_upper_half;               // 0xAC Upper half is '┌', lower half is '4'
  uint8_t four_op_lower_half;               // 0xAD Upper half is 'op', lower half is '└'
  uint8_t hertz;                            // 0xAE 'Hz' unit of frequency
  // xAF
  uint8_t light_shade;                      // 0xB0 U+2591 '░' LIGHT SHADE (25%),                               CP437 '\xB0'
  uint8_t medium_shade;                     // 0xB1 U+2592 '▒' MEDIUM SHADE (50%),                              CP437 '\xB1'
  uint8_t dark_shade;                       // 0xB2 U+2593 '▓' DARK SHADE (75%),                                CP437 '\xB2'
  uint8_t bd_light_vert;                    // 0xB3 U+2502 '│' BOX DRAWINGS LIGHT VERTICAL,                     CP437 '\xB3'
  uint8_t bd_light_vert_and_left;           // 0xB4 U+2524 '┤' BOX DRAWINGS LIGHT VERTICAL AND LEFT,            CP437 '\xB4'
  uint8_t bd_vert_light_and_left_heavy;     // 0xB5 U+2525 '┥' BOX DRAWINGS VERTICAL LIGHT AND LEFT HEAVY       in place of U+2561 '╡', CP437 '\xB5'
  uint8_t bd_vert_heavy_and_left_light;     // 0xB6 U+2528 '┨' BOX DRAWINGS VERTICAL HEAVY AND LEFT LIGHT       in place of U+2562 '╢', CP437 '\xB6'
  uint8_t bd_down_heavy_and_left_light;     // 0xB7 U+2512 '┒' BOX DRAWINGS DOWN HEAVY AND LEFT LIGHT           in place of U+2556 '╖', CP437 '\xB7'
  uint8_t bd_down_light_and_left_heavy;     // 0xB8 U+2511 '┑' BOX DRAWINGS DOWN LIGHT AND LEFT HEAVY           in place of U+2555 '╕', CP437 '\xB8'
  uint8_t bd_heavy_vert_and_left;           // 0xB9 U+252B '┫' BOX DRAWINGS HEAVY VERTICAL AND LEFT             in place of U+2563 '╣', CP437 '\xB9'
  uint8_t bd_heavy_vert;                    // 0xBA U+2503 '┃' BOX DRAWINGS HEAVY VERTICAL                      in place of U+2551 '║', CP437 '\xBA'
  uint8_t bd_heavy_down_and_left;           // 0xBB U+2513 '┓' BOX DRAWINGS HEAVY DOWN AND LEFT                 in place of U+2557 '╗', CP437 '\xBB'
  uint8_t bd_heavy_up_and_left;             // 0xBC U+251B '┛' BOX DRAWINGS HEAVY UP AND LEFT                   in place of U+255D '╝', CP437 '\xBC'
  uint8_t bd_up_heavy_and_left_light;       // 0xBD U+251A '┚' BOX DRAWINGS UP HEAVY AND LEFT LIGHT             in place of U+255C '╜', CP437 '\xBD'
  uint8_t bd_up_light_and_left_heavy;       // 0xBE U+2519 '┙' BOX DRAWINGS UP LIGHT AND LEFT HEAVY             in place of U+255B '╛', CP437 '\xBE'
  uint8_t bd_light_down_and_left;           // 0xBF U+2510 '┐' BOX DRAWINGS LIGHT DOWN AND LEFT,                CP437 '\xBF'
  uint8_t bd_light_up_and_right;            // 0xC0 U+2514 '└' BOX DRAWINGS LIGHT UP AND RIGHT,                 CP437 '\xC0'
  uint8_t bd_light_up_and_horiz;            // 0xC1 U+2534 '┴' BOX DRAWINGS LIGHT UP AND HORIZONTAL,            CP437 '\xC1'
  uint8_t bd_light_down_and_horiz;          // 0xC2 U+252C '┬' BOX DRAWINGS LIGHT DOWN AND HORIZONTAL,          CP437 '\xC2'
  uint8_t bd_light_vert_and_right;          // 0xC3 U+251C '├' BOX DRAWINGS LIGHT VERTICAL AND RIGHT,           CP437 '\xC3'
  uint8_t bd_light_horiz;                   // 0xC4 U+2500 '─' BOX DRAWINGS LIGHT HORIZONTAL,                   CP437 '\xC4'
  uint8_t bd_light_vert_and_horiz;          // 0xC5 U+253C '┼' BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL,      CP437 '\xC5'
  uint8_t bd_vert_light_and_right_heavy;    // 0xC6 U+251D '┝' BOX DRAWINGS VERTICAL LIGHT AND RIGHT HEAVY      in place of U+255E '╞', CP437 '\xC6'
  uint8_t bd_vert_heavy_and_right_light;    // 0xC7 U+2520 '┠' BOX DRAWINGS VERTICAL HEAVY AND RIGHT LIGHT      in place of U+255F '╟', CP437 '\xC7'
  uint8_t bd_heavy_up_and_right;            // 0xC8 U+2517 '┗' BOX DRAWINGS HEAVY UP AND RIGHT                  in place of U+255A '╚', CP437 '\xC8'
  uint8_t bd_heavy_down_and_right;          // 0xC9 U+250F '┏' BOX DRAWINGS HEAVY DOWN AND RIGHT                in place of U+2554 '╔', CP437 '\xC9'
  uint8_t bd_heavy_up_and_horiz;            // 0xCA U+253B '┻' BOX DRAWINGS HEAVY UP AND HORIZONTAL             in place of U+2569 '╩', CP437 '\xCA'
  uint8_t bd_heavy_down_and_horiz;          // 0xCB U+2533 '┳' BOX DRAWINGS HEAVY DOWN AND HORIZONTAL           in place of U+2566 '╦', CP437 '\xCB'
  uint8_t bd_heavy_vert_and_right;          // 0xCC U+2523 '┣' BOX DRAWINGS HEAVY VERTICAL AND RIGHT            in place of U+2560 '╠', CP437 '\xCC'
  uint8_t bd_heavy_horiz;                   // 0xCD U+2501 '━' BOX DRAWINGS HEAVY HORIZONTAL                    in place of U+2550 '═', CP437 '\xCD'
  uint8_t bd_heavy_vert_and_horiz;          // 0xCE U+254B '╋' BOX DRAWINGS HEAVY VERTICAL AND HORIZONTAL       in place of U+256C '╬', CP437 '\xCE'
  uint8_t bd_up_light_and_horiz_heavy;      // 0xCF U+2537 '┷' BOX DRAWINGS UP LIGHT AND HORIZONTAL HEAVY       in place of U+2567 '╧', CP437 '\xCF'
  uint8_t bd_up_heavy_and_horiz_light;      // 0xD0 U+2538 '┸' BOX DRAWINGS UP HEAVY AND HORIZONTAL LIGHT       in place of U+2568 '╨', CP437 '\xD0'
  uint8_t bd_down_light_and_horiz_heavy;    // 0xD1 U+252F '┯' BOX DRAWINGS DOWN LIGHT AND HORIZONTAL HEAVY     in place of U+2564 '╤', CP437 '\xD1'
  uint8_t bd_down_heavy_and_horiz_light;    // 0xD2 U+2530 '┰' BOX DRAWINGS DOWN HEAVY AND HORIZONTAL LIGHT     in place of U+2565 '╥', CP437 '\xD2'
  uint8_t bd_up_heavy_and_right_light;      // 0xD3 U+2516 '┖' BOX DRAWINGS UP HEAVY AND RIGHT LIGHT            in place of U+2559 '╙', CP437 '\xD3'
  uint8_t bd_up_light_and_right_heavy;      // 0xD4 U+2515 '┕' BOX DRAWINGS UP LIGHT AND RIGHT HEAVY            in place of U+2558 '╘', CP437 '\xD4'
  uint8_t bd_down_light_and_right_heavy;    // 0xD5 U+250D '┍' BOX DRAWINGS DOWN LIGHT AND RIGHT HEAVY          in place of U+2552 '╒', CP437 '\xD5'
  uint8_t bd_down_heavy_and_right_light;    // 0xD6 U+250E '┎' BOX DRAWINGS DOWN HEAVY AND RIGHT LIGHT          in place of U+2553 '╓', CP437 '\xD6'
  uint8_t bd_vert_heavy_and_horiz_light;    // 0xD7 U+2542 '╂' BOX DRAWINGS VERTICAL HEAVY AND HORIZONTAL LIGHT in place of U+256B '╫', CP437 '\xD7'
  uint8_t bd_vert_light_and_horiz_heavy;    // 0xD8 U+253F '┿' BOX DRAWINGS VERTICAL LIGHT AND HORIZONTAL HEAVY in place of U+256A '╪', CP437 '\xD8'
  uint8_t bd_light_up_and_left;             // 0xD9 U+2518 '┘' BOX DRAWINGS LIGHT UP AND LEFT,                  CP437 '\xD9'
  uint8_t bd_light_down_and_right;          // 0xDA U+250C '┌' BOX DRAWINGS LIGHT DOWN AND RIGHT,               CP437 '\xDA'
  uint8_t full_block;                       // 0xDB U+2588 '█' FULL BLOCK,                                      CP437 '\xDB'
  uint8_t lower_half_block;                 // 0xDC U+2584 '▄' LOWER HALF BLOCK,                                CP437 '\xDC'
  uint8_t left_half_block;                  // 0xDD U+258C '▌' LEFT HALF BLOCK,                                 CP437 '\xDD'
  uint8_t right_half_block;                 // 0xDE U+2590 '▐' RIGHT HALF BLOCK,                                CP437 '\xDE'
  uint8_t upper_half_block;                 // 0xDF U+2580 '▀' UPPER HALF BLOCK,                                CP437 '\xDF'
  // xE0      background for vertical lower filled blocks
  // xE1..xEF vertical lower filled blocks in ascending order (1/16..15/16) on a background
  // xF0
  // xF1
  // xF2
  // xF3
  uint8_t first_part_of;                    // 0xF4 superscript '1/'
  uint8_t second_part_of;                   // 0xF5 superscript '2/'
  // xF6
  // xF7
  // xF8
  uint8_t small_zero;                       // 0xF9 small zero (~U+25E6 '◦' WHITE BULLET but shrunken horizontally)
  uint8_t middle_dot;                       // 0xFA U+00B7 '·' MIDDLE DOT,                                      CP437 '\xFA'
  uint8_t check_mark;                       // 0xFB U+2713 '✓' CHECK MARK                                       in place of U+221A '√', CP437 '\xFB'
  // xFC
  // xFD
  uint8_t black_square;                     // 0xFE U+25A0 '■' BLACK SQUARE,                                    CP437 '\xFE'
  uint8_t nbsp;                             // 0xFF U+00A0 ' ' NO-BREAK SPACE,                                  CP437 '\xFF'

  uint8_t vol_bar_right_steps;              // horizontal (rising right) volume bar's in-character steps (from 1 to character's width)
  uint8_t vol_bar_right_back;               // horizontal (rising right) volume bar's background
  uint8_t vol_bar_right_back2;              // horizontal (rising right) volume bar's background #2
  uint8_t vol_bar_right_tail[7];            // horizontal (rising right) volume bar's tails on background
  uint8_t vol_bar_right_full;               // horizontal (rising right) volume bar's full block on background
  uint8_t vol_bar_right_full2;              // horizontal (rising right) volume bar's full block on background #2
  uint8_t vol_bar_up_steps;                 // vertical (rising up) volume bar's in-character steps (from 1 to character's height)
  uint8_t vol_bar_up_back;                  // vertical (rising up) volume bar's background
  uint8_t vol_bar_up_tail[15];              // vertical (rising up) volume bar's tails on background
  uint8_t vol_bar_up_full;                  // vertical (rising up) volume bar's full block on background
}
charmap;

#pragma pack(pop)

#endif // !defined(FONT_H)
