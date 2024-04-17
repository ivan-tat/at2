// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

Procedure UpdateUserTimers;
Type Proc   = Procedure;
Var Counter : Word;
Begin
 For Counter:=1 To ISS_MaxTimers Do Begin
   With ISS_TimersData[Counter] Do Begin
     If TActive Then Begin
       Inc(TCount,TimerSpeed);
       If (TCount>TSpeed) Then Begin
         Dec(TCount,TSpeed);
         TPrevCount:=TCount;
         Proc(TProc); { ■ Calling the specified routine ■ }
        End;
      End;
    End;
  End;
End;

Procedure UpdateUserTimers_Dummy; Begin End;
