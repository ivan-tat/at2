// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#if GO32

uint8_t screen_mode = 0;
uint8_t comp_text_mode = 0;

#endif // GO32

uint8_t command_typing = 1;
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
