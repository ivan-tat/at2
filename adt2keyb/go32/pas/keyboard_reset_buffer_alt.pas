// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure keyboard_reset_buffer_alt;
begin
  _dbg_enter ({$I %FILE%}, 'keyboard_reset_buffer_alt');

  If (MEMW[0:$041c]-MEMW[0:$041a] > 5) then
    MEMW[0:$041c] := MEMW[0:$041a];

  _dbg_Leave; //EXIT //keyboard_reset_buffer_alt
end;
