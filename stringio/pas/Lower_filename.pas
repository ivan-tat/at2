// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function Lower_filename(str: String): String;
begin
{$IFDEF UNIX}
  Lower_filename := str;
{$ELSE}
  Lower_filename := Lower(str);
{$ENDIF}
end;
