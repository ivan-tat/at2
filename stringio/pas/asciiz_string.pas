// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function asciiz_string(str: String): String;
begin
  If (Pos(#0,str) <> 0) then asciiz_string := Copy(str,1,Pos(#0,str)-1)
  else asciiz_string := '';
end;
