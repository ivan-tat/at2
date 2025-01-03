// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure keyboard_reset_buffer;
begin
  _dbg_enter ({$I %FILE%}, 'keyboard_reset_buffer');

  MEMW[0:$041c] := MEMW[0:$041a];

  _dbg_leave; //EXIT //keyboard_reset_buffer
end;
