// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function ExpC2StrR(str: String; size: Byte; chr: Char): String;
begin
  While (CStr2Len(str) < size) do
    str := str+chr;
  ExpC2StrR := str;
end;
