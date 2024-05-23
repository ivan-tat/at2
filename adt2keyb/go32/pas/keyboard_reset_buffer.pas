// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure keyboard_reset_buffer;
begin
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2KEYB.PAS:keyboard_reset_buffer';
  MEMW[0:$041c] := MEMW[0:$041a];
end;
