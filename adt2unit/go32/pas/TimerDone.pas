// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure TimerDone;
begin
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:TimerDone';
  ISS_DisableTimerIRQ;
  If (timer_poll_proc_ptr <> NIL) then ISS_StopTimer(timer_poll_proc_ptr);
  timer_poll_proc_ptr := NIL;
  ISS_EnableTimerIRQ;
end;
