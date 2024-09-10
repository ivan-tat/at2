// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function get_num_files: Byte;

var
  idx,result: Byte;

begin
  result := 18;
  For idx := 1 to 18 do
    If NOT opl3_record_channel[idx] then Dec(result);
  If (result <> 0) then get_num_files := result
  else get_num_files := 1;
end;
