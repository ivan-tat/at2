// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `INSTRUMENT_CONTROL_edit' procedure

function min0(number: Integer): Integer;
begin
  If (number > 0) then min0 := number
  else min0 := 0;
end;
