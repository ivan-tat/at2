// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function SameName(mask,str: String): Boolean;
begin
  If (Length(mask) > Length(str)) then
    While (mask[Length(mask)] in ['*','?']) and (Length(mask) > Length(str)) do
      Dec(mask[0]);
  SameName := _IsWild(str,mask,FALSE);
end;
