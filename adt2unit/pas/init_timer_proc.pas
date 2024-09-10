// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure init_timer_proc;
begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:init_timer_proc';
  scroll_ticks := 0;
{$ENDIF}
  Randomize;
  If timer_initialized then EXIT;
  timer_initialized := TRUE;
{$IFNDEF GO32V2}
  TimerInstallHandler(@timer_poll_proc);
{$ENDIF}
  TimerSetup(50);
end;
