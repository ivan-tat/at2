// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// From `MenuLib2' unit

procedure MenuLib2_Init;
begin
  _dbg_enter ({$I %FILE%}, 'MenuLib2_Init');

  MenuLib2_mn_setting.frame_type     := frame_single;
  MenuLib2_mn_setting.center_box     := FALSE;
  MenuLib2_mn_setting.shadow_enabled := FALSE;
  MenuLib2_mn_setting.cycle_moves    := FALSE;
  MenuLib2_mn_setting.title_attr     := dialog_background+dialog_title;
  MenuLib2_mn_setting.menu_attr      := dialog_background+dialog_text;
  MenuLib2_mn_setting.text_attr      := dialog_background+dialog_item;
  MenuLib2_mn_setting.text2_attr     := dialog_sel_itm_bck+dialog_sel_itm;
  MenuLib2_mn_setting.default_attr   := dialog_def_bckg+dialog_def;
  MenuLib2_mn_setting.short_attr     := dialog_background+dialog_short;
  MenuLib2_mn_setting.short2_attr    := dialog_sel_itm_bck+dialog_sel_short;
  MenuLib2_mn_setting.disbld_attr    := dialog_background+dialog_item_dis;
  MenuLib2_mn_setting.contxt_attr    := dialog_background+dialog_context;
  MenuLib2_mn_setting.contxt2_attr   := dialog_background+dialog_context_dis;
  MenuLib2_mn_setting.terminate_keys[3] := kTAB;
  MenuLib2_mn_setting.terminate_keys[4] := kLEFT;
  MenuLib2_mn_setting.terminate_keys[5] := kRIGHT;
  MenuLib2_mn_setting.terminate_keys[6] := kCtrlO;
  MenuLib2_mn_setting.terminate_keys[7] := kF1;
  MenuLib2_mn_setting.terminate_keys[8] := kShTAB;
                         
  MenuLib2_mn_environment.v_dest     := screen_ptr;
  MenuLib2_mn_environment.keystroke  := $0000;
  MenuLib2_mn_environment.context    := '';
  MenuLib2_mn_environment.unpolite   := FALSE;
  MenuLib2_mn_environment.winshade   := TRUE;
  MenuLib2_mn_environment.ext_proc   := NIL;
  MenuLib2_mn_environment.refresh    := NIL;
  MenuLib2_mn_environment.do_refresh := FALSE;
  MenuLib2_mn_environment.preview    := FALSE;
  MenuLib2_mn_environment.descr_len  := 0;
  MenuLib2_mn_environment.descr      := NIL;

  _dbg_leave; //EXIT //MenuLib2_Init
end;
