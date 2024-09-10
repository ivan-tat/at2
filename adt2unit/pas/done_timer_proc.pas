// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure done_timer_proc;
begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:done_timer_proc';
{$ENDIF}
  If NOT timer_initialized then EXIT;
  timer_initialized := FALSE;
  TimerDone;
  TimerRemoveHandler;
end;
