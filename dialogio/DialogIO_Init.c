// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void DialogIO_Init (void)
{
  DBG_ENTER ("DialogIO_Init");

  AT2_char_t t[8+1];

  t[8] = '\0';
  UTF8nstr_to_AT2 (t, 8, FRAME_SOLID_TYPE1);
  StrToString (frame_solid_type1, (char *)t, 8);
  UTF8nstr_to_AT2 (t, 8, FRAME_SOLID_TYPE2);
  StrToString (frame_solid_type2, (char *)t, 8);
  UTF8nstr_to_AT2 (t, 8, FRAME_SINGLE);
  StrToString (frame_single, (char *)t, 8);
  UTF8nstr_to_AT2 (t, 8, FRAME_DOUBLE);
  StrToString (frame_double, (char *)t, 8);
  UTF8nstr_to_AT2 (t, 8, FRAME_DOUBLE_SIDE);
  StrToString (frame_double_side, (char *)t, 8);
  UTF8nstr_to_AT2 (t, 8, FRAME_DOUBLE_TOP);
  StrToString (frame_double_top, (char *)t, 8);

  CopyString (dl_setting.frame_type, frame_double, 8);
  //dl_setting.shadow_enabled
  dl_setting.title_attr      = dialog_background + dialog_title;
  dl_setting.box_attr        = dialog_background + dialog_border;
  dl_setting.text_attr       = dialog_background + dialog_text;
  dl_setting.text2_attr      = dialog_background + dialog_hi_text;
  dl_setting.keys_attr       = dialog_background + dialog_item;
  dl_setting.keys2_attr      = dialog_sel_itm_bck + dialog_sel_itm;
  dl_setting.short_attr      = dialog_background + dialog_short;
  dl_setting.short2_attr     = dialog_sel_itm_bck + dialog_sel_short;
  dl_setting.disbld_attr     = dialog_background + dialog_item_dis;
  dl_setting.contxt_attr     = dialog_background + dialog_context;
  dl_setting.contxt2_attr    = dialog_background + dialog_context_dis;
  //dl_setting.xstart
  //dl_setting.ystart
  //dl_setting.center_box
  //dl_setting.center_text
  //dl_setting.cycle_moves
  //dl_setting.all_enabled
  //dl_setting.terminate_keys

  CopyString (mn_setting.frame_type, frame_double, 8);
  //mn_setting.frame_enabled
  //mn_setting.shadow_enabled
  //mn_setting.posbar_enabled
  mn_setting.title_attr      = dialog_background + dialog_title;
  mn_setting.menu_attr       = dialog_background + dialog_border;
  mn_setting.text_attr       = dialog_background + dialog_item;
  mn_setting.text2_attr      = dialog_sel_itm_bck + dialog_sel_itm;
  mn_setting.default_attr    = dialog_def_bckg + dialog_def;
  mn_setting.short_attr      = dialog_background + dialog_short;
  mn_setting.short2_attr     = dialog_sel_itm_bck + dialog_sel_short;
  mn_setting.disbld_attr     = dialog_background + dialog_item_dis;
  mn_setting.contxt_attr     = dialog_background + dialog_context;
  mn_setting.contxt2_attr    = dialog_background + dialog_context_dis;
  mn_setting.topic_attr      = dialog_background + dialog_topic;
  mn_setting.hi_topic_attr   = dialog_background + dialog_hi_topic;
  memset (mn_setting.topic_mask_chr, 0, sizeof (mn_setting.topic_mask_chr));
  //mn_setting.center_box
  //mn_setting.cycle_moves
  //mn_setting.edit_contents
  //mn_setting.reverse_use
  //mn_setting.show_scrollbar
  //mn_setting.topic_len
  //mn_setting.fixed_len
  //mn_setting.homing_pos
  //mn_setting.terminate_keys

  dl_environment.keystroke   = 0; // no key
  StrToString (dl_environment.context, "", 255);
  StrToString (dl_environment.input_str, "", 255);
  dl_environment.xpos        = 0;
  dl_environment.ypos        = 0;
  dl_environment.xsize       = 0;
  dl_environment.ysize       = 0;
  dl_environment.cur_item    = 1;
  dl_environment.ext_proc    = NULL;

  mn_environment.v_dest      = screen_ptr;
  mn_environment.keystroke   = 0; // no key
  StrToString (mn_environment.context, "", 255);
  mn_environment.unpolite    = false;
  mn_environment.winshade    = true;
  mn_environment.intact_area = false;
  //mn_environment.edit_pos
  //mn_environment.curr_page
  //mn_environment.curr_pos
  //mn_environment.curr_item
  mn_environment.ext_proc    = NULL;
  mn_environment.ext_proc_rt = NULL;
  mn_environment.refresh     = NULL;
  mn_environment.do_refresh  = false;
  mn_environment.own_refresh = false;
  mn_environment.preview     = false;
  mn_environment.fixed_start = 0;
  mn_environment.descr_len   = 0;
  mn_environment.descr       = NULL;
  mn_environment.is_editing  = false;
  mn_environment.xpos        = 0;
  mn_environment.ypos        = 0;
  mn_environment.xsize       = 0;
  mn_environment.ysize       = 0;
  mn_environment.desc_pos    = 0;
  mn_environment.hlight_chrs = 0;

  StrToString (fs_environment.last_file, "FNAME:EXT", FILENAME_SIZE);
  StrToString (fs_environment.last_dir, "", DIR_SIZE);

  for (uint8_t i = 0; i < 26; i++)
  {
    SetLength (_path[i], 3);
    GetStr (_path[i])[0] = 'a' + i;
    GetStr (_path[i])[1] = ':';
    GetStr (_path[i])[2] = PATHSEP;
  }

  DBG_LEAVE (); //EXIT //DialogIO_Init
}
