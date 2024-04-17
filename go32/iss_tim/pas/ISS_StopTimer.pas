// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

Function ISS_StopTimer(TimerProc : Pointer) : Boolean;
Var TNumber    : Word;
    Counter    : Word;
    LastTimer  : Boolean;
Begin
 Disable;
 TNumber:=0;
 For Counter:=1 To ISS_MaxTimers Do Begin
   With ISS_TimersData[Counter] Do Begin
     If TActive And (TProc=TimerProc) Then TNumber:=Counter;
    End;
  End;
 If TNumber=0 Then Begin
   ISS_TimerError:=ISS_TENotFound;
   ISS_StopTimer:=False;
  End Else Begin
   ISS_TimersData[TNumber].TActive:=False;
   LastTimer:=True;
   For Counter:=1 To ISS_MaxTimers Do Begin
     If ISS_TimersData[Counter].TActive=True Then LastTimer:=False;
    End;
   If LastTimer Then Begin
     TimerSpeed:=0;
     SetTimerSpeed(PIT_LATENCY_MAX);
     Set_PM_Interrupt(PIT_IRQ,OldTimer);
     Unlock_Data(DosMemSelector,SizeOf(DosMemSelector));
     Unlock_Data(ISS_TimersData,SizeOf(ISS_TimersData));
     UnLock_Code(@SysTimerIRQ,DWord(@SysTimerIRQ_Dummy)-DWord(@SysTimerIRQ));
     UnLock_Code(@UpdateUserTimers,DWord(@UpdateUserTimers_Dummy)-DWord(@UpdateUserTimers));
    End;
   ISS_StopTimer:=True;
  End;
 Enable;
End;
