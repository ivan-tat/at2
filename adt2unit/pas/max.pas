// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function max(value: Longint; maximum: Longint): Longint;
begin
  If (value < maximum) then max := value
  else max := maximum;
end;
