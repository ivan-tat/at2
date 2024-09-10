// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure TimerSetup(Hz: Longint);
begin
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:TimerSetup';
  If (Hz < PIT_FREQ_MIN) then Hz := PIT_FREQ_MIN;
  If (Hz > PIT_FREQ_MAX) then Hz := PIT_FREQ_MAX;
  ISS_DisableTimerIRQ;
  If (timer_poll_proc_ptr <> NIL) then ISS_StopTimer(timer_poll_proc_ptr);
  timer_poll_proc_ptr := @timer_poll_proc;
  ISS_StartTimer(timer_poll_proc_ptr,ISS_TimerSpeed DIV Hz);
  ISS_EnableTimerIRQ;
end;
