// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function FlipStr(str: String): String;

var
  idx: Byte;
  result: String;

begin
  result := '';
  For idx := 1 to Length(str) do
    result := str[idx]+result;
  FlipStr := result;
end;
