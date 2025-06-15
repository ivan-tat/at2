// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function pstr2(item: Word): String;

var
  idx: Byte;
  temp,result: String;

begin
  If (item <= mnu_count) then
    Move(pBYTE(mnu_data)[(item-1)*(mnu_len+1)],temp,mnu_len+1)
  else temp := '';
  If NOT solid then temp := ExpStrR(temp,mnu_len-2,' ')
  else temp := ExpStrR(temp,mnu_len,' ');
  If (mn_setting.fixed_len <> 0) then result := temp
  else begin
         result := '';
         For idx := 1 to Length(temp) do
         If (temp[idx] in mn_setting.topic_mask_chr) then
           result := result+'`'+temp[idx]+'`'
         else result := result+temp[idx];
       end;
  pstr2 := result;
end;
