// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function Upper_filename(str: String): String;
begin
{$IFDEF UNIX}
  Upper_filename := str;
{$ELSE}
  Upper_filename := Upper(str);
{$ENDIF}
end;
