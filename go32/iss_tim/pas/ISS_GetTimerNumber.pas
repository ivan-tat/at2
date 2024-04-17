// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

Function ISS_GetTimerNumber(TimerProc : Pointer) : Longint;
Var Counter : DWord;
Begin
 For Counter:=1 To ISS_MaxTimers Do Begin
   With ISS_TimersData[Counter] Do Begin
     If TActive And (TProc=TimerProc) Then Begin
       ISS_GetTimerNumber:=Counter;
       Exit;
      End;
    End;
  End;
End;
