//  This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef ADT2DATA_H
#define ADT2DATA_H

#include "defines.h"

extern unsigned char font8x16[4096];
extern unsigned char vga_font8x16[4096];
#if !GO32
extern unsigned char adt2_icon_bitmap[3126];
#endif // !GO32

#endif // !defined(ADT2DATA_H)
