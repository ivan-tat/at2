// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef ADT2KEYB_H
#define ADT2KEYB_H

#include "defines.h"

#include <stdbool.h>
#include <stdint.h>

bool keypressed (void);
uint16_t getkey (void);
bool LookUpKey (uint16_t key, uint16_t *table, uint8_t size);

#if GO32

void keyboard_reset_buffer_alt (void);

#endif // GO32

#endif // !defined(ADT2KEYB_H)
