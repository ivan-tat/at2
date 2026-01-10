// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `INSTRUMENT_CONTROL_edit' procedure

function _ftune(ftune: Integer): Integer;

var
  temp: Integer;

begin
  If (ftune DIV 18 = 0) and (ftune <> 0) then temp := 0
  else temp := ABS(ftune) DIV 18;
  _ftune := (ftune DIV min(ABS(ftune),1))*temp;
end;
