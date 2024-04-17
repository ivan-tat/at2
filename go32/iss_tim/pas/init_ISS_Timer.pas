// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure init_ISS_Timer;
begin
  FillChar(ISS_TimersData,SizeOf(ISS_TimersData),#0);
  NewIRQActive:=False;
  TimerSpeed:=0;
end;
