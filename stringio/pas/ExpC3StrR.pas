// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function ExpC3StrR(str: String; size: Byte; chr: Char): String;
begin
  While (C3StrLen(str) < size) do
    str := str+chr;
  ExpC3StrR := str;
end;
