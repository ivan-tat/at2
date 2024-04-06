// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "a2data.h"

const unsigned char font8x8[2048] = {
#include "font/play8.inc"
};

const unsigned char font8x16[4096] = {
#include "font/play16.inc"
};

const unsigned char picture_palette[768] = {
#include "picture/palette.inc"
};

const unsigned char picture_bitmap[64000] = {
#include "picture/bitmap.inc"
};
