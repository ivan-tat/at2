// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function limit_value(value,lo_bound,hi_bound: Longint): Longint;
begin
  If (value > hi_bound) then
    value := hi_bound
  else If (value < lo_bound) then
         value := lo_bound;
  limit_value := value;
end;
