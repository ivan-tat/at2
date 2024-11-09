// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure init_timer_proc;
begin
  _dbg_enter ({$I %FILE%}, 'init_timer_proc');

{$IFNDEF ADT2PLAY}
{$IFDEF GO32V2}
  scroll_ticks := 0;
{$ENDIF} // DEFINED(GO32V2)
  Randomize;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  If timer_initialized then
    begin
      _dbg_leave; EXIT; //init_timer_proc
    end;
  timer_initialized := TRUE;
{$IFNDEF ADT2PLAY}
{$IFNDEF GO32V2}
  TimerInstallHandler(@timer_poll_proc);
{$ENDIF} // NOT DEFINED(GO32V2)
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  TimerSetup(50);

  _dbg_leave; //EXIT //init_timer_proc
end;
