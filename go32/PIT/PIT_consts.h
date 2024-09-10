// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef GO32_PIT_CONSTS_H
#define GO32_PIT_CONSTS_H

#define PIT_FREQ_MAX    1193182 // Base HW frequency
// Minimal frequency is PIT_FREQ_MAX / PIT_LATENCY_MAX =~ 18.206512451
#define PIT_FREQ_FLOOR  18 // =floor(min)
#define PIT_FREQ_MIN    19 // =ceil(min)
#define PIT_LATENCY_MIN 1
#define PIT_LATENCY_MAX 0x10000

#endif // !defined(GO32_PIT_CONSTS_H)
