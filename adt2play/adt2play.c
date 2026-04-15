// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dpmi.h>
#include "platform.h"
#include "pascal.h"
#include "debug.h"
#include "go32/VBIOS.h"
#include "adt2opl3.h"
#include "adt2keyb.h"
#include "adt2ext3.h"
#include "txtscrio.h"
#include "a2scrio.h"

#include "adt2play.inc" // defines "VERSION"

static uint8_t orig_video_mode;
static uint8_t orig_video_page;

/*static*/ bool jukebox = false;
/*static*/ bool gfx_mode = false;
/*static*/ int16_t error_code;
/*static*/ int8_t loader_result;
/*static*/ char *loader_error;
/*static*/ ExtKeyCode fkey;

#include "adt2play/_list_title.c" // HINT: static
#include "adt2play/print_error.c" // static

typedef enum
{
  MODNAME_A2M_0,
  MODNAME_A2M_1,
  MODNAME_A2T_0,
  MODNAME_A2T_1,
  MODNAME_AMD_0,
  MODNAME_AMD_1,
  MODNAME_CFF,
  MODNAME_DFM,
  MODNAME_HSC,
  MODNAME_MTK,
  MODNAME_RAD,
  MODNAME_S3M,
  MODNAME_FMK,
  MODNAME_SAT_0,
  MODNAME_SAT_1,
  MODNAME_MAX
} modname_enum;

/*static*/ const char *modname[MODNAME_MAX] = // CP437
{
  [MODNAME_A2M_0] = "/\xB4""DLiB TR/\xB4""CK3R ][ module",          // "/┤DLiB TR/┤CK3R ][ module"
  [MODNAME_A2M_1] = "/\xB4""DLiB TR/\xB4""CK3R ][ G3 module",       // "/┤DLiB TR/┤CK3R ][ G3 module"
  [MODNAME_A2T_0] = "/\xB4""DLiB TR/\xB4""CK3R ][ tiny module",     // "/┤DLiB TR/┤CK3R ][ tiny module"
  [MODNAME_A2T_1] = "/\xB4""DLiB TR/\xB4""CK3R ][ G3 tiny module",  // "/┤DLiB TR/┤CK3R ][ G3 tiny module"
  [MODNAME_AMD_0] = "Amusic module",
  [MODNAME_AMD_1] = "XMS-Tracker module",
  [MODNAME_CFF]   = "BoomTracker 4.0 module",
  [MODNAME_DFM]   = "Digital-FM module",
  [MODNAME_HSC]   = "HSC AdLib Composer / HSC-Tracker module",
  [MODNAME_MTK]   = "MPU-401 tr\x92kk\xEEr module", // "MPU-401 trÆkkεr module"
  [MODNAME_RAD]   = "Reality ADlib Tracker module",
  [MODNAME_S3M]   = "Scream Tracker 3.x module",
  [MODNAME_FMK]   = "FM-Kingtracker module",
  [MODNAME_SAT_0] = "Surprise! AdLib Tracker module",
  [MODNAME_SAT_1] = "Surprise! AdLib Tracker 2.0 module"
};

#include "adt2play/load_song.c" // HINT: static
#include "adt2play/terminate.c" // HINT: static
#include "adt2play/done_adt2play.c" // static
#include "adt2play/run_adt2play.c" // main
