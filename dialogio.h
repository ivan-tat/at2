// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef DIALOGIO_H
#define DIALOGIO_H

#include "pascal.h"
#include "txtscrio.h"

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)
#pragma pack(push, 1)

/*** Dialog ***/

typedef struct
{
  String     frame_type[255+1];
  bool       shadow_enabled;
  uint8_t    title_attr;
  uint8_t    box_attr;
  uint8_t    text_attr;
  uint8_t    text2_attr;
  uint8_t    keys_attr;
  uint8_t    keys2_attr;
  uint8_t    short_attr;
  uint8_t    short2_attr;
  uint8_t    disbld_attr;
  uint8_t    contxt_attr;
  uint8_t    contxt2_attr;
  uint8_t    xstart;
  uint8_t    ystart;
  bool       center_box;
  bool       center_text;
  bool       cycle_moves;
  bool       all_enabled;
  ExtKeyCode terminate_keys[50]; // HINT: (FPC) start index 1
} tDIALOG_SETTING;

typedef struct
{
  ExtKeyCode keystroke;
  String     context[255+1];
  String     input_str[255+1];
  uint8_t    xpos, ypos;
  uint8_t    xsize, ysize;
  uint8_t    cur_item;
  void     (*ext_proc) (void);
} tDIALOG_ENVIRONMENT;

extern tDIALOG_SETTING     dl_setting;
extern tDIALOG_ENVIRONMENT dl_environment;

uint8_t Dialog (const String *text, const String *keys, const String *title,
                uint8_t spos);

/*** Menu ***/

typedef struct
{
  String     frame_type[255+1];
  bool       frame_enabled;
  bool       shadow_enabled;
  bool       posbar_enabled;
  uint8_t    title_attr;
  uint8_t    menu_attr;
  uint8_t    text_attr;
  uint8_t    text2_attr;
  uint8_t    default_attr;
  uint8_t    short_attr;
  uint8_t    short2_attr;
  uint8_t    disbld_attr;
  uint8_t    contxt_attr;
  uint8_t    contxt2_attr;
  uint8_t    topic_attr;
  uint8_t    hi_topic_attr;
  CharSet_t  topic_mask_chr; // HINT: (FPC) set of char
  bool       center_box;
  bool       cycle_moves;
  bool       edit_contents;
  bool       reverse_use;
  bool       show_scrollbar;
  uint8_t    topic_len;
  uint8_t    fixed_len;
  int32_t    homing_pos;
  ExtKeyCode terminate_keys[50]; // HINT: (FPC) start index 1
} tMENU_SETTING;

typedef struct
{
  tSCREEN_MEM *v_dest;
  ExtKeyCode keystroke;
  String   context[255+1];
  bool     unpolite;
  bool     winshade;
  bool     intact_area;
  uint8_t  edit_pos;
  uint16_t curr_page;
  uint16_t curr_pos;
  String   curr_item[255+1];
  void   (*ext_proc) ();
  void   (*ext_proc_rt) ();
  void   (*refresh) ();
  bool     do_refresh;
  bool     own_refresh;
  bool     preview;
  uint8_t  fixed_start;
  uint8_t  descr_len;
  void    *descr;
  bool     is_editing;
  uint8_t  xpos,ypos;
  uint8_t  xsize,ysize;
  uint8_t  desc_pos;
  uint8_t  hlight_chrs;
} tMENU_ENVIRONMENT;

extern tMENU_SETTING     mn_setting;
extern tMENU_ENVIRONMENT mn_environment;

uint16_t Menu (void *data, uint8_t x, uint8_t y, uint16_t spos, uint8_t len,
               uint8_t len2, uint16_t count, const String *title);

/*** Menu 1 ***/

typedef struct
{
  String     frame_type[255+1];
  bool       shadow_enabled;
  bool       posbar_enabled;
  uint8_t    title_attr;
  uint8_t    menu_attr;
  uint8_t    text_attr;
  uint8_t    text2_attr;
  uint8_t    default_attr;
  uint8_t    short_attr;
  uint8_t    short2_attr;
  uint8_t    disbld_attr;
  uint8_t    contxt_attr;
  uint8_t    contxt2_attr;
  bool       center_box;
  bool       cycle_moves;
  bool       edit_contents;
  bool       reverse_use;
  uint8_t    fixed_len;
  ExtKeyCode terminate_keys[50]; // HINT: (FPC) start index 1
} tMENU1_SETTING;

typedef struct
{
  tSCREEN_MEM *v_dest;
  ExtKeyCode   keystroke;
  String       context[255+1];
  bool         unpolite;
  bool         winshade;
  uint8_t      edit_pos;
  uint16_t     curr_pos;
  void       (*ext_proc) ();
  void       (*refresh) ();
  bool         do_refresh;
  bool         preview;
  uint8_t      descr_len;
  void        *descr;
} tMENU1_ENVIRONMENT;

// Aliases
#define mn1_setting     MenuLib1_mn_setting     // TODO: remove when done
#define mn1_environment MenuLib1_mn_environment // TODO: remove when done
#define Menu1           MenuLib1_Menu           // TODO: remove when done

extern tMENU1_SETTING     mn1_setting;
extern tMENU1_ENVIRONMENT mn1_environment;

uint16_t Menu1 (void *data, uint8_t x, uint8_t y, uint16_t spos, uint8_t len,
                uint8_t len2, uint16_t count, const String *title);

/*** Menu 2 ***/

typedef struct
{
  String     frame_type[255+1];
  bool       shadow_enabled;
  bool       posbar_enabled;
  uint8_t    title_attr;
  uint8_t    menu_attr;
  uint8_t    text_attr;
  uint8_t    text2_attr;
  uint8_t    default_attr;
  uint8_t    short_attr;
  uint8_t    short2_attr;
  uint8_t    disbld_attr;
  uint8_t    contxt_attr;
  uint8_t    contxt2_attr;
  bool       center_box;
  bool       cycle_moves;
  bool       edit_contents;
  bool       reverse_use;
  uint8_t    fixed_len;
  ExtKeyCode terminate_keys[50]; // HINT: (FPC) start index 1
} tMENU2_SETTING;

typedef struct
{
  tSCREEN_MEM *v_dest;
  ExtKeyCode   keystroke;
  String       context[255+1];
  bool         unpolite;
  bool         winshade;
  uint8_t      edit_pos;
  uint16_t     curr_pos;
  void       (*ext_proc) ();
  void       (*refresh) ();
  bool         do_refresh;
  bool         preview;
  uint8_t      descr_len;
  void        *descr;
} tMENU2_ENVIRONMENT;

// Aliases
#define mn2_setting     MenuLib2_mn_setting     // TODO: remove when done
#define mn2_environment MenuLib2_mn_environment // TODO: remove when done
#define Menu2           MenuLib2_Menu           // TODO: remove when done

extern tMENU2_SETTING     mn2_setting;
extern tMENU2_ENVIRONMENT mn2_environment;

uint16_t Menu2 (void *data, uint8_t x, uint8_t y, uint16_t spos, uint8_t len,
                uint8_t len2, uint16_t count, const String *title);

/*** File Selector ***/

#if GO32
#define FILENAME_SIZE 12
#define DIR_SIZE 80
#define PATH_SIZE 80
#else // !GO32
#define FILENAME_SIZE 255
#define DIR_SIZE 170
#define PATH_SIZE 255
#endif // !GO32

typedef struct
{
  String last_file[FILENAME_SIZE+1];
  String last_dir[DIR_SIZE+1];
} tFSELECT_ENVIRONMENT;

extern tFSELECT_ENVIRONMENT fs_environment;

String_t Fselect (const String *mask);

void DialogIO_Init (void);

#pragma pack(pop)

#endif // !defined(DIALOGIO_H)
