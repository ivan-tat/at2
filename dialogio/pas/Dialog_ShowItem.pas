// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `Dialog' procedure

procedure ShowItem;
begin
  If (idx2 = 0) then
    EXIT; //Dialog.ShowItem
  If (idx2 <> idx3) then
    ShowCStr(screen_ptr,dbuf[idx3].pos,ystart+num+1,dbuf[idx3].str,
             dl_setting.keys_attr,dl_setting.short_attr);

    ShowCStr(screen_ptr,dbuf[idx2].pos,ystart+num+1,dbuf[idx2].str,
             dl_setting.keys2_attr,dl_setting.short2_attr);
  idx3 := idx2;

  //EXIT //Dialog.ShowItem
end;
