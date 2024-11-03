// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure TimerSetup(Hz: Longint);
begin
  _dbg_enter ({$I %FILE%}, 'TimerSetup');

  If (Hz < PIT_FREQ_MIN) then Hz := PIT_FREQ_MIN;
  If (Hz > PIT_FREQ_MAX) then Hz := PIT_FREQ_MAX;
  ISS_DisableTimerIRQ;
  If (timer_poll_proc_ptr <> NIL) then ISS_StopTimer(timer_poll_proc_ptr);
  timer_poll_proc_ptr := @timer_poll_proc;
  ISS_StartTimer(timer_poll_proc_ptr,ISS_TimerSpeed DIV Hz);
  ISS_EnableTimerIRQ;

  _dbg_leave; //EXIT //TimerSetup
end;
