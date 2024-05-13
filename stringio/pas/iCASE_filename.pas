// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function iCASE_filename(str: String): String;
begin
{$IFDEF UNIX}
  iCASE_filename := str;
{$ELSE}
  iCASE_filename := iCASE(str);
{$ENDIF}
end;
