// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include <string.h> // CopyString()
#include "parserio.h"

static uint16_t CRC16_table[0x100];
static uint32_t CRC32_table[0x100];

#if !ADT2PLAY
#include "parserio/Scan.c"
#endif // !ADT2PLAY
#include "parserio/SensitiveScan.c"
#if !ADT2PLAY
#include "parserio/Compare.c"
#include "parserio/Empty.c"
#include "parserio/CountLines.c"
#endif // !ADT2PLAY
#include "parserio/Update16.c"
#include "parserio/Update32.c"
#include "parserio/make_table_16bit.c"
#include "parserio/make_table_32bit.c"
