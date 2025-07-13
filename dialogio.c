// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"
#include "platform.h"
#include "pascal.h"
#if GO32
#include <dpmi.h>
#include <go32.h>
#elif WIN32 || WIN64 || WINNT
#include <windows.h>
#endif // !(WIN32 || WIN64 || WINNT)
#include <stdio.h>
#include "debug.h"
#include "adt2sys.h"
#include "adt2text.h"
#include "adt2unit.h"
#include "dialogio.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

#pragma pack(push, 1)

#include "dialogio/OutKey.c" // static, used in `Dialog' and `Menu'
#include "dialogio/ReadChunk.c" // static, used in `Dialog' and `Fselect'

/*** Dialog ***/

tDIALOG_SETTING dl_setting =
{
  .frame_type     = { 0, }, // frame_single
  .shadow_enabled = true,
  .title_attr     = 0x0F,
  .box_attr       = 0x07,
  .text_attr      = 0x07,
  .text2_attr     = 0x0F,
  .keys_attr      = 0x07,
  .keys2_attr     = 0x70,
  .short_attr     = 0x0F,
  .short2_attr    = 0x70,
  .disbld_attr    = 0x07,
  .contxt_attr    = 0x0F,
  .contxt2_attr   = 0x07,
  .xstart         = 1,
  .ystart         = 1,
  .center_box     = true,
  .center_text    = true,
  .cycle_moves    = true,
  .all_enabled    = false,
  .terminate_keys =
  {
    kESC,
    kENTER,
    0,
  }
};

tDIALOG_ENVIRONMENT dl_environment;

typedef struct
{
  String_t str;
  uint8_t pos;
  char key;
  bool use;
} tDBUFFR[100];

static struct
{
  uint16_t idx2;
  uint16_t idx3;
  uint16_t num;
  uint16_t nm2;
  uint16_t xstart;
  uint16_t ystart;
  String_t str;
  tDBUFFR  dbuf;
} _dlg;

#include "dialogio/Dialog_SubPos.c" // static
#include "dialogio/Dialog_AddPos.c" // static
#include "dialogio/Dialog_ShowItem.c" // static
#include "dialogio/Dialog_RetKey.c" // static
//#include "dialogio/Dialog_CurrentKey.c" // static, not used actually
#include "dialogio/Dialog.c"

/*** Menu ***/

tMENU_SETTING mn_setting =
{
  .frame_type     = { 0, }, // frame_single
  .frame_enabled  = true,
  .shadow_enabled = true,
  .posbar_enabled = true,
  .title_attr     = 0x0F,
  .menu_attr      = 0x07,
  .text_attr      = 0x07,
  .text2_attr     = 0x70,
  .default_attr   = 0x0F,
  .short_attr     = 0x0F,
  .short2_attr    = 0x70,
  .disbld_attr    = 0x07,
  .contxt_attr    = 0x0F,
  .contxt2_attr   = 0x07,
  .topic_attr     = 0x07,
  .hi_topic_attr  = 0x0F,
  .topic_mask_chr = { 0, },
  .center_box     = true,
  .cycle_moves    = true,
  .edit_contents  = false,
  .reverse_use    = false,
  .show_scrollbar = true,
  .topic_len      = 0,
  .fixed_len      = 0,
  .homing_pos     = 0,
  .terminate_keys =
  {
    kESC,
    kENTER,
    0,
  }
};

tMENU_ENVIRONMENT mn_environment;

typedef struct
{
  char key;
  bool use;
} tMBUFFR[16384];

static struct
{
  uint16_t idx;
  uint16_t idx2;
  uint16_t max;
  uint16_t page;
  uint16_t first;
  uint16_t last;
  uint16_t opage;
  uint16_t opos;
  bool     solid;
  uint8_t  x;
  uint8_t  y;
  uint8_t  len;
  uint8_t  len2;
  uint8_t  topic_len;
  void    *data;
  uint16_t count;
  uint16_t vscrollbar_size;
  uint16_t vscrollbar_pos;
  tMBUFFR  mbuf;
} _mnu;

#include "dialogio/Menu_pstr.c" // static
#include "dialogio/Menu_pstr2.c" // static
#include "dialogio/Menu_pdes.c" // static
#include "dialogio/Menu_ShowCStr.c" // static
#include "dialogio/Menu_refresh.c" // static
#include "dialogio/Menu_SubPos.c" // static
#include "dialogio/Menu_AddPos.c" // static
#include "dialogio/Menu_RetKey.c" // static
#include "dialogio/Menu_edit_contents.c" // static
#include "dialogio/Menu.c"

/*** Menu 1 ***/

tMENU1_SETTING mn1_setting =
{
  .frame_type     = { 0, }, // frame_single
  .shadow_enabled = true,
  .posbar_enabled = true,
  .title_attr     = 0x0F,
  .menu_attr      = 0x07,
  .text_attr      = 0x07,
  .text2_attr     = 0x70,
  .default_attr   = 0x0F,
  .short_attr     = 0x0F,
  .short2_attr    = 0x70,
  .disbld_attr    = 0x07,
  .contxt_attr    = 0x0F,
  .contxt2_attr   = 0x07,
  .center_box     = true,
  .cycle_moves    = true,
  .edit_contents  = false,
  .reverse_use    = false,
  .fixed_len      = 0,
  .terminate_keys =
  {
    kESC,
    kENTER,
    0,
  }
};

tMENU1_ENVIRONMENT mn1_environment;

static struct
{
  uint16_t   idx;
  uint16_t   idx2;
  uint16_t   max;
  uint16_t   page;
  uint16_t   first;
  uint16_t   last;
  uint16_t   opage;
  uint16_t   opos;
  bool       solid;
  uint8_t    x;
  uint8_t    y;
  uint8_t    len;
  uint8_t    len2;
  void      *data;
  uint16_t   count;
  uint16_t   vscrollbar_size;
  uint16_t   vscrollbar_pos;
  tMBUFFR    mbuf;
} _mnu1;

#include "dialogio/Menu1_pstr.c" // static
#include "dialogio/Menu1_pdes.c" // static
#include "dialogio/Menu1_ShowCStr.c" // static
#include "dialogio/Menu1_refresh.c" // static
#include "dialogio/Menu1_SubPos.c" // static
#include "dialogio/Menu1_AddPos.c" // static
#include "dialogio/Menu1_RetKey.c" // static
#include "dialogio/Menu1_edit_contents.c" // static
#include "dialogio/Menu1.c"

/*** Menu 2 ***/

tMENU2_SETTING mn2_setting =
{
  .frame_type     = { 0, }, // frame_single
  .shadow_enabled = true,
  .posbar_enabled = true,
  .title_attr     = 0x0F,
  .menu_attr      = 0x07,
  .text_attr      = 0x07,
  .text2_attr     = 0x70,
  .default_attr   = 0x0F,
  .short_attr     = 0x0F,
  .short2_attr    = 0x70,
  .disbld_attr    = 0x07,
  .contxt_attr    = 0x0F,
  .contxt2_attr   = 0x07,
  .center_box     = true,
  .cycle_moves    = true,
  .edit_contents  = false,
  .reverse_use    = false,
  .fixed_len      = 0,
  .terminate_keys =
  {
    kESC,
    kENTER,
    0,
  }
};

tMENU2_ENVIRONMENT mn2_environment;

static struct
{
  uint16_t   idx;
  uint16_t   idx2;
  uint16_t   max;
  uint16_t   page;
  uint16_t   first;
  uint16_t   last;
  uint16_t   opage;
  uint16_t   opos;
  bool       solid;
  uint8_t    x;
  uint8_t    y;
  uint8_t    len;
  uint8_t    len2;
  void      *data;
  uint16_t   count;
  uint16_t   vscrollbar_size;
  uint16_t   vscrollbar_pos;
  tMBUFFR    mbuf;
} _mnu2;

#include "dialogio/Menu2_pstr.c" // static
#include "dialogio/Menu2_pdes.c" // static
#include "dialogio/Menu2_ShowCStr.c" // static
#include "dialogio/Menu2_refresh.c" // static
#include "dialogio/Menu2_SubPos.c" // static
#include "dialogio/Menu2_AddPos.c" // static
#include "dialogio/Menu2_RetKey.c" // static
#include "dialogio/Menu2_edit_contents.c" // static
#include "dialogio/Menu2.c"

/*** File Selector ***/

tFSELECT_ENVIRONMENT fs_environment;

#define MAX_FILES 4096
static const String updir_str[] = "\x06" "\x13" "updir"; // TODO: initialize using `charmap'
#define UPDIR_STR &updir_str
#if linux || unix
#define DRIVE_DIVIDER 0
#else // ! (linux || unix)
#define DRIVE_DIVIDER 1
#endif // ! (linux || unix)

#if GO32
#define MENUDAT_LEN (1 + 12 + 1)
#else // !GO32
#define MENUDAT_LEN (1 + 23 + 1)
#endif // !GO32
typedef String tMNUDAT[MAX_FILES][MENUDAT_LEN+1];

#define DESCDAT_LEN 20
typedef String tDSCDAT[MAX_FILES][DESCDAT_LEN+1];

typedef struct
{
  String name[FILENAME_SIZE+1];
  String_t info;
  uint16_t attr;
  int32_t size;
} tSEARCH;

typedef struct
{
  uint16_t count; // number of elements in `.stuff[]'
  uint16_t drive_count;
  uint16_t match_count;
  tSEARCH stuff[MAX_FILES];
} tSTREAM;

static struct
{
  uint16_t count; // number of elements in `.masks[]'
  String_t masks[20];
  tMNUDAT menudat;
  tDSCDAT descr;
  tSTREAM fstream;
} _fs;

#if WIN32 || WIN64 || WINNT
static char drive_list[129];
#endif // WIN32 || WIN64 || WINNT

#include "dialogio/LookUpMask.c" // static, used in `make_stream'

#if GO32

#include "dialogio/go32/phantom_drive.c" // static, used in `valid_drive'
#include "dialogio/go32/valid_drive.c" // static, used in `make_stream'

#else // !GO32

#include "dialogio/sdl/valid_drive.c" // static, used in `make_stream'

typedef enum { isLess, isMore, isEqual } COMPARE_STR_RESULT_enum;
typedef uint8_t tCOMPARE_STR_RESULT; // holds COMPARE_STR_RESULT_enum

#include "dialogio/sdl/CompareStr.c" // static, used in `make_stream'

#endif // !GO32

#include "dialogio/QuickSort.c" // static, used in `make_stream'

static const String ONE_DOT_STR[] = { "\x01" "." }; // current directory
static const String TWO_DOTS_STR[] = { "\x02" ".." }; // parent directory

#include "dialogio/make_stream.c" // static

static String _path[26][PATH_SIZE+1];
static void (*old_fselect_external_proc) (void);

#include "dialogio/new_fselect_external_proc.c" // static
#include "dialogio/path_filter.c" // static, used in `Fselect'
#include "dialogio/Fselect.c"

#include "dialogio/DialogIO_Init.c"

#pragma pack(pop)
