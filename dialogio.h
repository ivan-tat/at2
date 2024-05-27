// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef DIALOGIO_H
#define DIALOGIO_H

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

#include "pascal.h"
#include "txtscrio.h"

#pragma pack(push, 1)
typedef struct {
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
#pragma pack(pop)

extern tMENU_ENVIRONMENT mn_environment;

#endif // !defined(DIALOGIO_H)
