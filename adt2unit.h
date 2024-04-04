// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef ADT2UNIT_H
#define ADT2UNIT_H

#include "defines.h"

#include <stdint.h>
#include "pascal.h"

#if GO32
extern uint8_t ___ADT2UNIT_DATA_START___, ___ADT2UNIT_DATA_END___;
#endif // GO32

#include "typcons1.h"
#include "typcons2.h"

extern uint8_t pattord_page;
extern uint8_t pattord_hpos;
extern uint8_t pattord_vpos;
extern uint8_t instrum_page;
extern uint8_t pattern_patt;
extern uint8_t pattern_page;
extern uint8_t pattern_hpos;

#include "realtime.h"

int32_t min (int32_t value, int32_t minimum);
int32_t max (int32_t value, int32_t maximum);

#endif // !defined(ADT2UNIT_H)
