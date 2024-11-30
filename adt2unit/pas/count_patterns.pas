// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure count_patterns(var patterns: Byte);

var
  temp1: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'count_patterns');

  patterns := 0;
  For temp1 := 0 to PRED(max_patterns) do
    begin
      realtime_gfx_poll_proc;
      If NOT is_data_empty(pattdata^[temp1 DIV 8][temp1 MOD 8],PATTERN_SIZE) then
        patterns := temp1+1;
    end;

  _dbg_leave; //EXIT //count_patterns
end;
