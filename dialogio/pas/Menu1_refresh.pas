// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// From `MenuLib1' unit

procedure refresh;

{$I Menu1_ShowCStr.pas}

begin
  _dbg_enter ({$I %FILE%}, 'refresh');

  If (page = opage) and (idx2 = opos) and NOT MenuLib1_mn_environment.do_refresh then
    begin
      _dbg_leave; EXIT; //refresh
    end
  else begin
         opage := page;
         opos  := idx2;
         MenuLib1_mn_environment.do_refresh := FALSE;
       end;

  For idx := page to mnu_len2+page-1 do
    If (idx = idx2+page-1) then
      ShowCStr_clone(MenuLib1_mn_environment.v_dest,mnu_x+1,mnu_y+idx2,
                     ExpStrR(pstr(idx2+page-1)+pdes(idx2+page-1),
                     max+(Length(pstr(idx2+page-1))+Length(pdes(idx2+page-1))-
                     CStrLen(pstr(idx2+page-1)+pdes(idx2+page-1))),' '),
                     MenuLib1_mn_setting.text2_attr,MenuLib1_mn_setting.short2_attr,
                     MenuLib1_mn_setting.text_attr,MenuLib1_mn_setting.short_attr)
    else
      If mbuf[idx].use then
        ShowCStr(MenuLib1_mn_environment.v_dest,mnu_x+1,mnu_y+idx-page+1,
                 ExpStrR(pstr(idx)+pdes(idx),
                 max+(Length(pstr(idx))+Length(pdes(idx2+page-1))-
                 CStrLen(pstr(idx)+pdes(idx))),' '),
                 MenuLib1_mn_setting.text_attr,MenuLib1_mn_setting.short_attr)
      else
        ShowCStr(MenuLib1_mn_environment.v_dest,mnu_x+1,mnu_y+idx-page+1,
                 ExpStrR(pstr(idx)+pdes(idx),
                 max+(Length(pstr(idx))+Length(pdes(idx2+page-1))-
                 CStrLen(pstr(idx)+pdes(idx))),' '),
                 MenuLib1_mn_setting.disbld_attr,MenuLib1_mn_setting.disbld_attr);

  vscrollbar_pos :=
    VScrollBar(MenuLib1_mn_environment.v_dest,mnu_x+max+1,mnu_y+1,temp2,mnu_count,idx2+page-1,
               vscrollbar_pos,MenuLib1_mn_setting.menu_attr,MenuLib1_mn_setting.menu_attr);

  _dbg_leave; //EXIT //refresh
end;
