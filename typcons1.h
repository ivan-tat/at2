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

#if GO32

extern uint8_t screen_mode;
extern uint8_t comp_text_mode;

#endif // GO32

extern uint8_t command_typing;

extern CharSet_t _valid_characters; // HINT: (FPC) set of char
extern CharSet_t _valid_characters_fname; // HINT: (FPC) set of char
