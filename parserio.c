// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "parserio.h"
#if USE_FPC
#include "pascal/string.h" // CopyString()
#endif // USE_FPC

static uint16_t CRC16_table[0x100];
static uint32_t CRC32_table[0x100];

#include "parserio/Scan.c"
#include "parserio/SensitiveScan.c"
#include "parserio/Compare.c"
#include "parserio/Empty.c"
#include "parserio/CountLines.c"
#include "parserio/Update16.c"
#include "parserio/Update32.c"
#include "parserio/make_table_16bit.c"
#include "parserio/make_table_32bit.c"
