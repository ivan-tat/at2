//  This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"

#include "adt2data.h"

unsigned char font8x16[4096] = {
#include "font/track16.inc"
};

unsigned char vga_font8x16[4096] = {
#include "font/vga16.inc"
};

#if !GO32

unsigned char adt2_icon_bitmap[3126] = {
#include "adtrack2-icon.inc"
};

#endif // !GO32
