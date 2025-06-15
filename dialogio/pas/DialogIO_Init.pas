// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure DialogIO_Init;

var
  index: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'DialogIO_Init');

  dl_setting.frame_type      := frame_double;
  dl_setting.title_attr      := dialog_background+dialog_title;
  dl_setting.box_attr        := dialog_background+dialog_border;
  dl_setting.text_attr       := dialog_background+dialog_text;
  dl_setting.text2_attr      := dialog_background+dialog_hi_text;
  dl_setting.keys_attr       := dialog_background+dialog_item;
  dl_setting.keys2_attr      := dialog_sel_itm_bck+dialog_sel_itm;
  dl_setting.short_attr      := dialog_background+dialog_short;
  dl_setting.short2_attr     := dialog_sel_itm_bck+dialog_sel_short;
  dl_setting.disbld_attr     := dialog_background+dialog_item_dis;
  dl_setting.contxt_attr     := dialog_background+dialog_context;
  dl_setting.contxt2_attr    := dialog_background+dialog_context_dis;

  mn_setting.frame_type      := frame_double;
  mn_setting.title_attr      := dialog_background+dialog_title;
  mn_setting.menu_attr       := dialog_background+dialog_border;
  mn_setting.text_attr       := dialog_background+dialog_item;
  mn_setting.text2_attr      := dialog_sel_itm_bck+dialog_sel_itm;
  mn_setting.default_attr    := dialog_def_bckg+dialog_def;
  mn_setting.short_attr      := dialog_background+dialog_short;
  mn_setting.short2_attr     := dialog_sel_itm_bck+dialog_sel_short;
  mn_setting.disbld_attr     := dialog_background+dialog_item_dis;
  mn_setting.contxt_attr     := dialog_background+dialog_context;
  mn_setting.contxt2_attr    := dialog_background+dialog_context_dis;
  mn_setting.topic_attr      := dialog_background+dialog_topic;
  mn_setting.hi_topic_attr   := dialog_background+dialog_hi_topic;
  mn_setting.topic_mask_chr  := [];

  mn_environment.v_dest      := screen_ptr;
  dl_environment.keystroke   := $0000;
  mn_environment.keystroke   := $0000;
  dl_environment.context     := '';
  mn_environment.context     := '';
  mn_environment.unpolite    := FALSE;
  dl_environment.input_str   := '';
  mn_environment.winshade    := TRUE;
  mn_environment.intact_area := FALSE;
  mn_environment.ext_proc    := NIL;
  mn_environment.ext_proc_rt := NIL;
  mn_environment.refresh     := NIL;
  mn_environment.do_refresh  := FALSE;
  mn_environment.own_refresh := FALSE;
  mn_environment.preview     := FALSE;
  mn_environment.fixed_start := 0;
  mn_environment.descr_len   := 0;
  mn_environment.descr       := NIL;
  mn_environment.is_editing  := FALSE;
  fs_environment.last_file   := 'FNAME:EXT';
  fs_environment.last_dir    := '';
  mn_environment.xpos        := 0;
  mn_environment.xpos        := 0;
  mn_environment.xsize       := 0;
  mn_environment.ysize       := 0;
  mn_environment.desc_pos    := 0;
  mn_environment.hlight_chrs := 0;
  dl_environment.xpos        := 0;
  dl_environment.ypos        := 0;
  dl_environment.xsize       := 0;
  dl_environment.ysize       := 0;
  dl_environment.cur_item    := 1;
  dl_environment.ext_proc    := NIL;

  For index := 1 to 26 do
    path[index] := CHR(ORD('a')+PRED(index))+':'+PATHSEP;

  _dbg_leave; //EXIT //DialogIO_Init
end;
