// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "font.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

uint8_t font_height = 16;

struct charmap_t charmap =
{
  .nul                              = '\x00',
  .one_op_left_half                 = '\x01',
  .two_op_left_half                 = '\x02',
  .op_right_half                    = '\x03',
  .four_op_left_half                = '\x04',
  .point_two                        = '\x05',
  .point_five                       = '\x06',
  // x07
  // x08
  // x09
  .up_down_arrow                    = '\x0A',
  .black_circle                     = '\x0B',
  .eighth_note_off                  = '\x0C',
  .eighth_note                      = '\x0D',
  // x0E
  // x0F
  .black_right_triangle             = '\x10',
  .black_left_triangle              = '\x11',
  // x12
  .loop_flag                        = '\x13',
  // x14
  .pan_left                         = '\x15',
  .pan_right                        = '\x16',
  // x17
  .up_arrow                         = '\x18',
  .down_arrow                       = '\x19',
  .right_arrow                      = '\x1A',
  .left_arrow                       = '\x1B',
  .black_small_circle               = '\x1C',
  .floppy_disk                      = '\x1D',
  .black_up_triangle                = '\x1E',
  .black_down_triangle              = '\x1F',
  // x20..x7E
  // x7F
  // x80..x8E
  // x8F..x9D
  // x9E
  // x9F
  .perc_bass_drum                   = '\xA0',
  .perc_snare_drum                  = '\xA1',
  .perc_tom_tom                     = '\xA2',
  .perc_top_cymbal                  = '\xA3',
  .perc_hi_hat                      = '\xA4',
  // xA5..xA8
  // xA9
  // xAA
  // xAB
  // xAC
  // xAD
  .hertz                            = '\xAE',
  // xAF
  .light_shade                      = '\xB0',
  .medium_shade                     = '\xB1',
  .dark_shade                       = '\xB2',
  .bd_light_vert                    = '\xB3',
  .bd_light_vert_and_left           = '\xB4',
  .bd_vert_light_and_left_heavy     = '\xB5',
  .bd_vert_heavy_and_left_light     = '\xB6',
  .bd_down_heavy_and_left_light     = '\xB7',
  .bd_down_light_and_left_heavy     = '\xB8',
  .bd_heavy_vert_and_left           = '\xB9',
  .bd_heavy_vert                    = '\xBA',
  .bd_heavy_down_and_left           = '\xBB',
  .bd_heavy_up_and_left             = '\xBC',
  .bd_up_heavy_and_left_light       = '\xBD',
  .bd_up_light_and_left_heavy       = '\xBE',
  .bd_light_down_and_left           = '\xBF',
  .bd_light_up_and_right            = '\xC0',
  .bd_light_up_and_horiz            = '\xC1',
  .bd_light_down_and_horiz          = '\xC2',
  .bd_light_vert_and_right          = '\xC3',
  .bd_light_horiz                   = '\xC4',
  .bd_light_vert_and_horiz          = '\xC5',
  .bd_vert_light_and_right_heavy    = '\xC6',
  .bd_vert_heavy_and_right_light    = '\xC7',
  .bd_heavy_up_and_right            = '\xC8',
  .bd_heavy_down_and_right          = '\xC9',
  .bd_heavy_up_and_horiz            = '\xCA',
  .bd_heavy_down_and_horiz          = '\xCB',
  .bd_heavy_vert_and_right          = '\xCC',
  .bd_heavy_horiz                   = '\xCD',
  .bd_heavy_vert_and_horiz          = '\xCE',
  .bd_up_light_and_horiz_heavy      = '\xCF',
  .bd_up_heavy_and_horiz_light      = '\xD0',
  .bd_down_light_and_horiz_heavy    = '\xD1',
  .bd_down_heavy_and_horiz_light    = '\xD2',
  .bd_up_heavy_and_right_light      = '\xD3',
  .bd_up_light_and_right_heavy      = '\xD4',
  .bd_down_light_and_right_heavy    = '\xD5',
  .bd_down_heavy_and_right_light    = '\xD6',
  .bd_vert_heavy_and_horiz_light    = '\xD7',
  .bd_vert_light_and_horiz_heavy    = '\xD8',
  .bd_light_up_and_left             = '\xD9',
  .bd_light_down_and_right          = '\xDA',
  .full_block                       = '\xDB',
  .lower_half_block                 = '\xDC',
  .left_half_block                  = '\xDD',
  .right_half_block                 = '\xDE',
  .upper_half_block                 = '\xDF',
  // xE0
  // xE1..xEF
  // xF0
  // xF1
  // xF2
  // xF3
  .first_part_of                    = '\xF4',
  .second_part_of                   = '\xF5',
  // xF6
  // xF7
  // xF8
  .small_zero                       = '\xF9',
  .middle_dot                       = '\xFA',
  .check_mark                       = '\xFB',
  // xFC
  // xFD
  .black_square                     = '\xFE',
  .nbsp                             = '\xFF',

  .vol_bar_right_steps              = 4,
  .vol_bar_right_back               = '\xCD', // = `.bd_heavy_horiz'
  .vol_bar_right_back2              = '\xCF', // = `.bd_up_light_and_horiz_heavy'
  .vol_bar_right_tail               =
  {
    '\xA5', // 1/4 left filled block on `.vol_bar_right_back'
    '\xA6', // 2/4 left filled block on `.vol_bar_right_back'
    '\xA7', // 3/4 left filled block on `.vol_bar_right_back'
    0,
    0,
    0,
    0
  },
  .vol_bar_right_full               = '\xA8', // full block on `.vol_bar_right_back'
  .vol_bar_right_full2              = '\xA9', // full block on `.vol_bar_right_back2'
  .vol_bar_up_steps                 = 16,     // = `font_height'
  .vol_bar_up_back                  = '\xE0',
  .vol_bar_up_tail                  =
  {
    '\xE1', //  1/16 filled block on `.vol_bar_up_back'
    '\xE2', //  2/16 filled block on `.vol_bar_up_back'
    '\xE3', //  3/16 filled block on `.vol_bar_up_back'
    '\xE4', //  4/16 filled block on `.vol_bar_up_back'
    '\xE5', //  5/16 filled block on `.vol_bar_up_back'
    '\xE6', //  6/16 filled block on `.vol_bar_up_back'
    '\xE7', //  7/16 filled block on `.vol_bar_up_back'
    '\xE8', //  8/16 filled block on `.vol_bar_up_back'
    '\xE9', //  9/16 filled block on `.vol_bar_up_back'
    '\xEA', // 10/16 filled block on `.vol_bar_up_back'
    '\xEB', // 11/16 filled block on `.vol_bar_up_back'
    '\xEC', // 12/16 filled block on `.vol_bar_up_back'
    '\xED', // 13/16 filled block on `.vol_bar_up_back'
    '\xEE', // 14/16 filled block on `.vol_bar_up_back'
    '\xEF'  // 15/16 filled block on `.vol_bar_up_back'
  },
  .vol_bar_up_full                  = '\xDB'  // = `.full_block'
};

#pragma pack(pop)
