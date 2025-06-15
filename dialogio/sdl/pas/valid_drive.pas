// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function valid_drive(drive: Char; var info: String): Boolean;

var
  idx: Byte;

begin
  valid_drive := FALSE;
  info := '';
  {$IFNDEF UNIX}
  idx := 0;
  For idx := 0 to 128 do
    If (drive_list[idx] = drive) then
      begin
        info := 'DRiVE';
        BREAK;
      end;
  {$ENDIF}
  If (info <> '') then valid_drive := TRUE;
end;
