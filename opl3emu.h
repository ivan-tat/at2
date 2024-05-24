// This file is part of Adlib Tracker II (AT2).
//
// OPL3 EMULATOR
// Based on NukedOPL3 1.6 by Nuke.YKT (Alexey Khokholov)
// Special thanks to insane/Altair for initial C to Pascal conversion
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef OPL3EMU_H
#define OPL3EMU_H

#include <stdint.h>

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

void OPL3EMU_init (void);
void OPL3EMU_WriteReg (uint16_t reg, uint8_t data);
void OPL3EMU_PollProc (uint32_t *p_data, void *ch_table);

#endif // !defined(OPL3EMU_H)
