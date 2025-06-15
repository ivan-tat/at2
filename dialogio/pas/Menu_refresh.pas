// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure refresh;

{$I Menu_ShowCStr.pas}

var
  item_str,item_str_alt,
  item_str2,item_str2_alt: String;
  desc_str,desc_str2,desc_str3: String;

begin { refresh }
  _dbg_enter ({$I %FILE%}, 'refresh');

  If (page = opage) and (idx2 = opos) and NOT mn_environment.do_refresh then
    begin
      _dbg_leave; EXIT; //refresh
    end
  else begin
         opage := page;
         opos  := idx2;
         mn_environment.do_refresh := FALSE;
       end;

  If NOT mn_environment.own_refresh then
    For idx := page to mnu_len2+page-1 do
      begin
        item_str := pstr(idx-page+1);
        item_str_alt := pstr(idx);
        item_str2 := pstr2(idx2+page-1);
        item_str2_alt := pstr2(idx);
        desc_str := pdes(idx-page+1);
        desc_str2 := pdes(idx);
        desc_str3 := pdes(idx2+page-1);

        If (mn_environment.hlight_chrs <> 0) and (item_str <> '') then
          item_str := '~'+Copy(item_str,1,mn_environment.hlight_chrs)+
                      '~'+Copy(item_str,mn_environment.hlight_chrs+1,Length(item_str)-mn_environment.hlight_chrs);
        If (mn_environment.hlight_chrs <> 0) and (item_str_alt <> '') then
          item_str_alt := '~'+Copy(item_str_alt,1,mn_environment.hlight_chrs)+
                          '~'+Copy(item_str_alt,mn_environment.hlight_chrs+1,Length(item_str_alt)-mn_environment.hlight_chrs);
        If (mn_environment.hlight_chrs <> 0) and (item_str2 <> '') then
          item_str2 := '~'+Copy(item_str2,1,mn_environment.hlight_chrs)+
                       '~'+Copy(item_str2,mn_environment.hlight_chrs+1,Length(item_str2)-mn_environment.hlight_chrs);
        If (mn_environment.hlight_chrs <> 0) and (item_str2_alt <> '') then
          item_str2_alt := '~'+Copy(item_str2_alt,1,mn_environment.hlight_chrs)+
                           '~'+Copy(item_str2_alt,mn_environment.hlight_chrs+1,Length(item_str2_alt)-mn_environment.hlight_chrs);

        If (idx = idx2+page-1) then
          ShowCStr_clone(mn_environment.v_dest,mnu_x+1,mnu_y+idx2,
                         ExpStrR(item_str2+desc_str3,
                         max+(Length(item_str2)+Length(desc_str3)-
                         (C3StrLen(item_str2)+CStrLen(desc_str3))),' '),
                         mn_setting.text2_attr,
                         mn_setting.short2_attr,
                         mn_setting.text_attr,
                         mn_setting.short_attr)
        else
          If (idx-page+1 <= mnu_topic_len) then
            ShowCStr(mn_environment.v_dest,mnu_x+1,mnu_y+idx-page+1,
                     ExpStrR(item_str+desc_str,
                     max+(Length(item_str)+Length(desc_str3)-
                     CStrLen(item_str+desc_str)),' '),
                     mn_setting.topic_attr,
                     mn_setting.hi_topic_attr)
          else
            If mbuf[idx].use then
              ShowC3Str(mn_environment.v_dest,mnu_x+1,mnu_y+idx-page+1,
                        ExpStrR(item_str2_alt+desc_str2,
                        max+(Length(item_str2_alt)+Length(desc_str3)-
                        (C3StrLen(item_str2_alt)+CStrLen(desc_str2))),' '),
                        mn_setting.text_attr,
                        mn_setting.short_attr,
                        mn_setting.topic_attr)
            else
              ShowCStr(mn_environment.v_dest,mnu_x+1,mnu_y+idx-page+1,
                       ExpStrR(item_str_alt+desc_str2,
                       max+(Length(item_str_alt)+Length(desc_str3)-
                       CStrLen(item_str_alt+desc_str2)),' '),
                       mn_setting.disbld_attr,
                       mn_setting.disbld_attr);
      end;

  If mn_setting.show_scrollbar then
    vscrollbar_pos :=
      VScrollBar(mn_environment.v_dest,mnu_x+max+1,mnu_y+1-mn_setting.topic_len,
                 temp2,mnu_count,idx2+page-1,
                 vscrollbar_pos,mn_setting.menu_attr,mn_setting.menu_attr);

  _dbg_leave; //EXIT //refresh
end;
