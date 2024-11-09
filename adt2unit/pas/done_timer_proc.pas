// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure done_timer_proc;
begin
  _dbg_enter ({$I %FILE%}, 'done_timer_proc');

  If NOT timer_initialized then
    begin
      _dbg_leave; EXIT; //done_timer_proc
    end;
  timer_initialized := FALSE;
  TimerDone;
{$IFNDEF ADT2PLAY}
  TimerRemoveHandler;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

  _dbg_leave; //EXIT //done_timer_proc
end;
