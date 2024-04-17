// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef GO32_PIT_H
#define GO32_PIT_H

// x86 Programmable Interval Timer (PIT)

#define PIT_IRQ 8 // Default hardware interrupt 0 vector number

#include "go32/PIT/PIT_consts.h"

#include "go32/PIT/PIT_disable_IRQ.c"
#include "go32/PIT/PIT_enable_IRQ.c"
#include "go32/PIT/PIT_set_latency.c"

#endif // !defined(GO32_PIT_H)
