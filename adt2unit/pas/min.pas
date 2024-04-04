// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function min(value: Longint; minimum: Longint): Longint;
begin
  If (value > minimum) then min := value
  else min := minimum;
end;
