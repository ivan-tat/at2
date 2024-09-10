// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function TimerCallback(interval: Uint32; param: Pointer):Uint32; cdecl;
begin
  If (@timer_handler <> NIL) then timer_handler;
  TimerCallback := _interval; // trick to alter delay rate on the fly
end;

procedure TimerSetup(Hz: Longint); cdecl;
begin
  _interval := 1000 DIV Hz;
  // only activate timer once, later only alter delay rate
  If (TimerID = NIL) then
    begin
      TimerID := SDL_AddTimer(_interval,SDL_NewTimerCallback(@TimerCallBack),NIL);
      If (TimerID = NIL) then
        Writeln('SDL: Error creating timer');
    end;
  snd_SetTimer(Hz);
end;
